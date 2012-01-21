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

#define KEY_DELAY   108 // ms

class Keys_t {
private:
    uint32_t Timer;
    bool Key1IsDown, Key2IsDown;
    bool Key1HwPressed(void) { return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET); }
    bool Key2HwPressed(void) { return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET); }
public:
    void Init(void);
    void Task(void);
    // Events
    ftVoid_Void EvtKey1Press, EvtKey2Press;
    ftVoid_Void EvtKeyPressBoth;
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
