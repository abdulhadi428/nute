/*
 * keys.h
 *
 *  Created on: 08.02.2012
 *      Author: kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include <inttypes.h>
#include "kl_util.h"

#define KEYS_RATE   99  // ms
#define KEY_NONE    0xFF

// For event function: uint8_t AKbdSide, uint8_t AKey
typedef void(*ftVoidU8)(uint8_t);

class Keys_t {
private:
    uint32_t ITimer;
public:
    uint8_t PressedKey;
    void Task(void);
    void Init(void);
    ftVoidU8 EvtKbd;
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
