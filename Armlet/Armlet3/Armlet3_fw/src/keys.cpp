/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"

EventSource EvtSrcKey;
uint8_t KeyStatus[KEYS_CNT];

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
static msg_t KeysThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Keys");

    uint8_t i, CurrentStatus;
    bool HasChanged;
    while(1) {
        chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
        // Check keys
        HasChanged = false;
        for(i=0; i<KEYS_CNT; i++) {
            CurrentStatus = PinIsSet(KEY_GPIO, KeyPin[i])? KEY_RELEASED : KEY_PRESSED;
            if(KeyStatus[i] != CurrentStatus) {
                KeyStatus[i] = CurrentStatus;
//                Uart.Printf("Key %u\r", i);
                HasChanged = true;
            }
        } // for

        if(HasChanged) chEvtBroadcast(&EvtSrcKey);
    }
    return 0;
}

// ==== Keys methods ====
void KeysInit() {
    chEvtInit(&EvtSrcKey);
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KEY_GPIO, KeyPin[i], pudPullUp);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, KeysThread, NULL);
}

void KeysRegisterEvt(EventListener *PEvtLstnr, uint8_t EvtMask) {
    chEvtRegisterMask(&EvtSrcKey, PEvtLstnr, EvtMask);
}
