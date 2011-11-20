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
#include "delay_util.h"
#include "arm_common.h"

class Sns_t {
private:
    uint32_t Timer;
    bool WasDetected;
    bool Detected(void) { return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3); }
public:
    ftVoid_Void EvtDetected;
    void Init(void);
    void Task(void);
};


// =============================== Global ======================================
extern Sns_t Sensor;

#endif	/* SENSORS_H */

