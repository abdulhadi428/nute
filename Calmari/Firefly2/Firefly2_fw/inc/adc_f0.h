/*
 * adc_f0.h
 *
 *  Created on: 16.02.2013
 *      Author: kreyl
 */

#ifndef ADC_F0_H_
#define ADC_F0_H_

#include "stm32f0xx.h"

#define ADC_SAMPLE_POWER    3
#define ADC_BUF_SZ          (1<<ADC_SAMPLE_POWER)

class Adc_t {
private:
    uint16_t Buf[ADC_BUF_SZ];
public:
    void StartConversion() { ADC1->CR |= ADC_CR_ADSTART; }
    void Init();
    void ChannelConfig(uint32_t Channel, uint32_t SampleTime);
    void ContModeEnable() { ADC1->CFGR1 = ADC_CFGR1_CONT; }  // Enable continuos conversion
    void Enable() {
        ADC1->CR = ADC_CR_ADEN;                 // Enable ADC
        while(!(ADC1->ISR & ADC_ISR_ADRDY));    // wait until ready
    }
    void Calibrate();
};

// ADC sampling_times
#define ADC_SampleTime_1_5Cycles                     ((uint32_t)0x00000000)
#define ADC_SampleTime_7_5Cycles                     ((uint32_t)0x00000001)
#define ADC_SampleTime_13_5Cycles                    ((uint32_t)0x00000002)
#define ADC_SampleTime_28_5Cycles                    ((uint32_t)0x00000003)
#define ADC_SampleTime_41_5Cycles                    ((uint32_t)0x00000004)
#define ADC_SampleTime_55_5Cycles                    ((uint32_t)0x00000005)
#define ADC_SampleTime_71_5Cycles                    ((uint32_t)0x00000006)
#define ADC_SampleTime_239_5Cycles                   ((uint32_t)0x00000007)

// ADC_channels
#define ADC_Channel_0                              ADC_CHSELR_CHSEL0
#define ADC_Channel_1                              ADC_CHSELR_CHSEL1
#define ADC_Channel_2                              ADC_CHSELR_CHSEL2
#define ADC_Channel_3                              ADC_CHSELR_CHSEL3
#define ADC_Channel_4                              ADC_CHSELR_CHSEL4
#define ADC_Channel_5                              ADC_CHSELR_CHSEL5
#define ADC_Channel_6                              ADC_CHSELR_CHSEL6
#define ADC_Channel_7                              ADC_CHSELR_CHSEL7
#define ADC_Channel_8                              ADC_CHSELR_CHSEL8
#define ADC_Channel_9                              ADC_CHSELR_CHSEL9
#define ADC_Channel_10                             ADC_CHSELR_CHSEL10
#define ADC_Channel_11                             ADC_CHSELR_CHSEL11
#define ADC_Channel_12                             ADC_CHSELR_CHSEL12
#define ADC_Channel_13                             ADC_CHSELR_CHSEL13
#define ADC_Channel_14                             ADC_CHSELR_CHSEL14
#define ADC_Channel_15                             ADC_CHSELR_CHSEL15
#define ADC_Channel_16                             ADC_CHSELR_CHSEL16
#define ADC_Channel_17                             ADC_CHSELR_CHSEL17
#define ADC_Channel_18                             ADC_CHSELR_CHSEL18

#define ADC_Channel_TempSensor                     ((uint32_t)ADC_Channel_16)
#define ADC_Channel_Vrefint                        ((uint32_t)ADC_Channel_17)
#define ADC_Channel_Vbat                           ((uint32_t)ADC_Channel_18)



#endif /* ADC_F0_H_ */
