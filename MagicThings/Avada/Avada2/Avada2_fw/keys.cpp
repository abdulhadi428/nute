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
        bool PressedNow = !PinIsSet(KeyData[i].PGpio, KeyData[i].Pin);
        // Check if just pressed
        if(PressedNow and !Key[i].IsPressed) {
            chSysLock();
            Key[i].IsPressed = true;
            Key[i].IsLongPress = false;
            Key[i].IsRepeating = false;
            chEvtSignalI(PThd, KeyData[i].EvtMskPress);
            // Reset timers
            RepeatTimer = chTimeNow();
            LongPressTimer = chTimeNow();
            chSysUnlock();
        }
        // Check if just released
        else if(!PressedNow and Key[i].IsPressed) {
            Key[i].IsPressed = false;
            if(!Key[i].IsLongPress) {
                chSysLock();
                chEvtSignalI(PThd, KeyData[i].EvtMskRelease);
                chSysUnlock();
            }
        }
        // Check if still pressed
        else if(PressedNow and Key[i].IsPressed) {
            // Check if long press
            if(!Key[i].IsLongPress) {
                if(TimeElapsed(&LongPressTimer, KEY_LONGPRESS_DELAY_MS)) {
                    Key[i].IsLongPress = true;
                    chSysLock();
                    chEvtSignalI(PThd, KeyData[i].EvtMskLongPress);
                    chSysUnlock();
                }
            }
            // Check if repeat
            if(!Key[i].IsRepeating) {
                if(TimeElapsed(&RepeatTimer, KEYS_KEY_BEFORE_REPEAT_DELAY_MS)) {
                    Key[i].IsRepeating = true;
                    chSysLock();
                    chEvtSignalI(PThd, KeyData[i].EvtMskRepeat);
                    chSysUnlock();
                }
            }
            else {
                if(TimeElapsed(&RepeatTimer, KEY_REPEAT_PERIOD_MS)) {
                    chSysLock();
                    chEvtSignalI(PThd, KeyData[i].EvtMskRepeat);
                    chSysUnlock();
                }
            }
        } // if still pressed
    } // for
}

// ==== Keys methods ====
void Keys_t::Init(Thread *AppThd) {
    PThd = AppThd;
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KeyData[i].PGpio, KeyData[i].Pin, pudPullUp);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, (tfunc_t)KeysThread, NULL);
}
