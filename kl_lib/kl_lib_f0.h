/*
 * kl_lib_f0xx.h
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#ifndef KL_LIB_F0XX_H_
#define KL_LIB_F0XX_H_

#include "stm32f0xx.h"

extern "C" {
//void _init(void);   // Need to calm linker
void __attribute__ ((weak)) _init(void)  {}

}

// =============================== General =====================================
#define PACKED __attribute__ ((__packed__))
#ifndef countof
#define countof(A)  (sizeof(A)/sizeof(A[0]))
#endif


// ===================== Single pin manipulations ==============================
enum PinMode_t {
    pmIn        = 0b00,
//    pmOut       = 0b01,
//    pmAlterFunc = 0b10,
    pmAnalog    = 0b11
};
enum PinOutMode_t {
    poPushPull  = 0,
    poOpenDrain = 1
};
enum PinPullUpDown_t {
    pudNone = 0b00,
    pudPullUp = 0b01,
    pudPullDown = 0b10
};
enum PinSpeed_t {
    ps2MHz  = 0b00,
    ps10MHz = 0b01,
    ps50MHz = 0b11
};
enum PinAF_t {
    AF0=0, AF1=1, AF2=2, AF3=3, AF4=4, AF5=5, AF6=6, AF7=7
};

// Set/clear
static inline void PinSet    (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->BSRR = (uint32_t)(1<<APinNumber); }
static inline void PinClear  (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->BRR  = (uint32_t)(1<<APinNumber); }
static inline void PinToggle (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->ODR ^= (uint32_t)(1<<APinNumber); }
// Check state
static inline bool PinIsSet(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { return (PGpioPort->IDR & (uint32_t)(1<<APinNumber)); }
// Setup
static inline void PinClockEnable(GPIO_TypeDef *PGpioPort) {
    if     (PGpioPort == GPIOA) RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    else if(PGpioPort == GPIOB) RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    else if(PGpioPort == GPIOC) RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
}
static inline void PinSetupOut(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber, const PinOutMode_t PinOutMode, const PinPullUpDown_t APullUpDown, const PinSpeed_t ASpeed = ps50MHz) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));  // clear previous bits
    PGpioPort->MODER |= 0b01 << (APinNumber*2);     // Set new bits
    // Setup output type
    PGpioPort->OTYPER &= ~(1<<APinNumber);
    PGpioPort->OTYPER |= (uint32_t)PinOutMode << APinNumber;
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
    // Setup speed
    PGpioPort->OSPEEDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->OSPEEDR |= (uint32_t)ASpeed << (APinNumber*2);
}
static inline void PinSetupIn(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber, const PinMode_t AMode, const PinPullUpDown_t APullUpDown) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));          // clear previous bits
    PGpioPort->MODER |= (uint32_t)AMode << (APinNumber*2);  // Set new bits
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
}
static inline void PinSetupAlterFunc(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber, const PinOutMode_t PinOutMode, const PinPullUpDown_t APullUpDown, const PinAF_t AAlterFunc, const PinSpeed_t ASpeed = ps50MHz) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));  // clear previous bits
    PGpioPort->MODER |= 0b10 << (APinNumber*2);     // Set new bits
    // Setup output type
    PGpioPort->OTYPER &= ~(1<<APinNumber);
    PGpioPort->OTYPER |= (uint32_t)PinOutMode << APinNumber;
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
    // Setup speed
    PGpioPort->OSPEEDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->OSPEEDR |= (uint32_t)ASpeed << (APinNumber*2);
    // Setup Alternate Function
    uint32_t n = (APinNumber <= 7)? 0 : 1;
    PGpioPort->AFR[n] &= ~(0b1111 << (APinNumber*4));
    PGpioPort->AFR[n] |= (uint32_t)AAlterFunc << (APinNumber*4);
}

// ============================ Delay and time =================================
// Systick is used here
extern uint32_t ITickCounter;
class Delay_t {
public:
    // Configure SysTick timer: Reload Value = SysTick Counter Clock (Hz) x Desired Time base (s)
    void Init(void) { SysTick_Config((uint32_t)(SystemCoreClock / 1000)); }   // 1/1000 s and clk = sysclk/8
    // Simple loop-based delays, no init needed
    void Loop (volatile uint32_t ACounter) { while(ACounter--); }
    void ms (uint32_t Ams);
    // Timer-driven delays
    bool Elapsed(uint32_t *AVar, const uint32_t ADelay);
    void Reset  (uint32_t *AVar) { *AVar = ITickCounter; }
    void Bypass (uint32_t *AVar, const uint32_t ADelay) { *AVar = (uint32_t)(ITickCounter - ADelay); }
};
extern Delay_t Delay;
// IRQ handler
extern "C" {
void SysTick_Handler(void);
}



#endif /* KL_LIB_F0XX_H_ */
