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

App_t App;

// Prototypes


// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 128);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    Color_t c = clBlack;
    while(1) {
        chThdSleepMilliseconds(1800);
        Lcd.Cls(c);
        for(uint8_t y=0; y<120; y+=16) {
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
        chThdSleepMilliseconds(207);
        for(uint8_t i=0; i<KEY_COUNT; i++) {
            if((Key[i] == keyPressed) and (PrevKeyStatus[i] == keyReleased)) {
                // Keypress occured
                PrevKeyStatus[i] = keyPressed;
                Beep(ShortBeep);
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
