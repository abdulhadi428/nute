/*
 * adc.h
 *
 *  Created on: 27.11.2011
 *      Author: Kreyl
 */

#ifndef ADC_H_
#define ADC_H_

#include <inttypes.h>
#include "kl_lib.h"

#define BATTERY_EMPTY   1400    // 1150 mV
#define BATTERY_HALF    1600

#define ADC_AVERAGE_COUNT   16  // Number of times to measure

typedef enum {bsEmpty, bsHalf, bsFull} BatteryState_t;

class Adc_t {
private:
    uint16_t ADCValues[ADC_AVERAGE_COUNT];
protected:
    uint32_t IValue;
    uint32_t Timer;
    bool MeasurementCompleted() {
        if(DMA_GetFlagStatus(DMA1_FLAG_TC1)) {
            DMA_ClearFlag(DMA1_FLAG_TC1);
            return true;
        }
        else return false;
    }
public:
    void Init();
    void Task();
};

extern Adc_t Adc;

extern uint16_t R[4];

#endif /* ADC_H_ */
