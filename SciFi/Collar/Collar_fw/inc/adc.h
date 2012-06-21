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
    uint32_t Timer;
    uint16_t ADCValues[ADC_AVERAGE_COUNT];
    void Measure(void);
protected:
    uint16_t IValue;
public:
    void Init(void);
    void Task(void);
};

class Battery_t : public Adc_t {
public:
    bool IsDischarged(void) { return (IValue < BATTERY_DISCHARGED_U); }
};


extern Battery_t Battery;

#endif /* ADC_H_ */
