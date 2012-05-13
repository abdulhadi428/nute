/*
 * kl_lib.h
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

/*
 * GPIO_Mode_AIN, GPIO_Mode_IN_FLOATING, GPIO_Mode_IPD, GPIO_Mode_IPU,
 * GPIO_Mode_Out_OD, GPIO_Mode_Out_PP, GPIO_Mode_AF_OD, GPIO_Mode_AF_PP
 */

// ==== Single pin manipulations ====
// Set/clear
static inline void klGpioSetByN     (GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { PGpioPort->BSRR = (uint16_t)(1<<APinNumber); }
static inline void klGpioSetByMsk   (GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { PGpioPort->BSRR = APinMask; }
static inline void klGpioClearByN   (GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { PGpioPort->BRR  = (uint16_t)(1<<APinNumber); }
static inline void klGpioClearByMsk (GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { PGpioPort->BRR  = APinMask; }
static inline void klGpioToggleByN  (GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { PGpioPort->ODR ^= (uint16_t)(1<<APinNumber); }
static inline void klGpioToggleByMsk(GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { PGpioPort->ODR ^= APinMask; }
// Check state
static inline bool klGpioIsSetByMsk  (GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { return (bool)(PGpioPort->IDR & APinMask); }
static inline bool klGpioIsSetByN    (GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { return klGpioIsSetByMsk(PGpioPort, (uint16_t)(1<<APinNumber)); }
static inline bool klGpioIsClearByMsk(GPIO_TypeDef *PGpioPort, uint16_t APinMask)   { return !((bool)(PGpioPort->IDR & APinMask)); }
static inline bool klGpioIsClearByN  (GPIO_TypeDef *PGpioPort, uint16_t APinNumber) { return klGpioIsClearByMsk(PGpioPort, (uint16_t)(1<<APinNumber)); }
// Setup
void klGpioSetupByMsk  (GPIO_TypeDef *PGpioPort, uint16_t APinMask, GPIOMode_TypeDef AMode);
static inline void klGpioSetupByN (GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode) { klGpioSetupByMsk(PGpioPort, (uint16_t)(1<<APinNumber), AMode); }
// Disable JTAG, leaving SWD
static inline void klJtagDisable(void) {
    bool AfioWasEnabled = (RCC->APB2ENR & RCC_APB2ENR_AFIOEN);
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;     // Enable AFIO
    uint32_t tmp = AFIO->MAPR;
    tmp &= ~0x07000000;
    tmp |= 0x02000000;
    AFIO->MAPR = tmp;
    if (!AfioWasEnabled) RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;
}

// ==== Class for single pin manipulation ====
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
    operator bool() { return klGpioIsSetByMsk(IGPIO, IPinMask); }
    void Init(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode);
    void Set(void)    { klGpioSetByMsk   (IGPIO, IPinMask); }
    void Clear(void)  { klGpioClearByMsk (IGPIO, IPinMask); }
    void Toggle(void) { klGpioToggleByMsk(IGPIO, IPinMask); }
};

class klPinIrq_t : public klPin_t {
private:
    uint32_t IChannel;
public:
    // Interrupt
    void IrqSetup(EXTITrigger_TypeDef ATriggerType);
    void IrqEnable(void);
    void IrqDisable(void);
};

// ==== Timer ====
#define TIM_FREQ_MAX    0xFFFFFFFF
class klTimer_t {
protected:
    TIM_TypeDef* ITimer;
public:
    void Init(TIM_TypeDef* PTimer, uint16_t ATopValue, uint32_t AFreqHz);
    void Enable(void)  { ITimer->CR1 |= TIM_CR1_CEN; }
    void Disable(void) { ITimer->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN)); }
};

/*
 * TIM_OCPolarity_High, TIM_OCPolarity_Low
 */
class klPwmChannel_t : klTimer_t {
private:
    uint8_t INumber;
public:
    void Init(TIM_TypeDef* PTimer, uint16_t ATopValue, uint32_t AFreqHz, uint8_t ANumber, uint16_t APolarity);
    void Enable(void);
    void Disable(void);
    void DisableTimer(void) { klTimer_t::Disable(); }
    void Set(uint16_t AValue);
};


// =================================== Delay ===================================
class Delay_t {
public:
    uint32_t TickCounter;
    void Init(void);
    // Simple loop-based delays, no init needed
    void Loop (volatile uint32_t ACounter) { while(ACounter--); }
    void ms (uint32_t Ams);
    // Interrupt-driven delays
    bool Elapsed(uint32_t *AVar, const uint32_t ADelay);
    void Reset  (uint32_t *AVar) { *AVar = TickCounter; }
    void Bypass (uint32_t *AVar, const uint32_t ADelay) { *AVar = TickCounter - ADelay; }

    void IncreaseTickCounter(void) { TickCounter++; }
};

extern Delay_t Delay;

// Declare Timer IRQ. Use externC to make it visible from asm file.
extern "C" {
void TIM2_IRQHandler(void);
}




#endif /* KL_GPIO_H_ */
