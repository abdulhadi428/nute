/*
 * flash.h
 *
 *  Created on: 25 џэт. 2014 у.
 *      Author: Kreyl
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "kl_lib_f100.h"

#define FLASH_GPIO          GPIOB
#define FLASH_PIN           1
#define FLASH_DURATION_MS   36

class Flash_t {
private:
    VirtualTimer ITmr;
    static void IOff() { PinClear(FLASH_GPIO, FLASH_PIN); }
public:
    void Init() {
        PinSetupOut(FLASH_GPIO, FLASH_PIN, omOpenDrain);
        PinClear(FLASH_GPIO, FLASH_PIN);
    }
    void Fire() {
        PinSet(FLASH_GPIO, FLASH_PIN);
        chVTSetI(&ITmr, MS2ST(FLASH_DURATION_MS), (vtfunc_t)&IOff, nullptr);
    }
};

#endif /* FLASH_H_ */
