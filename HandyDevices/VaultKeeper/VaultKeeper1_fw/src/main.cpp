/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib.h"
#include "led.h"
#include "sim900.h"
#include "comline.h"
#include "kl_time.h"
#include "kl_string.h"
#include <string.h>
#include <stdlib.h>
#include "sha1.h"
#include "sensors.h"

#define VERSION_ID      "Minya" // "First" on Quenya. Used for HostKey generation. No more than 20 char
#define HOST_ID         1001

#define RETRY_TIMEOUT   60      // s
class Report_t {
private:
    uint32_t RetryTmr;
    bool ReportIsSent;
    Error_t GetTime(void);
public:
    void Init(void);
    void Task(void);
    void SendNow(void) { ReportIsSent = false; Time.Bypass(&RetryTmr, RETRY_TIMEOUT); }
} Report;

LedBlink_t Led;
char HostKey[41];   // CEF251C525315E1D471FD0C63CB5443BB7DAF7F5

// Prototypes
void GeneralInit(void);
void GenerateHostKey(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();
    //Mdm.On();
    //Mdm.SendSMS("+79169895800", "Aiya Feanaro!");

    // ==== Main cycle ====
    //uint32_t Tmr;
    while (1) {
        Led.Task();
        //if(Delay.Elapsed(&Tmr, 999)) Led.Blink(199);
        Report.Task();
        Sensors.Task();
    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);
    Com.Init();
    Com.Printf("\rVault Keeper1\rHostID: %u\r", HOST_ID);

    Led.Init(GPIOD, 2);
    //Led.On();
    Time.Init();
    Sensors.Init();

    Mdm.Init();
    GenerateHostKey();
    Report.Init();
}


#define IDBASE      0x1FFFF7E8  // Memory address of ID
void GenerateHostKey(void) {
    char FBuf[45];
    // Get uinique CPU id
    uint32_t ID0 = *((uint16_t*)(IDBASE));
    uint32_t ID1 = *((uint16_t*)(IDBASE+2));
    uint32_t ID2 = *((uint32_t*)(IDBASE+4));
    uint32_t ID3 = *((uint32_t*)(IDBASE+8));
    klSPrintf(FBuf, "%S%X4%X4%X8%X8", VERSION_ID, ID0, ID1, ID2, ID3);
    //Com.Printf("ID: %S\r", FBuf);
    Sha1(FBuf);
    strcpy(HostKey, Sha1String);
    Com.Printf("HostKey: %S\r", HostKey);
}

// =============================== Report ======================================
#define URL_HOST        "vaultkeeper.ru"
#define URL_TIME        "/request.php?time"
#define URL_REQUEST     "/request.php"
#define REPORT_MINUTE   23

void Report_t::Init() {
    SendNow();
}

void Report_t::Task(void) {
    // Send report every hour
    static uint8_t FLastHour = 27;  // dummy for first time
    if ((Time.GetMinute() == REPORT_MINUTE) and (Time.GetHour() != FLastHour)) {    // Do not send report twice a minute
        Com.Printf("Time to send report\r");
        SendNow();
    }

    // Send report if new leakage occured
    else if (Sensors.NewProblemOccured) {
        Sensors.NewProblemOccured = false;  // Served
        Com.Printf("Report new problem\r");
        SendNow();
    }

    // Check if report was sent; if not, recend it after timeout
    if(!ReportIsSent) if(Time.SecElapsed(&RetryTmr, RETRY_TIMEOUT)) {
        Com.Printf("==== Sending report ==== ");
        Time.Print();
        Mdm.On();
        if (Mdm.GprsOn() == erOk) {
            if (Mdm.OpenConnection(URL_HOST) == erOk) {
                // Renew time
                if (GetTime() == erOk) {
                    // ==== Send data ====
                    RowData_t *PRow;
                    char *S = Mdm.DataString, ErrStr[SNS_COUNT*7+5+1], *E;    // every sns needs 7 chars; battery needs 5.
                    Error_t r = erOk;
                    while (!SnsBuf.IsEmpty() and (r == erOk)) {
                        PRow = SnsBuf.Read();

                        // Construct error string
                        E = ErrStr;
                        for (uint32_t i=0; i<SNS_COUNT; i++) {
                            if (PRow->SnsArr[i] != 0) E = klSPrintf(E, "%u.%u,", i, PRow->SnsArr[i]);
                        }
                        if (PRow->Battery != 0) klSPrintf(E, "b.%u", PRow->Battery);
                        // Remove ',' at end of string.
                        else // No need if battery printed
                            if (E != ErrStr) // if not empty
                                if (*(E-1) == ',') *(E-1) = 0;
                        Com.Printf("Errors: %S\r", ErrStr);

                        // Construct data string
                        klSPrintf(S, "host_id=%u&water_value=%u&time=%u%u%u%u%u%u&errors=%S&host_key=%S",
                                HOST_ID, PRow->WaterValue,
                                PRow->DateTime.Year, PRow->DateTime.Month, PRow->DateTime.Day,
                                PRow->DateTime.H,    PRow->DateTime.M,     PRow->DateTime.S,
                                ErrStr, HostKey);
                        Com.Printf("Data: %S\r", S);
                        Sha1(S);
                        Com.Printf("Hash: %S\r", Sha1String);

                        // Construct string to send
                        klSPrintf(S, "host_id=%u&water_value=%u&time=%u%u%u%u%u%u&errors=%S&host_hash=%S",
                                HOST_ID, PRow->WaterValue,
                                PRow->DateTime.Year, PRow->DateTime.Month, PRow->DateTime.Day,
                                PRow->DateTime.H,    PRow->DateTime.M,     PRow->DateTime.S,
                                ErrStr, Sha1String);
                        Com.Printf("To send: %S\r", S);
                        if ((r = Mdm.POST(URL_HOST, URL_REQUEST, S)) == erOk) SnsBuf.PrepareToReadNext(); // Sent ok, goto next
                    } // while
                    if (r == erOk) {
                        // Mission completed
                        Com.Printf("==== Report sent ====\r");
                        ReportIsSent = true;
                        FLastHour = Time.GetHour();
                    }
                } // if time
                Mdm.CloseConnection();
            } // if open connection
            Mdm.GprsOff();
        } // if GPRS
        Mdm.Off();
    } // if not sent
}


// keepertime 2012-06-22 18-14-05
Error_t Report_t::GetTime(void) {
    Com.Printf("Receiving Time...\r");
    if (Mdm.GET(URL_HOST, URL_TIME, Mdm.DataString, 30) == erOk) {
        char S[7];
        klStrNCpy(S, &Mdm.DataString[11], 4);
        uint16_t Year = atoi(S);
        klStrNCpy(S, &Mdm.DataString[16], 2);
        uint8_t Month = atoi(S);
        klStrNCpy(S, &Mdm.DataString[19], 2);
        uint8_t Day = atoi(S);
        klStrNCpy(S, &Mdm.DataString[22], 2);
        uint8_t H = atoi(S);
        klStrNCpy(S, &Mdm.DataString[25], 2);
        uint8_t M = atoi(S);
        klStrNCpy(S, &Mdm.DataString[28], 2);
        uint8_t Sec = atoi(S);
        Time.SetDate(Year, Month, Day);
        Time.SetTime(H, M, Sec);
        Com.Printf("New time: ");
        Time.Print();
        return erOk;
    }
    else return erError;
}

