/*
 * peripheral.cpp
 *
 *  Created on: 18.01.2013
 *      Author: kreyl
 */

#include "ch.h"
#include "hal.h"
#include "clocking.h"
#include "southbridge.h"
#include "string.h"     // For memcpy
#include "SouthbridgeTxRx.h"

// Variables
KeyStatus_t Key[KEY_COUNT];
PwrStatus_t PwrStatus;
IR_t IR;
Pill_t Pill[PILL_COUNT_MAX];
SouthBridge_t SouthBridge;

// Feeders
KeysFdr_t KeysFdr;

Feeder_t* const PFeeders[] = {
        &KeysFdr,
};
const uint8_t FeederCnt = countof(PFeeders);

// ========================== Peripheral functions =============================
void Beep(const BeepChunk_t *PSequence) {
    SBCmd_t Cmd;
    Cmd.CmdType = NTS_BEEP;
    Cmd.Ptr = (void*)PSequence;
    // Count of bytes of sequence
    Cmd.DataSz = 0;
    while(PSequence->VolumePercent >= 0) {
        Cmd.DataSz += BEEP_CHUNK_SZ;
        PSequence++;
    }
    Transmitter.AddCmd(&Cmd);
}

// ================================== Keys =====================================
// ==== Feeding data ====
FeederRetVal_t KeysFdr_t::FeedStart(uint8_t Byte) {
    if(Byte == STN_KEY_STATUS) {
        PFeedData = (uint8_t*)&Key[0];
        FdrByteCnt = 0;
        return frvOk;
    }
    return frvNoMore;
}
FeederRetVal_t KeysFdr_t::FeedData(uint8_t Byte) {
    *PFeedData = Byte;
    FdrByteCnt++;
    // Check if last possible byte received
    if(FdrByteCnt >= KEY_COUNT) return frvNoMore;
    PFeedData++;
    return frvOk;
}

// =============================== SouthBridge =================================
// Inner use
static inline void IResetLo() { PinClear(SB_GPIO, SB_RST); }
//static inline void IResetHi() { PinSet  (SB_GPIO, SB_RST); }
inline void SBUartInit();

void SouthBridge_t::Init() {
    // Reset South Bridge
    //PinSetupOut(GPIOC, 5, omPushPull);
    Status = sbsOff;
    //IResetLo();
    IInitVars();
    Transmitter.Init();
    Rcvr.Init();
    SBUartInit();
}

void SouthBridge_t::On() {
    Shutdown();
    IInitVars();
    SBUartInit();
    //IResetHi();
}

void SouthBridge_t::Shutdown() {
    Status = sbsOff;
    IResetLo();
}

void SouthBridge_t::IInitVars() {
    Status = sbsOff;    // }
    FwVersion = 0;      // } Will be changed by receiving AnswerToReset
    // Init external structures
    for(uint8_t i=0; i<KEY_COUNT; i++) Key[i] = keyReleased;
    PwrStatus.ExternalPwrOn = false;
    PwrStatus.IsCharging = false;
    PwrStatus.Voltage_mV = 0;
    IR.RxSize = 0;
    for(uint8_t i=0; i<PILL_COUNT_MAX; i++) Pill[i].Status = psNotConnected;
}

// ================================= Harsware init =============================
// Called from Transmitter.init
void SBUartInit() {
    // Pins
    PinSetupAlterFunc(SB_GPIO, SB_OUT, omPushPull,  pudNone,   AF8);
    PinSetupAlterFunc(SB_GPIO, SB_IN,  omOpenDrain, pudPullUp, AF8);

    // ==== USART init ====
    rccEnableUSART6(FALSE);         // Usart6 CLK, no clock in low-power
    SB_UART->CR1 = USART_CR1_UE;    // Enable UART
    SB_UART->CR2 = 0;
    SB_UART->CR3 = USART_CR3_DMAT;  // Enable DMA at transmitter
    SB_UART->BRR = SB_UART_CLK / SB_BAUDRATE;    // Baudrate
    SB_UART->CR1 =
            USART_CR1_TE            // Transmitter enable
            | USART_CR1_RE          // Receiver enable
            | USART_CR1_RXNEIE;     // RX Irq enable
    nvicEnableVector(STM32_USART6_NUMBER, CORTEX_PRIORITY_MASK(STM32_SERIAL_USART6_PRIORITY));

    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (SB_DMA_STREAM, 1, IrqSBTxCompleted, NULL);
    dmaStreamSetPeripheral(SB_DMA_STREAM, &SB_UART->DR);
    dmaStreamSetMode      (SB_DMA_STREAM,
            STM32_DMA_CR_CHSEL(5) |     // DMA2 Stream6 Channel5 is USART6_TX request
            DMA_PRIORITY_MEDIUM |
            STM32_DMA_CR_MSIZE_BYTE |
            STM32_DMA_CR_PSIZE_BYTE |
            STM32_DMA_CR_MINC |         // Memory pointer increase
            STM32_DMA_CR_DIR_M2P |      // Direction is memory to peripheral
            STM32_DMA_CR_TCIE           // Enable TX Completed irq
            );
    SB_UART->CR1 |= USART_CR1_UE;       // Enable USART
}
