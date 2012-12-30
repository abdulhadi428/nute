/*
 * kl_lib_f205.h
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#ifndef KL_LIB_F2XX_H_
#define KL_LIB_F2XX_H_

#include "stm32f2xx.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_tim.h"
#include "stm32f2xx_dma.h"

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
//enum PinMode_t {
//    pmIn        = 0b00,
////    pmOut       = 0b01,
////    pmAlterFunc = 0b10,
//    pmAnalog    = 0b11
//};
enum PinOutMode_t {
    omPushPull  = 0,
    omOpenDrain = 1
};
enum PinPullUpDown_t {
    pudNone = 0b00,
    pudPullUp = 0b01,
    pudPullDown = 0b10
};
enum PinSpeed_t {
    ps2MHz  = 0b00,
    ps25MHz = 0b01,
    ps50MHz = 0b10,
    ps100MHz = 0b11
};
enum PinAF_t {
    AF0=0, AF1=1, AF2=2, AF3=3, AF4=4, AF5=5, AF6=6, AF7=7,
    AF8=8, AF9=9,AF10=10, AF11=11, AF12=12, AF13=13, AF14=14, AF15=15
};

// Set/clear
static inline void PinSet    (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->BSRRL = (uint32_t)(1<<APinNumber); }
static inline void PinClear  (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->BSRRH = (uint32_t)(1<<APinNumber); }
static inline void PinToggle (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->ODR  ^= (uint32_t)(1<<APinNumber); }
// Check state
static inline bool PinIsSet(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { return (PGpioPort->IDR & (uint32_t)(1<<APinNumber)); }
// Setup
static inline void PinClockEnable(GPIO_TypeDef *PGpioPort) {
    if     (PGpioPort == GPIOA) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
    else if(PGpioPort == GPIOB) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOB;
    else if(PGpioPort == GPIOC) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;
    else if(PGpioPort == GPIOD) RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
}
// GPIOA, 1, omPushPull/omOpenDrain, pudNone/pudPullUp/pudPullDown, ps2MHz/ps25MHz/ps50MHz/ps100MHz
static inline void PinSetupOut(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber, const PinOutMode_t PinOutMode, const PinPullUpDown_t APullUpDown = pudNone, const PinSpeed_t ASpeed = ps50MHz) {
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
static inline void PinSetupIn(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber, const PinPullUpDown_t APullUpDown) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));          // clear previous bits
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
}
static inline void PinSetupAnalog(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber, const PinPullUpDown_t APullUpDown = pudNone) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER |= 0b11 << (APinNumber*2);  // Set new bits
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
    uint32_t n = (APinNumber <= 7)? 0 : 1;      // 0 if 0...7, 1 if 8..15
    uint32_t Shift = 4 * ((APinNumber <= 7)? APinNumber : APinNumber - 8);
    PGpioPort->AFR[n] &= ~(0b1111 << Shift);
    PGpioPort->AFR[n] |= (uint32_t)AAlterFunc << Shift;
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

// ============================== UART command unit ============================
// USART1: DMA2, Chnl 4, Stream 7
#define UART_TXBUF_SIZE         18
#define UART1TX_DMA_CHNL        DMA_Channel_4
#define UART1TX_DMA_STREAM      DMA2_Stream7
#define UART1TX_DMA_FLAG_TC     DMA_FLAG_TCIF7

//#define RX_ENABLED

#ifdef RX_ENABLED
#define UART_RXBUF_SIZE     45
enum CmdState_t {csNone, csInProgress, csReady};
#endif

class CmdUnit_t {
private:
    DMA_InitTypeDef DMA_InitStructure;
    uint8_t TXBuf[UART_TXBUF_SIZE];
    uint8_t *PWrite, *PRead;
    uint16_t ICountToSendNext;
    bool IDmaIsIdle;
#ifdef RX_ENABLED
    CmdState_t CmdState;
    char RXBuf[UART_RXBUF_SIZE];
    uint8_t RxIndx;
    void CmdReset(void) { RxIndx = 0; CmdState = csNone; }
#endif
    void IStartTx();
public:
    void Task();
    char UintToHexChar (uint8_t b) { return ((b<=0x09) ? (b+'0') : (b+'A'-10)); }
    void Printf(const char *S, ...);
    void FlushTx() { while(!IDmaIsIdle); }  // wait DMA
    void Init(uint32_t ABaudrate);
#ifdef RX_ENABLED
    void Task();
    void NewCmdHandler();   // Place it where needed
    void RxIRQHandler();
#endif
    void IRQDmaTxHandler();
};

// RX IRQ
#ifdef RX_ENABLED
extern "C" {
void USART1_IRQHandler(void);
}
#endif
extern "C" {
void DMA1_Channel2_3_IRQHandler(void);
}


extern CmdUnit_t Uart;


#endif /* KL_LIB_F2XX_H_ */
