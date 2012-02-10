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
#include "delay_util.h"

#define KEYS_RATE   45  // ms
#define KEY_STAR    10
#define KEY_HASH    11
#define KEY_NONE    12

class Keys_t {
private:
    uint32_t ITimer;
public:
    uint8_t Kbd[2]; // Pressed key
    void Task(void);
    void Init(void);
    ftVoid_Void EvtKbd[2];
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
