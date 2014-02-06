/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"

Keys_t Keys;

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
__attribute__((noreturn))
static void KeysThread(void *arg) {
    chRegSetThreadName("Keys");
    while(true) Keys.ITask();
}

void Keys_t::ITask() {
    chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
    // Check keys
    for(uint8_t i=0; i<KEYS_CNT; i++) {
        bool PressedNow = PinIsSet(KeyData[i].PGpio, KeyData[i].Pin);
        // Check if just pressed
        if(PressedNow and !IsPressed[i]) {
            chSysLock();
            IsPressed[i] = true;
            chEvtSignalI(PThd, KeyData[i].EvtMskPress);
            chSysUnlock();
        }
        // Check if just released
        else if(!PressedNow and IsPressed[i]) {
            IsPressed[i] = false;
        }
    } // for
}

// ==== Keys methods ====
void Keys_t::Init(Thread *AppThd) {
    PThd = AppThd;
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KeyData[i].PGpio, KeyData[i].Pin, pudPullDown);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, (tfunc_t)KeysThread, NULL);
}
