/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"

//Keys_t Keys;
static uint8_t PrevStatus[KEYS_CNT];
EventSource EvtSrcKey;

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
static msg_t KeysThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Keys");

    uint8_t i, CurrentStatus;
    while(1) {
        chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
        // Check keys
        for(i=0; i<KEYS_CNT; i++) {
            CurrentStatus = PinIsSet(KEY_GPIO, KeyPin[i])? KEY_RELEASED : KEY_PRESSED;
            if(PrevStatus[i] != CurrentStatus) {
                PrevStatus[i] = CurrentStatus;
                Uart.Printf("Key %u\r", i);
                chEvtBroadcast(&EvtSrcKey);
            }
        }
    }
    return 0;
}

// ==== Keys methods ====
void KeysInit() {
    chEvtInit(&EvtSrcKey);
    for(uint8_t i=0; i<KEYS_CNT; i++) {
        PrevStatus[i] = KEY_RELEASED;
        PinSetupIn(KEY_GPIO, KeyPin[i], pudPullUp);
    }
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, KeysThread, NULL);
}
