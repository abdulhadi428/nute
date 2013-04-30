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

#include "lvl1_assym.h"
#include "evt_mask.h"
#include "kl_sd.h"

#include "ArmletApi.h"

App_t App;

static EventListener EvtLstnrRadioRx, EvtListenerKeys, EvtListenerPill, EvtListenerIR;
static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

// Prototypes

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

    while(1) {
        //chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(0, Buf, PktSZ, &Rslt2);
//        Uart.Printf("### %u\r", Rslt1);

        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_KEYS | EVTMASK_PILL | EVTMASK_IR);

        if(EvtMsk &EVTMASK_KEYS) {
            if((KeyStatus[0] == KEY_PRESSED) or (KeyStatus[1] == KEY_PRESSED) or (KeyStatus[2] == KEY_PRESSED)) {
                Beeper.Beep(BeepBeep);
            }
            else if((KeyStatus[3] == KEY_PRESSED) or (KeyStatus[4] == KEY_PRESSED) or (KeyStatus[5] == KEY_PRESSED)) {
                Vibro.Vibrate(BrrBrr);
            }
            else if((KeyStatus[6] == KEY_PRESSED) or (KeyStatus[7] == KEY_PRESSED) or (KeyStatus[8] == KEY_PRESSED)) {
                Beeper.Beep(ShortBeep);
                Vibro.Vibrate(ShortBrr);
            }
        }

        if(EvtMsk & EVTMASK_RADIO_RX) {
            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK) {
                Uart.Printf("%A\r", SRxPkt.Data, SRxPkt.Length, ' ');
                rLevel1.AddPktToTx(0, SRxPkt.Data, SRxPkt.Length);
            }
        } // if evtmsk

        if(EvtMsk & EVTMASK_PILL) {
            Beeper.Beep(ShortBeep);
        }

        if(EvtMsk & EVTMASK_IR) {
            w = IR.RxWord;
            w >>= 8;
            Uart.Printf("IR ID=%u\r", w);
            Beeper.Beep(ShortBeep);
        }
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
    // Init shell
    ArmletApi::InitializeShell();

    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}

bool ArmletApi::InitializeShell() { return false; }

// ============================= File operations ===============================
using namespace ArmletApi;
// return false in case of failure, true in case of success
// Try to open. In case of failure, create new file if bCreate is true, otherwise return error.
bool OpenFile(FILE* file, const char* filename, bool bCreate) {
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
/*
// returns length read
int ReadFile(FILE* file, char* buf, int len) {
    f_lseek(file, 0);   // move to beginning
    UINT FLen=0;
    f_read(file, buf, len, &FLen);
    return FLen;
}

// returns length written, rewrites file from beginning
int WriteFile (FILE* file, char* buf, int len) {
    f_lseek(file, 0);   // move to beginning
    UINT FLen=0;
    f_write(file, buf, len, &FLen);
    return FLen;
}

// return length written, appends to end of file
int AppendFile(FILE* file, char* buf, int len) {
    if(file == NULL) return 0;
    f_lseek(file, f_size(file));
    UINT FLen=0;
    f_write(file, buf, len, &FLen);
    return FLen;
}
*/

