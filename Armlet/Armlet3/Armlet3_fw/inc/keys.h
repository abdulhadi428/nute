/*
 * keys.h
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "hal.h"
#include "kl_lib_f2xx.h"

// Keys ports and pins
#define KEY_GPIO    GPIOD
const uint16_t KeyPin[] = {
        0,  // 1 = A
        1,  // 2 = B
        3,  // 3 = C
        4,  // 4
        7,  // 5
        8,  // 6
        9,  // 7 = X
        10, // 8 = Y
        11, // 9 = Z
};

// Key masks for event registering
#define KEY_A       0x001
#define KEY_B       0x002
#define KEY_C       0x004
#define KEY_L    0x008
#define KEY_M      0x010
#define KEY_R   0x020
#define KEY_X       0x040
#define KEY_Y       0x080
#define KEY_Z       0x100

#define KEYS_CNT    9
#define KEYS_POLL_PERIOD_MS     72
// Key status
#define KEY_PRESSED     1
#define KEY_RELEASED    0

extern EventSource EvtSrcKey;
extern uint8_t KeyStatus[KEYS_CNT];

void KeysInit();

#endif /* KEYS_H_ */
