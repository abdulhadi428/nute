/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f2xx.h"
#include <stdarg.h>
#include <string.h>
#include "tiny_sprintf.h"
/*
#include "stm32f2xx_usart.h"
#include "misc.h"

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

// ============================== UART command =================================
CmdUnit_t Uart;
#define UADMA_DATASIZE      UART1TX_DMA_STREAM->NDTR
#define UADMA_MEMPOINTER    UART1TX_DMA_STREAM->M0AR

void CmdUnit_t::Printf(const char *format, ...) {
    char buf[200];
    va_list args;
    va_start(args, format);
    uint32_t Cnt = tiny_vsprintf(buf, format, args);
    va_end(args);
    if(Cnt > UART_TXBUF_SIZE) Cnt = UART_TXBUF_SIZE;    // Shrink too long string

    if(IDmaIsIdle) {
        memcpy(TXBuf, buf, Cnt);    // Place string to buffer from beginning
        PWrite = TXBuf + Cnt;       // Prepare pointer for next time
        PRead = TXBuf + Cnt;        // Prepare pointer for next time
        ICountToSendNext = 0;       // Reset next-time counter
        // Start DMA
        IDmaIsIdle = false;
        UADMA_MEMPOINTER = (uint32_t)TXBuf;
        UADMA_DATASIZE = Cnt;
        DMA_Cmd(UART1TX_DMA_STREAM, ENABLE);
    }
    else {
        ICountToSendNext += Cnt;
        uint32_t BytesFree = UART_TXBUF_SIZE - (PWrite - TXBuf);
        if(Cnt < BytesFree) {   // Message fits in buffer, no splitting needed
            memcpy(PWrite, buf, Cnt);
            PWrite += Cnt;
        }
        else { // Cnt >= BytesFree
            memcpy(PWrite, buf, BytesFree);
            uint32_t Remainder = Cnt - BytesFree;
            if(Remainder) memcpy(TXBuf, &buf[BytesFree], Remainder);
            PWrite = TXBuf + Remainder;
        }
    } // if not idle
}

// ==== Init & DMA ====
void CmdUnit_t::Init(uint32_t ABaudrate) {
    PWrite = TXBuf;
    PRead = TXBuf;
    ICountToSendNext = 0;
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
*/
    // ==== DMA ====
    /* Here only the unchanged parameters of the DMA initialization structure are
     * configured. During the program operation, the DMA will be configured with
     * different parameters according to the operation phase.
     */
/*    DMA_DeInit(UART1TX_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
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
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)TXBuf; // }
    DMA_InitStructure.DMA_BufferSize         = UART_TXBUF_SIZE; // } dummy
    DMA_Init(UART1TX_DMA_STREAM, &DMA_InitStructure);
    // Enable DMA Transfer Complete interrupt
    DMA_ITConfig(UART1TX_DMA_STREAM, DMA_IT_TC, ENABLE);

    // ==== Interrupts ====
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

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
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
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
    if(DMA_GetFlagStatus(UART1TX_DMA_STREAM, UART1TX_DMA_FLAG_TC)) {
        DMA_ClearFlag(UART1TX_DMA_STREAM, UART1TX_DMA_FLAG_TC);
        DMA_Cmd(UART1TX_DMA_STREAM, DISABLE);   // Registers may be changed only when stream is disabled

        if(ICountToSendNext == 0) IDmaIsIdle = true;
        else {  // There is something to transmit more
            UADMA_MEMPOINTER = (uint32_t)PRead;
            // Handle pointer
            uint32_t BytesLeft = UART_TXBUF_SIZE - (PRead - TXBuf);
            if(ICountToSendNext < BytesLeft) {      // Data fits in buffer without split
                UADMA_DATASIZE = ICountToSendNext;
                PRead += ICountToSendNext;
                ICountToSendNext = 0;
            }
            else {  // Some portion of data placed in the beginning
                UADMA_DATASIZE = BytesLeft;
                PRead = TXBuf;  // Set pointer to beginning
                ICountToSendNext -= BytesLeft;
            }
            DMA_Cmd(UART1TX_DMA_STREAM, ENABLE);    // Restart DMA
        }
    } // if getflagstatus
}

void DMA2_Stream7_IRQHandler(void) {
    Uart.IRQDmaTxHandler();
}
*/
