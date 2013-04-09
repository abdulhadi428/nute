/*
 * power.h
 *
 *  Created on: 09.04.2013
 *      Author: kreyl
 */

#ifndef POWER_H_
#define POWER_H_

#include "kl_lib_f2xx.h"

#define PWR_EXTERNAL_PIN    9
#define PWR_EXTERNAL_GPIO   GPIOA
#define PWR_CHARGING_PIN    3
#define PWR_CHARGING_GPIO   GPIOC
#define PWR_BATTERY_PIN     0
#define PWR_BATTERY_GPIO    GPIOC

#define PWR_MEASURE_EVERY_MS    999

#define ADC_CHNL   10
//#define

class Pwr_t {
private:

public:
    bool ExternalPwrOn() { return  PinIsSet(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN); }
    bool IsCharging()    { return !PinIsSet(PWR_CHARGING_GPIO, PWR_CHARGING_PIN); }
    uint16_t Voltage_mV;
    uint8_t RemainingPercent;
    EventSource EvtSrcPwrChange;
    void Init();
};
extern Pwr_t Power;


#endif /* POWER_H_ */
