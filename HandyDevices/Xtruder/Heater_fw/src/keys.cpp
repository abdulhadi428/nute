/*
 * keys.cpp
 *
 *  Created on: 26.11.2011
 *      Author: Kreyl
 */

#include "keys.h"

Keys_t Keys;

void Keys_t::Init() {
    for(uint8_t i=0; i<KEY_COUNT; i++) {
        klGpioSetupByMsk(KeyData[i].PGpioPort, KeyData[i].PinMask, GPIO_Mode_IPU);
        IsDown[i] = false;
    }
}

void Keys_t::Task() {
    if (!Delay.Elapsed(&Timer, KEY_DELAY)) return;
    for(uint8_t i=0; i<KEY_COUNT; i++) {
        if(HwPressed(i) and !IsDown[i]) {
            IsDown[i] = true;
            if(KeyData[i].EventPressed != 0) KeyData[i].EventPressed();
        }
        else if(!HwPressed(i) and IsDown[i]) {
            IsDown[i] = false;
        }
    } // for
}
