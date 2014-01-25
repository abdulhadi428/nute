/*
 * flash.h
 *
 *  Created on: 25 џэт. 2014 у.
 *      Author: Kreyl
 */

#ifndef LED_H_
#define LED_H_

#include "kl_lib_f100.h"

#define LED_GPIO          GPIOB
#define LED_PIN           1
#define LED_DURATION_MS   36

class Led_t {
private:
    VirtualTimer ITmr;
    static void IOff() { PinClear(LED_GPIO, LED_PIN); }
public:
    void Init() {
        PinSetupOut(LED_GPIO, LED_PIN, omPushPull);
        PinClear(LED_GPIO, LED_PIN);
    }
    void Fire() {
        PinSet(LED_GPIO, LED_PIN);
        chVTSetI(&ITmr, MS2ST(LED_DURATION_MS), (vtfunc_t)&IOff, nullptr);
    }
};

#endif /* LED_H_ */
