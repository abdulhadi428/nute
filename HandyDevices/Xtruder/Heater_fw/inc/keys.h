/*
 * keys.h
 *
 *  Created on: 26.11.2011
 *      Author: Kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "kl_lib.h"
#include <inttypes.h>
#include "stm32f10x_gpio.h"

#define KEY_DELAY   99 // ms

struct KeyData_t {
    GPIO_TypeDef *PGpioPort;
    uint16_t PinMask;
    ftVoid_Void EventPressed;
};

// Key events
void Evt_KeyUpPressed(void);
void Evt_KeyDownPressed(void);
void Evt_KeyEnterPressed(void);

const KeyData_t KeyData[] = {
        {GPIOB, GPIO_Pin_5, Evt_KeyUpPressed},      // Up
        {GPIOB, GPIO_Pin_8, Evt_KeyDownPressed},    // Down
        {GPIOB, GPIO_Pin_9, Evt_KeyEnterPressed},   // Enter
};
#define KEY_COUNT   countof(KeyData)

class Keys_t {
private:
    uint16_t Timer;
    bool IsDown[KEY_COUNT];
    bool HwPressed(uint8_t AIndx) { return klGpioIsClearByMsk(KeyData[AIndx].PGpioPort, KeyData[AIndx].PinMask); }
public:
    void Init(void);
    void Task(void);
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
