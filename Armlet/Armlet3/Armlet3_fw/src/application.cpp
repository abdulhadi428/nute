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

#include "lvl1_assym.h"
#include "evt_mask.h"

#include "kl_sd.h"

App_t App;

static EventListener EvtLstnrRadioRx, EvtListenerKeys, EvtListenerPill;
static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

// Prototypes

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    // Open file
//    FRESULT rslt;
//    rslt = f_open(&SD.File, "settings.ini", FA_READ+FA_OPEN_EXISTING);
//    Uart.Printf("OpenFile: %u\r", (uint8_t)rslt);
//    if(rslt == FR_OK) {
//        Uart.Printf("Size: %u\r", SD.File.fsize);
//        uint32_t N=0;
//        rslt = f_read(&SD.File, Str, 250, (UINT*)&N);
//        if(rslt == FR_OK) {
//            Uart.Printf("N: %u; Str: %s\r", N, Str);
//        }
//        else Uart.Printf("ReadFile: %u\r", (uint8_t)rslt);
//        f_close(&SD.File);
//    }
//    Uart.Printf("1\r");
//    uint32_t Count=0;
//    iniReadUint32("Sound", "Count", "settings.ini", &Count);
//    Uart.Printf("Cnt: %u\r", Count);

//    Color_t c = clBlack;

//#define PktSZ   4
//    uint8_t Buf[PktSZ], Rslt1 = FAILURE, Rslt2 = FAILURE;
//    for(uint8_t i=0; i<PktSZ; i++) Buf[i] = i;

    // Events
    uint32_t EvtMsk;
    KeysRegisterEvt(&EvtListenerKeys, EVTMASK_KEYS);
    rLevel1.RegisterEvtRx(&EvtLstnrRadioRx, EVTMASK_RADIO_RX);
    PillRegisterEvtChange(&EvtListenerPill, EVTMASK_PILL);

    while(1) {
        //chThdSleepMilliseconds(999);
//        Rslt1 = rLevel1.AddPktToTx(0, Buf, PktSZ, &Rslt2);
//        Uart.Printf("### %u\r", Rslt1);

        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_KEYS | EVTMASK_PILL);

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

        //Uart.Printf("Evt \r");
        //Lcd.Cls(c);
//        for(uint8_t y=0; y<128; y+=8) {
//            chThdSleepMilliseconds(999);
//            //Lcd.Printf(0, y, clBlue, c, "YA=%u", y);
//            //Beep(BeepBeep);
//
//        }
//        c = (c == clBlack)? clWhite : clBlack;

//        Lcd.Cls(c);
//        switch(c) {
//            case clRed: c = clGreen; break;
//            case clGreen: c = clBlue; break;
//            case clBlue: c = clRed; break;
//            default: c = clRed; break;
//        }
    }
    return 0;
}

// IR
void IRCmdCallback(uint16_t CmdWord) {
    Uart.Printf("%02X\r", CmdWord);
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
            if(OpenFile(&SD.File, "settings.ini", true)) {
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
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
