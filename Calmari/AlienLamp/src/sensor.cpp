/*
 * sensor.cpp
 *
 *  Created on: 19.02.2012
 *      Author: kreyl
 */

#include "sensor.h"
#include "delay_util.h"

Sensor_t Sensor;

Sensor_t::Sensor_t() {
    // Setup pin
    klPinIrq_t IPin(GPIOB, 12, GPIO_Mode_IPU);
    IPin.IrqSetup(EXTI_Trigger_Rising_Falling);
    IPin.IrqEnable();
}

void Sensor_t::Task() {
    if (IsTriggered) {
        if (!IsHandled) {
            if (EvtTrigger != 0) EvtTrigger();
            IsHandled = true;
        }
        if (Delay.Elapsed(&ITimer, ON_TIME)) {
            // Time to shutdown
            IsTriggered = false;
            IsHandled = false;
            if (EvtNoTrigger != 0) EvtNoTrigger();
        }
    }
}


void Sensor_t::IrqHandler() {
    IsTriggered = true;
    Delay.Reset(&ITimer);
}

// Interrupt
void EXTI15_10_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line12) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line12); // Clear the EXTI line pending bit
        Sensor.IrqHandler();
    }
}
