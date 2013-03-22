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
        0,  // 1
        1,  // 2
        3,  // 3
        4,  // 4
        7,  // 5
        8,  // 6
        9,  // 7
        10, // 8
        11, // 9
};
#define KEYS_CNT    countof(KeyPin)
#define KEYS_POLL_PERIOD_MS     72
// Key status
#define KEY_PRESSED     1
#define KEY_RELEASED    0

void KeysInit();

#endif /* KEYS_H_ */
