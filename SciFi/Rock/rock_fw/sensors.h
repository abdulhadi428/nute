/*
 * File:   sensors.h
 * Author: Kreyl
 *
 * Created on June 19, 2011, 11:59 AM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
//#include "misc.h"

#include "acc_mma.h"
#include "delay_util.h"

// Constants
#define ADC_REQUEST_PERIOD      500  // ms


class Sns_t {
private:
    uint32_t Timer;
    void BatteryInit(void);
public:
    union {
        uint16_t ADCValues[2];
        struct {
            uint16_t BatteryADC, LumiADC;
        };
    };
    // General
    void Init(void);
    void Task(void);
};

extern Sns_t ESns;

// Declare Timer IRQ. Use externC to make it visible from asm file.
//extern "C" {
//void ADC1_2_IRQHandler(void);
//}

#endif	/* SENSORS_H */

