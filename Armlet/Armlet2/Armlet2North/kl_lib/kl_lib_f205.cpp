/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f205.h"
#include <stdarg.h>
#include "tiny_sprintf.h"
#include "stm32f2xx_usart.h"
/*
// ============================== Delay ========================================
Delay_t Delay;
uint32_t ITickCounter;
bool Delay_t::Elapsed(uint32_t *AVar, const uint32_t ADelay) {
    if ((uint32_t)(ITickCounter - (*AVar)) >= ADelay) {
        *AVar = ITickCounter; // Reset delay
        return true;
    }
    else return false;
}

void Delay_t::ms(uint32_t Ams) {
    uint32_t __ticks = (SystemCoreClock / 8000) * Ams;
    Loop(__ticks);
}

// IRQ
void SysTick_Handler(void) {
    ITickCounter++;
}
*/
// ============================== UART command =================================
CmdUnit_t Uart;

void CmdUnit_t::Printf(const char *format, ...) {
    char buf[200];
    va_list args;
    va_start(args, format);
    uint32_t Cnt = tiny_vsprintf(buf, format, args);
    va_end(args);
    // Check if message fits in buffer's remainder
    if(Cnt > (UART_TXBUF_SIZE - (PWrite - TXBuf))) PWrite = TXBuf;  // Set pointer to start
    // Place bytes to buffer
    for(uint32_t i=0; (i<Cnt) and (i<UART_TXBUF_SIZE); i++) *PWrite++ = buf[i];
    if(PWrite == &TXBuf[UART_TXBUF_SIZE-1]) PWrite = TXBuf; // Handle circular buffer
    // Start transmission if DMA is idle
    if(IDmaIsIdle) {        // Send ICountToSend chars staring from PRead
        ICountToSend = Cnt;
        IStartTx();
        PRead = PWrite;     // Reset read pointer for next time
        ICountToSend = 0;   // Nothing to send next time
    }
    else {                  // DMA is busy transmitting, prepare for it to empty
        ICountToSend += Cnt;
    }
}

void CmdUnit_t::IStartTx() {
    IDmaIsIdle = false;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)PRead;
    DMA_InitStructure.DMA_BufferSize = ICountToSend;
    DMA_Init(UART1TX_DMA_STREAM, &DMA_InitStructure);

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(UART1TX_DMA_STREAM, ENABLE);
//    UART_DMA_CHNL->CMAR = (uint32_t) PBuf;  // Set memory base address
//    UART_DMA_CHNL->CNDTR = TxIndx;          // Set count to transmit
//    DMA_Cmd(UART_DMA_CHNL, ENABLE);
//    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

// ==== Init & DMA ====
void CmdUnit_t::Init(uint32_t ABaudrate) {
    PWrite = TXBuf;
    PRead = TXBuf;
    ICountToSend = 0;
    IDmaIsIdle = true;
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);      // UART clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    // ==== GPIO init ====
    PinSetupAlterFunc(GPIOA, 9, omPushPull, pudNone, AF7);      // TX1
#ifdef RX_ENABLED
    klGpioSetupByN(GPIOA, 10, GPIO_Mode_IPU);   // RX1
#endif
    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = ABaudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
#ifdef RX_ENABLED
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
#else
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
#endif
    USART_Init(USART1, &USART_InitStructure);

    // Remap USART1 TX DMA Ch to DMA Ch4
    //SYSCFG->CFGR1 |= SYSCFG_CFGR1_USART1TX_DMA_RMP;
    // ==== DMA ====
    /* Here only the unchanged parameters of the DMA initialization structure are
     * configured. During the program operation, the DMA will be configured with
     * different parameters according to the operation phase.
     */
    DMA_DeInit(UART1TX_DMA_STREAM);
    DMA_InitStructure.DMA_Channel            = UART1TX_DMA_CHNL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART1->DR;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
//    DMA_InitStructure.DMA_BufferSize         = UART_TXBUF_SIZE;
//    DMA_Init(UART_DMA_CHNL, &DMA_InitStructure);
//    // Enable DMA1 Ch2 Transfer Complete interrupt
//    DMA_ITConfig(UART_DMA_CHNL, DMA_IT_TC, ENABLE);
//
//    // ==== Interrupts ====
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

#ifdef RX_ENABLED
    // ==== NVIC ====
    // Enable the USART Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Enable RX interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
    // Enable USART
    USART_Cmd(USART1, ENABLE);
}

#ifdef RX_ENABLED
void CmdUnit_t::Task() {
    if (CmdState == csReady) {
        NewCmdHandler();
        CmdReset();
    }
}
#endif

// ==== IRQs ====
#ifdef RX_ENABLED
void CmdUnit_t::IRQHandler() {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        char b = USART1->DR;
        if (b != '\n') switch (CmdState) {  // Ignore \n
            case csNone:
                RXBuf[RxIndx++] = b;
                CmdState = csInProgress;
                break;
            case csInProgress:
                // Check if end of cmd
                if (b == '\r') {
                    CmdState = csReady;
                    RXBuf[RxIndx] = 0;
                }
                else {
                    RXBuf[RxIndx++] = b;
                    // Check if too long
                    if (RxIndx == UART_RXBUF_SIZE) CmdReset();
                }
                break;
            case csReady:   // New byte received, but command still not handled
                break;
        } // switch
    } // if rx
}

void USART1_IRQHandler(void) {
    CmdUnit.IRQHandler();
}
#endif

void CmdUnit_t::IRQDmaTxHandler() {
//    if(DMA_GetITStatus(DMA1_IT_TC2)) {
//        DMA_ClearITPendingBit(DMA1_IT_GL2); // Clear CH2 IRQ global bit
//        // Switch to next buffer if needed
//        if(TxIndx != 0) IStartTx();
//        else IDmaIsIdle = true;
//    }
}

void DMA1_Channel2_3_IRQHandler(void) {
    Uart.IRQDmaTxHandler();
}
