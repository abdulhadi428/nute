/*
 * ws2812b.cpp
 *
 *  Created on: 05 апр. 2014 г.
 *      Author: Kreyl
 */

#include "ws2812b.h"
#include "cmd_uart_f10x.h"

#if LED_WS_ENABLE
LedWs_t LedWs;

// Tx timings
#define T0H_N       3
#define T1H_N       7
#define T_TOTAL_N   10

// DMA
#define LED_DMA_STREAM  STM32_DMA1_STREAM5
#define LED_DMA_MODE    DMA_PRIORITY_VERYHIGH \
                        | STM32_DMA_CR_MSIZE_BYTE \
                        | STM32_DMA_CR_PSIZE_HWORD \
                        | STM32_DMA_CR_MINC     /* Memory pointer increase */ \
                        | STM32_DMA_CR_DIR_M2P  /* Direction is memory to peripheral */ \
                        | STM32_DMA_CR_TCIE     /* Enable Transmission Complete IRQ */

extern "C" {
// Wrapper for Tx Completed IRQ
void LedTxcIrq(void *p, uint32_t flags) {
    dmaStreamDisable(LED_DMA_STREAM);
    LedWs.IStopTx();
    //Uart.Printf("Irq\r");
}
} // "C"

void LedWs_t::Init() {
    // ==== Timer ====
    // Remap T15CH2 to PB15
    bool AfioWasEnabled = (RCC->APB2ENR & RCC_APB2ENR_AFIOEN);
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;     // Enable AFIO
    AFIO->MAPR2 |= AFIO_MAPR2_TIM15_REMAP;
    if (!AfioWasEnabled) RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;
    // Init tmr in PWM mode
    TxTmr.Init();
    TxTmr.Enable();
    TxTmr.InitPwm(GPIOB, 15, 2, invNotInverted, true);
    TxTmr.SetTopValue(T_TOTAL_N);
    TxTmr.SetPwm(0);
    TxTmr.EnableDmaOnUpdate();
    TxTmr.Disable();

    // ==== DMA ====
    dmaStreamAllocate     (LED_DMA_STREAM, IRQ_PRIO_LOW, LedTxcIrq, NULL);
    dmaStreamSetPeripheral(LED_DMA_STREAM, TxTmr.PCCR);
    dmaStreamSetMemory0   (LED_DMA_STREAM, BitBuf);
    dmaStreamSetMode      (LED_DMA_STREAM, LED_DMA_MODE);

    // ==== Initial variables ====
    //for(uint16_t i=0; i<RST_BIT_CNT; i++) BitBuf[i] = T0H_N;    // Set
}

void LedWs_t::SetCommonColor(Color_t Clr) {
    // Fill bit buffer
    PBit = &BitBuf[RST_BIT_CNT];
    for(uint32_t i=0; i<LED_CNT; i++) {
        AppendBitsMadeOfByte(Clr.Green);
        AppendBitsMadeOfByte(Clr.Red);
        AppendBitsMadeOfByte(Clr.Blue);
    }
//    Uart.Printf("%u %u %u\r", Clr.Green, Clr.Red, Clr.Blue);
//    Uart.Printf("%A\r", &BitBuf[RST_BIT_CNT], DATA_BIT_CNT, ' ');
//    Uart.Printf("%A\r", BitBuf, TOTAL_BIT_CNT, ' ');
    // Start transmission
    dmaStreamSetTransactionSize(LED_DMA_STREAM, TOTAL_BIT_CNT);
    dmaStreamEnable(LED_DMA_STREAM);
    TxTmr.SetCounter(0);
    TxTmr.Enable();
}

void LedWs_t::AppendBitsMadeOfByte(uint8_t Byte) {
    for(uint8_t i=0; i<8; i++) {
        if(Byte & 0x80) *PBit = T1H_N;
        else *PBit = T0H_N;
        PBit++;
        Byte <<= 1;
    }
}

#endif
