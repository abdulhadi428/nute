/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "lcd2630.h"
#include "peripheral.h"
#include "BeepSequences.h"
#include "VibroSequences.h"
#include "keys.h"
#include "pill.h"
#include "infrared.h"
#include "power.h"
#include "tiny_sprintf.h"

#include "lvl1_assym.h"
#include "evt_mask.h"
#include "kl_sd.h"

#include "ArmletApi.h"

using namespace ArmletApi;

App_t App;

static EventListener EvtLstnrRadioRx, EvtListenerKeys, EvtListenerPill, EvtListenerIR, EvtListenerTmr;
static EventSource IEvtSrcTmr;
static VirtualTimer STmr;

static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

static uint8_t LustraID;

// Timer
struct ITmr_t {
    TIMER_PROC* Proc;
    bool Busy;
    uint32_t Time, Period;
};
static ITmr_t STmrData;

// Pill
struct Med_t {
    int CureID, Charges;
} __attribute__ ((__packed__));
static Med_t Med;

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");
    uint16_t w;

    // Events
    uint32_t EvtMsk;
    KeysRegisterEvt(&EvtListenerKeys, EVTMASK_KEYS);
    rLevel1.RegisterEvtRx(&EvtLstnrRadioRx, EVTMASK_RADIO_RX);
    PillRegisterEvtChange(&EvtListenerPill, EVTMASK_PILL);
    IR.RegisterEvt(&EvtListenerIR, EVTMASK_IR);
    chEvtRegisterMask(&IEvtSrcTmr, &EvtListenerTmr, EVTMASK_TIMER);

    while(1) {
        //chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(0, Buf, PktSZ, &Rslt2);
//        Uart.Printf("### %u\r", Rslt1);

        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_KEYS | EVTMASK_PILL | EVTMASK_IR | EVTMASK_TIMER);

        if(EvtMsk &EVTMASK_KEYS) {
            for(uint8_t i=0; i<KEYS_CNT; i++) {
                if(KeyStatus[i].HasChanged) {
                    if(KeyStatus[i].State == ksReleased) OnButtonRelease(i);
                    else OnButtonPress(i);
                    KeyStatus[i].HasChanged = false;
                }
            } // for
        } // keys

        if(EvtMsk & EVTMASK_RADIO_RX) {
            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK) {
                //Uart.Printf("%A\r", SRxPkt.Data, SRxPkt.Length, ' ');
                //rLevel1.AddPktToTx(0, SRxPkt.Data, SRxPkt.Length);
                OnRadioPacket(SRxPkt.Data, SRxPkt.Length);
            }
        } // if evtmsk

        if(EvtMsk & EVTMASK_PILL) {
            Beeper.Beep(ShortBeep);
            // Read med
            if(Pill[0].Connected) {
                Pill[0].Read((uint8_t*)&Med, sizeof(Med_t));
                OnPillConnect(Med.CureID, Med.Charges);
            }
        }

        if(EvtMsk & EVTMASK_IR) {
            w = IR.RxWord;
            LustraID = w >> 8;
            Uart.Printf("IR ID=%u\r", LustraID);
            Beeper.Beep(ShortBeep);
        }

        if(EvtMsk & EVTMASK_TIMER) {
            Uart.Printf("Tmr\r");
            bool Proceed = false;
            if(STmrData.Proc != NULL) {
                uint32_t Elapsed = chTimeNow() - STmrData.Time;
                STmrData.Time = chTimeNow();
                Proceed = STmrData.Proc(Elapsed);
            }
            // Check if stop timer
            if(!Proceed) {
                chVTReset(&STmr);
                STmrData.Busy = false;
            } // if proceed
        } // if timer
    } // while 1
    return 0;
}

static DIR dir;
FILINFO FileInfo;
//=========================== Command processing ===============================
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    uint8_t b;
    FRESULT res;
    switch(CmdCode) {
        case 0x01:
            b = OK;
            Uart.Cmd(0x90, &b, 1);
            break;

        case 0x50: // Dir
            res = f_opendir(&dir, NULL);
            if (res == FR_OK) {
                for (;;) {
                    res = f_readdir(&dir, &FileInfo);                   /* Read a directory item */
                    if (res != FR_OK || FileInfo.fname[0] == 0) break;  /* Break on error or end of dir */
                    if (FileInfo.fname[0] == '.') continue;             /* Ignore dot entry */
                    Uart.Printf("%S\r", FileInfo.fname);
                }
            }
            break;

        case 0x51:  // GetID
            Uart.Printf("ID=%u\r", rLevel1.GetID());
            break;

        case 0x52:  // SetID
            b = PData[0];
            res = f_open(&SD.File, "settings.ini", FA_CREATE_ALWAYS | FA_WRITE);
            if(res == FR_OK) {
                f_printf(&SD.File, "[Radio]\r\nID=%u\r\n", b);
                f_close(&SD.File);
                Uart.Printf("Written\r");
            }
            rLevel1.SetID(b);
            Uart.Printf("New ID=%u\r", rLevel1.GetID());
            break;

        default: break;
    } // switch
}

