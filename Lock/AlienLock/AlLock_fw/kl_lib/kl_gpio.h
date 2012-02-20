/*
 * kl_gpio.h
 *
 *  Created on: 18.02.2012
 *      Author: kreyl
 */

#ifndef KL_GPIO_H_
#define KL_GPIO_H_

#include <inttypes.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"

//enum GpioPort_t {GpioA, GpioB};
//GPIO_TypeDefenum PinMode_t {InputAnalog, InputFloating, InputPullUp, InputPullDown, OutputOpenDrain, OutputPushPull, AlternatePushPull, AlternateOpenDrain};



class klPin_t {
protected:
    GPIO_TypeDef* IGPIO;
    uint16_t IPinMask, IPinNumber;
public:
    klPin_t(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode);
    bool operator =(const bool AValue) {
        if (AValue) IGPIO->BSRR = IPinMask;
        else IGPIO->BRR = IPinMask;
        return true;
    }
    operator bool() { return ((bool)(IGPIO->IDR & IPinMask)); }
    void Toggle(void) { IGPIO->ODR ^= IPinMask; }
};

class klPinIrq_t : klPin_t {
private:
    uint32_t IChannel;
public:
    klPinIrq_t(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode) : klPin_t(PGpioPort, APinNumber, AMode) { }
    // Interrupt
    void IrqSetup(EXTITrigger_TypeDef ATriggerType);
    void IrqEnable(void);
    void IrqDisable(void);
};

#endif /* KL_GPIO_H_ */
