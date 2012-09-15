/*
 * batery.h
 *
 *  Created on: 15.09.2012
 *      Author: Kreyl
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include <inttypes.h>
#include "kl_lib.h"

/*
4.20v = 100% = ADC 2585
4.03v = 76%  = ADC 2474
3.86v = 52%  = ADC 2373
3.83v = 42%  = ADC 2357
3.79v = 30%  = ADC 2330
3.70v = 11%  = ADC 2277
3.60v = 0%   = ADC 2212
*/

#define BATTERY_ZERO    2212    // 3.6
#define BATTERY_EMPTY   2277    // 3.7
#define BATTERY_HALF    2373    // 3.86

#define ADC_AVERAGE_COUNT   16  // Number of times to measure

typedef enum {bsZero, bsEmpty, bsHalf, bsFull, bsCharging} BatteryState_t;

class Battery_t {
private:
    uint16_t ADCValues[ADC_AVERAGE_COUNT];
    uint32_t Timer;
    BatteryState_t State;
    bool IsCharging()         { return klGpioIsClearByMsk(GPIOA, GPIO_Pin_11); }
    bool IsChargingComplete() { return klGpioIsClearByMsk(GPIOA, GPIO_Pin_12); }
    bool IsExternalPowerOk()  { return klGpioIsClearByMsk(GPIOA, GPIO_Pin_15); }
public:
    void Init(void);
    void Task(void);
};


extern Battery_t Battery;

#endif /* BATTERY_H_ */
