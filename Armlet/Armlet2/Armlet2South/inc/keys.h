/*
 * keys.h
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "hal.h"
#include "kl_lib_f0.h"

// Keys ports and pins
struct KeyPin_t {
    GPIO_TypeDef *PGpioPort;
    uint16_t PinNumber;
};

const KeyPin_t KeyPin[] = {
        {GPIOB, 5},     // 1
        {GPIOB, 4},     // 2
        {GPIOB, 3},     // 3
        {GPIOA, 15},    // 4
        {GPIOA, 12},    // 5
        {GPIOA, 8},     // 6
        {GPIOA, 4},     // 7
        {GPIOA, 5},     // 8
        {GPIOA, 6},     // 9
};
#define KEYS_CNT    countof(KeyPin)

#define KEYS_POLL_PERIOD_MS     45

// Key status
#define KEY_PRESSED     1
#define KEY_RELEASED    0

class Keys_t {
public:
    uint8_t Status[KEYS_CNT];
    void Init();
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
