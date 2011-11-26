/*
 * keys.h
 *
 *  Created on: 26.11.2011
 *      Author: Kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "kl_util.h"
#include <inttypes.h>
#include "stm32f10x_gpio.h"

#define KEY_COUNT   2
#define KEY_DELAY   108 // ms

const uint16_t KeyPin[KEY_COUNT] = {GPIO_Pin_0, GPIO_Pin_1};

class Keys_t {
private:
    uint32_t Timer;
    bool KeyIsDown[KEY_COUNT];
public:
    void Init(void);
    void Task(void);
    // Events
    ftVoid_Void EvtKeyPress[KEY_COUNT];
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
