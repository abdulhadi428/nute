/*
 * pwr.h
 *
 *  Created on: Jun 12, 2013
 *      Author: g.kruglov
 */

#ifndef PWR_H_
#define PWR_H_

#include "kl_lib_f100.h"

#define CHARGE_GPIO     GPIOA
#define CHARGE_PIN      12
#define EXT_PWR_GPIO    GPIOA
#define EXT_PWR_PIN     0

class Pwr_t {
public:
    void Init() {
        PinSetupIn(EXT_PWR_GPIO, EXT_PWR_PIN, pudPullDown);
        PinSetupIn(CHARGE_GPIO, CHARGE_PIN, pudPullUp);
    }
    bool ExtPwrOn() { return PinIsSet(EXT_PWR_GPIO, EXT_PWR_PIN); }
    bool Charging() { return !PinIsSet(CHARGE_GPIO, CHARGE_PIN); }
};


#endif /* PWR_H_ */
