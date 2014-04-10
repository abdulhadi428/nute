/*
 * keys.h
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "hal.h"
#include "kl_lib_f100.h"
#include "evt_mask.h"

// Keys data
struct KeyData_t {
    GPIO_TypeDef *PGpio;
    uint16_t Pin;
    eventmask_t EvtMskPress, EvtMskRelease, EvtMskRepeat, EvtMskLongPress;
};

const KeyData_t KeyData[] = {
        {GPIOA, 0, EVTMSK_KEY_FIRE, EVTMSK_NOTHING,   EVTMSK_NOTHING, EVTMSK_NOTHING},
};
#define KEYS_CNT    countof(KeyData)

// Timings
#define KEYS_POLL_PERIOD_MS             72
#define KEYS_KEY_BEFORE_REPEAT_DELAY_MS 999
#define KEY_REPEAT_PERIOD_MS            162
#define KEY_LONGPRESS_DELAY_MS          3006

// Key status
struct Key_t {
    bool IsPressed, IsRepeating, IsLongPress;
};

class Keys_t {
private:
    Thread *PThd;
    Key_t Key[KEYS_CNT];
    systime_t RepeatTimer, LongPressTimer;
public:
    void Init(Thread *AppThd);
    void Shutdown() { for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupAnalog(KeyData[i].PGpio, KeyData[i].Pin); }
    // Inner use
    void ITask();
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
