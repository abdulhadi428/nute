/*
 * sensor.h
 *
 *  Created on: 19.02.2012
 *      Author: kreyl
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "kl_util.h"
#include "kl_gpio.h"
#include "stm32f10x_exti.h"

#define ON_TIME     4005    // ms to stay online after trigger

class Sensor_t {
private:
    uint32_t ITimer;
    bool IsTriggered, IsHandled;
public:
    Sensor_t(void);
    void Task(void);
    void IrqHandler(void);
    ftVoid_Void EvtTrigger, EvtNoTrigger;
};

// Interrupt
extern "C" {
void EXTI15_10_IRQHandler(void);
}

extern Sensor_t Sensor;

#endif /* SENSOR_H_ */
