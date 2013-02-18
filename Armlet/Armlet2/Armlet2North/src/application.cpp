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

#include "kl_sd.h"

App_t App;

char Str[255];

// Prototypes

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 128);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    Color_t c = clBlack;

    // Open file
    FRESULT rslt;
    Uart.Printf("1\r");
    rslt = f_open(&SD.File, "settings.ini", FA_READ+FA_OPEN_EXISTING);
    Uart.Printf("OpenFile: %u\r", (uint8_t)rslt);
    if(rslt == FR_OK) {
        Uart.Printf("Size: %u\r", SD.File.fsize);
        uint32_t N=0;
        rslt = f_read(&SD.File, Str, 250, (UINT*)&N);
        if(rslt == FR_OK) {
            Uart.Printf("N: %u; Str: %s\r", N, Str);
        }
        else Uart.Printf("ReadFile: %u\r", (uint8_t)rslt);
        f_close(&SD.File);
    }

    uint32_t Count=0;
    iniReadUint32("Sound", "Count", "settings.ini", &Count);
    Uart.Printf("Cnt: %u\r", Count);


    while(1) {
        chThdSleepMilliseconds(1800);
        Lcd.Cls(c);
        for(uint8_t y=0; y<128; y+=8) {
            chThdSleepMilliseconds(999);
            Lcd.Printf(0, y, clBlue, c, "YA=%u", y);
            //Beep(BeepBeep);

        }
        c = (c == clBlack)? clWhite : clBlack;

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

// ================================ Keys =======================================
KeyStatus_t PrevKeyStatus[KEY_COUNT];
static WORKING_AREA(waKeysThread, 32);
static msg_t KeysThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Keys");

    while(1) {
        chThdSleepMilliseconds(54);
        for(uint8_t i=0; i<KEY_COUNT; i++) {
            if((Key[i] == keyPressed) and (PrevKeyStatus[i] == keyReleased)) {
                // Keypress occured
                PrevKeyStatus[i] = keyPressed;
                Beep(ShortBeep);
                Vibro(ShortBrr);
            }
            else if((Key[i] == keyReleased) and (PrevKeyStatus[i] == keyPressed)) {
                // Key release occured
                PrevKeyStatus[i] = keyReleased;
            }
        }
    }
    return 0;
}

// =============================== App class ===================================
void App_t::Init() {
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, KeysThread, NULL);
}
