/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "buttons.h"
#include "ch.h"

Keys_t Keys;

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
static msg_t KeysThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Keys");

    uint8_t i;
    KeyState_t Now;
    bool FHasChanged;
    while(1) {
        chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
        // Check keys
        FHasChanged = false;
        for(i=0; i<KEYS_CNT; i++) {
            Now = PinIsSet(Key[i].Gpio, Key[i].Pin)? ksReleased : ksPressed;
            if(Keys.Status[i].State != Now) {
                Keys.Status[i].State = Now;
                Keys.Status[i].HasChanged = true;
//                Uart.Printf("Key %u\r", i);
                FHasChanged = true;
            }
        } // for
        if(FHasChanged) chEvtBroadcast(&Keys.EvtSrc);
    }
    return 0;
}

// ==== Keys methods ====
void Keys_t::Init() {
    chEvtInit(&Keys.EvtSrc);
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(Key[i].Gpio, Key[i].Pin, pudPullUp);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, KeysThread, NULL);
}

void Keys_t::Shutdown() {
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupAnalog(Key[i].Gpio, Key[i].Pin);
}
