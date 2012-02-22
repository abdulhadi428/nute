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

// Static methods only
class klGpio {
public:
    // Static methods
    static void SetupByMsk(GPIO_TypeDef *PGpioPort, uint16_t APinMask, GPIOMode_TypeDef AMode);
    static void SetupByN(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode) { SetupByMsk(PGpioPort, (uint16_t)(1<<APinNumber), AMode); }
    static void SetByN(GPIO_TypeDef *PGpioPort, uint16_t APinNumber)   { PGpioPort->BSRR = (uint16_t)(1<<APinNumber); }
    static void ClearByMsk(GPIO_TypeDef *PGpioPort, uint16_t APinMask) { PGpioPort->BRR = APinMask; }
    static bool IsSetByMsk  (GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { return (bool)(PGpioPort->IDR & APinMask); }
    static bool IsClearByMsk(GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { return !((bool)(PGpioPort->IDR & APinMask)); }
    static bool IsClearByN  (GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { return IsClearByMsk(PGpioPort, (uint16_t)(1<<APinNumber)); }
};

// Class for single pin manipulation
class klPin_t {
protected:
    GPIO_TypeDef* IGPIO;
    uint16_t IPinMask, IPinNumber;
public:
    klPin_t(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode) { Init(PGpioPort, APinNumber, AMode); }
    klPin_t(void) { }
    bool operator =(const bool AValue) {
        if (AValue) Set();
        else Clear();
        return true;
    }
    operator bool() { return ((bool)(IGPIO->IDR & IPinMask)); }
    void Init(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode);
    void Set(void)    { IGPIO->BSRR = IPinMask; }
    void Clear(void)  { IGPIO->BRR  = IPinMask; }
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
