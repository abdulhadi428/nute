/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib.h"
#include "led.h"
#include "sim900.h"
//#include "comline.h"
#include "kl_time.h"
#include "kl_string.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sha1.h"
#include "sensors.h"
#include "UARTClass.h"
#include "tiny_sprintf.h"
#include "DBG_Unit.h"
#include "GPIO_config.h"

#define VERSION_ID      "Minya" // "First" on Quenya. Used for HostKey generation. No more than 20 char
#define HOST_ID         1002

UART_Class* pUART1;
UART_Class* pUART2;
UART_Class* pUART3;
UART_Class* pUART4;
UART_Class* pUART5;

UART_Class DbgUART;
DBG_Message_Class DbgMessage;

#define RETRY_TIMEOUT   99      // s
class Report_t {
private:
    uint32_t RetryTmr;
    bool ReportIsSent;
    Error_t GetTime(void);
public:
    void Task(void);
    void SendNow(void) { ReportIsSent = false; Time.Bypass(&RetryTmr, RETRY_TIMEOUT); }
} Report;

char HostKey[41];

// Prototypes
void GeneralInit(void);
void GenerateHostKey(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();
    DbgUART.UART_Init(USART2);
    DbgUART.eco_in_flag=1;
    Delay.ms(63);
    DbgMessage.DbgEnable();
    DbgMessage.PrintF("\rVault Keeper1\rHostID: %u\r", HOST_ID);
    GenerateHostKey();
    Time.Init();
    Sensors.Init();
    SimpleGPIO_Init();
    LedOn();
    Mdm.Init();
    Report.SendNow();   // Get time
    Mdm.On();
    //Mdm.SendSMS("+79169895800", "Aiya Feanaro!  ^-^");
    LedOff();
    // ==== Main cycle ====
    while (1) {
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
   // Com.Init();
    Delay.ms(630);




    //
    //
}


#define IDBASE      0x1FFFF7E8  // Memory address of ID
void GenerateHostKey(void) {
    char FBuf[45];
    // Get uinique CPU id
    uint32_t ID0 = *((uint16_t*)(IDBASE));
    uint32_t ID1 = *((uint16_t*)(IDBASE+2));
    uint32_t ID2 = *((uint32_t*)(IDBASE+4));
    uint32_t ID3 = *((uint32_t*)(IDBASE+8));
    tiny_sprintf(FBuf, "%S%X4%X4%X8%X8", VERSION_ID, ID0, ID1, ID2, ID3);
    DbgMessage.PrintF("ID: %s\r", FBuf);
    Sha1(FBuf);
    strcpy(HostKey, Sha1String);
    DbgMessage.PrintF("HostKey: %s\r", HostKey);
}

// =============================== Report ======================================
#define URL_HOST        "vaultkeeper.ru"
#define URL_TIME        "/request.php?time"
#define URL_REPORT      "/report.php"
#define REPORT_MINUTE   18

#define MDM_ENABLE     // DEBUG: comment this to disable modem
void Report_t::Task(void) {
#ifdef MDM_ENABLE
    if(!Time.TimeIsSet) {
    	DbgMessage.PrintF("Need to get time\r");
        SendNow();
    }
#endif

    // Send report every hour
    static uint8_t FLastHour = 0;
    if ((Time.GetMinute() == REPORT_MINUTE) and (Time.GetHour() != FLastHour)) {    // Do not send report twice a minute
    	DbgMessage.PrintF("Time to send report\r");
        SendNow();
    }


    // Send report immediately if leakage occured or vanished
    else if (Sensors.NeedToReport) {
        Sensors.NeedToReport = false;  // Served
        DbgMessage.PrintF("Report new situation\r");
        SendNow();
    }

    // Check if report was sent; if not, recend it after timeout
    if(!ReportIsSent) if(Time.SecElapsed(&RetryTmr, RETRY_TIMEOUT)) {
    	DbgMessage.PrintF("==== Sending report ==== ");
        Time.Print();
#ifdef MDM_ENABLE
        Mdm.On();
        if (Mdm.GprsOn() == erOk) {
            if (Mdm.OpenConnection(URL_HOST) == erOk) {
                // Renew time
                if (GetTime() == erOk) {
#endif
                    // ==== Send data ====
                    RowData_t *PRow;
                    char *S = Mdm.DataString, SnsStr[SNS_COUNT*7+5+1], *E;    // every sns needs 7 chars; battery needs 5.
                    Error_t r = erOk;
                    while (!SnsBuf.IsEmpty() and (r == erOk)) {
                        PRow = SnsBuf.Read();

                        // Construct sensor string
                        E = SnsStr;
                        *E = 0; // Empty string
                        for (uint32_t i=0; i<SNS_COUNT; i++) {
                            if (PRow->SnsArr[i] != 0) E = klSPrintf(E, "%u.%u,", i+1, PRow->SnsArr[i]);
                        }
                        if (PRow->Battery != 0) klSPrintf(E, "b.%u", PRow->Battery);
                        // Remove ',' at end of string.
                        else // No need if battery printed
                            if (E != SnsStr) // if not empty
                                if (*(E-1) == ',') *(E-1) = 0;
                        DbgMessage.PrintF("Errors: %S\r", SnsStr);
                        Delay.ms(630);

                        // Construct data string for hash calculation
                        klSPrintf(S, "host_id=%u&water_value=%u&time=%u4%u2%u2%u2%u2%u2&sensors=%S&host_key=%S",
                                HOST_ID, PRow->WaterValue,
                                PRow->DateTime.Year, PRow->DateTime.Month, PRow->DateTime.Day,
                                PRow->DateTime.H,    PRow->DateTime.M,     PRow->DateTime.S,
                                SnsStr, HostKey);
                        DbgMessage.PrintF("Data: %S\r", S);
                        Sha1(S);
                        DbgMessage.PrintF("Hash: %S\r", Sha1String);
                        Delay.ms(630);
                        // Construct string to send, using S and hash
                        klSPrintf(S, "host_id=%u&water_value=%u&time=%u4%u2%u2%u2%u2%u2&sensors=%S&host_hash=%S",
                                HOST_ID, PRow->WaterValue,
                                PRow->DateTime.Year, PRow->DateTime.Month, PRow->DateTime.Day,
                                PRow->DateTime.H,    PRow->DateTime.M,     PRow->DateTime.S,
                                SnsStr, Sha1String);
                        DbgMessage.PrintF("To send: %S\r", S);
                        Delay.ms(630);
#ifdef MDM_ENABLE
                        if ((r = Mdm.POST(URL_HOST, URL_REPORT, S)) == erOk)
#endif
                            SnsBuf.PrepareToReadNext(); // Sent ok, goto next
                    } // while
                    if (r == erOk) {
                        // Mission completed
                    	DbgMessage.PrintF("==== Report sent ====\r");
                        ReportIsSent = true;
                        FLastHour = Time.GetHour();
                    }
#ifdef MDM_ENABLE
                } // if time
                Mdm.CloseConnection();
            } // if open connection
            Mdm.GprsOff();
        } // if GPRS
        Mdm.Off();
#endif
    } // if not sent
}


// keepertime 2012-06-22 18-14-05
Error_t Report_t::GetTime(void) {
	DbgMessage.PrintF("Receiving Time...\r");
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
        DbgMessage.PrintF("New time: ");
        Time.Print();
        Time.TimeIsSet = true;
        return erOk;
    }
    else return erError;
}

