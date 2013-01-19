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

App_t App;

// Prototypes

// =============================== App Thread ==================================
static WORKING_AREA(waLcdThread, 128);
static msg_t LcdThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    //Color_t c = clRed;
    while(1) {
        chThdSleepMilliseconds(1800);
        Lcd.Cls(clBlack);
        for(uint8_t y=0; y<120; y+=16) {
            chThdSleepMilliseconds(999);
            Lcd.Printf(0, y, clWhite, clBlack, "YA=%u", y);
        }
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

// =============================== App class ===================================
void App_t::Init() {
    chThdCreateStatic(waLcdThread, sizeof(waLcdThread), NORMALPRIO, LcdThread, NULL);
}