// =============================== App class ===================================
void App_t::Init() {
    LustraID = UNKNOWN_ID;
    STmrData.Busy = false;
    chEvtInit(&IEvtSrcTmr);
    // Init shell
    ArmletApi::InitializeShell();

    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}

// Dummy
bool ArmletApi::InitializeShell() { return false; }

void ArmletApi::OnButtonPress(int button_id) {
    Uart.Printf("Btn Press %u\r", button_id);
}
void ArmletApi::OnButtonRelease(int button_id) {
    Uart.Printf("Btn Rlz %u\r", button_id);
}

void ArmletApi::OnRadioPacket(unsigned char* packet, int len) {
    Uart.Printf("%A\r", packet, len, ' ');
}
void ArmletApi::OnPillConnect(int cure_id, int charges) {
    Uart.Printf("Pill %d, %d\r", Med.CureID, Med.Charges);
}

// ================================== API ======================================
unsigned char ArmletApi::GetLustraId() {
    uint8_t b = LustraID;
    LustraID = UNKNOWN_ID;  // Reset ID
    return b;
}

int snprintf(char* buf, int bufSz, char* fmt,...) {
    va_list args;
    va_start(args, fmt);
    uint32_t Cnt = tiny_vsprintf(buf, bufSz, fmt, args);
    va_end(args);
    return Cnt;
}

unsigned char ArmletApi::GetBatteryLevel() { return Power.RemainingPercent; }

// Display
void ArmletApi::DrawPixel(int x, int y, unsigned short c) {
    Lcd.PutBitmap(x, y, 1, 1, &c);
}
void ArmletApi::DrawArea(int x, int y, unsigned short *const pc, int len, int sx, int sy) {
    Lcd.PutBitmap(x, y, sx, sy, pc);
}
void ArmletApi::SetScreenBrightness(uint8_t Percent) {
    Lcd.Brightness(Percent);
}


void ArmletApi::DoVibroAndBeep(int msecs) {
    Vibro.Vibrate(msecs);
    Beeper.Beep(msecs);
}

// Pill
bool ArmletApi::WritePill(int cure_id, int charges) {
    if(!Pill[0].Connected) return false;
    Med.CureID = cure_id;
    Med.Charges = charges;
    return (Pill[0].Write((uint8_t*)&Med, sizeof(Med_t)) == OK);
}

// Radio
void ArmletApi::SendRadioPacket(unsigned char* packet, int len) {
    rLevel1.AddPktToTx(0, packet, len);
}
unsigned short ArmletApi::GetArmletId() { return rLevel1.GetID(); }

// Timer
static void AppTmrCallback(void *p) {
    chEvtBroadcast(&IEvtSrcTmr);
    chVTSetI(&STmr, MS2ST(STmrData.Period), AppTmrCallback, NULL);
}
bool ArmletApi::RequestTimer(TIMER_PROC* timerProc, int period) {
    if(STmrData.Busy) return false;
    STmrData.Busy = true;
    STmrData.Proc = timerProc; // Save Callback
    STmrData.Time = chTimeNow();
    STmrData.Period = period;
    chVTReset(&STmr);
    chVTSet(&STmr, MS2ST(period), AppTmrCallback, NULL);
    return true;
}

int ArmletApi::GetUpTime() { return chTimeNow(); }
void ArmletApi::Sleep(int msces) { chThdSleepMilliseconds(msces); }

unsigned int ArmletApi::GetRandom(unsigned int max) { return Random(max); }

// ============================= File operations ===============================
// return false in case of failure, true in case of success
// Try to open. In case of failure, create new file if bCreate is true, otherwise return error.
bool ArmletApi::OpenFile(FILE* file, const char* filename, bool bCreate) {
    FRESULT Rslt;
    Rslt = f_open((FIL*)file, filename, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
    if(Rslt == FR_OK) return true;
    else {                  // File does not exist
        if(bCreate) {       // Create file if needed
            Rslt = f_open((FIL*)file, filename, FA_CREATE_NEW | FA_READ | FA_WRITE);
            return (Rslt == FR_OK);
        }
        else return false;  // No need to create, return failure
    }
}

// returns length read
int ArmletApi::ReadFile(FILE* file, char* buf, int len) {
    f_lseek((FIL*)file, 0);   // move to beginning
    UINT FLen=0;
    f_read((FIL*)file, buf, len, &FLen);
    return FLen;
}

// returns length written, rewrites file from beginning
int ArmletApi::WriteFile (FILE* file, char* buf, int len) {
    f_lseek((FIL*)file, 0);   // move to beginning
    UINT FLen=0;
    f_write((FIL*)file, buf, len, &FLen);
    return FLen;
}

// return length written, appends to end of file
int ArmletApi::AppendFile(FILE* file, char* buf, int len) {
    if(file == NULL) return 0;
    f_lseek((FIL*)file, f_size((FIL*)file));
    UINT FLen=0;
    f_write((FIL*)file, buf, len, &FLen);
    return FLen;
}

