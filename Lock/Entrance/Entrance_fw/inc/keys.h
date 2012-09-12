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
#include "interface.h"

#define KEY_DELAY                   99      // ms
#define KEY_BEFORE_REPEAT_DELAY     999     // ms
#define KEY_REPEAT_DELAY            162     // ms
#define KEY_LONGPRESS_DELAY         4005    // ms

typedef void(*ftVoidBool)(bool);

// Structure defining key properties
struct KeyData_t {
    GPIO_TypeDef *PGpioPort;
    uint16_t PinMask;
    ftVoidBool EventPressed;
};

// Set of keys.
// ==== Setup here Port, Pin and KeyPress event handler ====
const KeyData_t KeyData[] = {
        {GPIOB, GPIO_Pin_8,  KeyUp},      // Up
        {GPIOB, GPIO_Pin_9,  KeyDown},    // Down
        {GPIOB, GPIO_Pin_10, KeyEnter},   // Enter
};
#define KEY_COUNT   countof(KeyData)

struct Key_t {
    bool IsDown, IsRepeating, IsLongPress;
};

class Keys_t {
private:
    uint32_t Timer, RepeatTimer, LongPressTimer;
    Key_t Key[KEY_COUNT];
    bool HwPressed(uint8_t AIndx) { return klGpioIsClearByMsk(KeyData[AIndx].PGpioPort, KeyData[AIndx].PinMask); }
public:
    void Init(void);
    void Task(void);
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
