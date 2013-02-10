/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"
#include "SouthbridgeTxRx.h"

Keys_t Keys;

const SBCmd_t KeysCmd = {
        STN_KEY_STATUS,
        Keys.Status,
        KEYS_CNT
};

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
static msg_t KeysThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Keys");

    uint8_t i, CurrentStatus;
    bool StatusChanged = false;
    while(1) {
        chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
        // Check keys
        for(i=0; i<KEYS_CNT; i++) {
            CurrentStatus = PinIsSet(KeyPin[i].PGpioPort, KeyPin[i].PinNumber)? KEY_RELEASED : KEY_PRESSED;
            if(Keys.Status[i] != CurrentStatus) {
                StatusChanged = true;
                Keys.Status[i] = CurrentStatus;
            }
        }
        // Send data if needed
        if(StatusChanged) {
            Transmitter.AddCmd((SBCmd_t*)&KeysCmd);
            StatusChanged = false;
        }
    }
    return 0;
}

// ==== Keys methods ====
void Keys_t::Init() {
    for(uint8_t i=0; i<KEYS_CNT; i++) {
        Status[i] = KEY_RELEASED;
        PinSetupIn(KeyPin[i].PGpioPort, KeyPin[i].PinNumber, pudPullUp);
    }
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, KeysThread, NULL);
}
