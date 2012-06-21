/*
 * adc.h
 *
 *  Created on: 27.11.2011
 *      Author: Kreyl
 */

#ifndef ADC_H_
#define ADC_H_

#include <inttypes.h>

#define BATTERY_DISCHARGED_U    2050

#define ADC_AVERAGE_COUNT       16  // Number of times to measure

class Adc_t {
private:
    uint16_t ADCValues[ADC_AVERAGE_COUNT];
public:
    uint16_t GetValue(void);
    void Init(void);
};
//
//class Battery_t : public Adc_t {
//public:
//    bool IsDischarged(void) { return (GetValue() < BATTERY_DISCHARGED_U); }
//};


extern Adc_t Adc;

#endif /* ADC_H_ */
