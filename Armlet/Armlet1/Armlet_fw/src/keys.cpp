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
        Key[i].IsDown = false;
    }
}

void Keys_t::Task() {
    if (!Delay.Elapsed(&Timer, KEY_DELAY)) return;
    for(uint8_t i=0; i<KEY_COUNT; i++) {
        if(HwPressed(i) and !Key[i].IsDown) {       // Just pressed
            Key[i].IsDown = true;
            KeyData[i].EventPressed(false);
            Delay.Reset(&LongPressTimer);
            Delay.Reset(&RepeatTimer);
            Key[i].IsLongPress = false;
            Key[i].IsRepeating = false;
        }
        else if(!HwPressed(i) and Key[i].IsDown) {  // Just depressed
            Key[i].IsDown = false;
        }
        else if((HwPressed(i) and Key[i].IsDown)) { // Still pressed
            // Check if long press
            if(!Key[i].IsLongPress)
                if(Delay.Elapsed(&LongPressTimer, KEY_LONGPRESS_DELAY))
                    Key[i].IsLongPress = true;
            // Check if repeat
            if(!Key[i].IsRepeating) {
                if(Delay.Elapsed(&RepeatTimer, KEY_BEFORE_REPEAT_DELAY)) {
                    Key[i].IsRepeating = true;
                    KeyData[i].EventPressed(Key[i].IsLongPress);
                }
            }
            else {
                if(Delay.Elapsed(&RepeatTimer, KEY_REPEAT_DELAY))
                    KeyData[i].EventPressed(Key[i].IsLongPress);
            }
        }
    } // for
}
