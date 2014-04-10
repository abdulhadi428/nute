/*
 * flash.h
 *
 *  Created on: 25 џэт. 2014 у.
 *      Author: Kreyl
 */

#ifndef LED_H_
#define LED_H_

#include "kl_lib_f100.h"

#define LED_FIRE_GPIO   GPIOA
#define LED_FIRE_PIN    3
#define LED_DAC_GPIO    GPIOA
#define LED_DAC_PIN     4
#define LED_DAC_VALUE   2607    // 1000 mA


#define LED_DURATION_MS 54

class Led_t {
private:
    VirtualTimer ITmr;
    static void IOff() { PinSet(LED_FIRE_GPIO, LED_FIRE_PIN); }
public:
    void Init() {
        // Fire pin
        PinSetupOut(LED_FIRE_GPIO, LED_FIRE_PIN, omOpenDrain);
        PinSet(LED_FIRE_GPIO, LED_FIRE_PIN);
        // Current regulation pin and DAC
        PinSetupAnalog(LED_DAC_GPIO, LED_DAC_PIN);
        rccEnableAPB1(RCC_APB1ENR_DACEN, FALSE);
        DAC->CR = DAC_CR_EN1;   // Do not disable buffer, otherwise lower values on output
        DAC->DHR12R1 = LED_DAC_VALUE;
    }
    void Fire() {
        PinClear(LED_FIRE_GPIO, LED_FIRE_PIN);
        chVTSetI(&ITmr, MS2ST(LED_DURATION_MS), (vtfunc_t)&IOff, nullptr);
    }
};

#endif /* LED_H_ */
