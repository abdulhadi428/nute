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

// Variables
bool KeyPressed[KEY_COUNT];
PwrStatus_t PwrStatus;
IR_t IR;
Pill_t Pill[PILL_COUNT_MAX];
SouthBridge_t SouthBridge;

// ========================== Peripheral functions =============================
void Beep(const BeepSequence_t *PSequence) {
    SBCmd_t Cmd;
    Cmd.CmdType = NTS_BEEP_HEADER;
    Cmd.Ptr = (void*)PSequence;
    SouthBridge.PutCmd(&Cmd);
}


// =============================== SouthBridge =================================
// Inner use
static inline void IResetLo() { PinClear(SB_GPIO, SB_RST); }
static inline void IResetHi() { PinSet  (SB_GPIO, SB_RST); }
static inline void SBUartInit();

// ==== Southbridge Thread ====
static WORKING_AREA(waSBThread, 128);
static msg_t SBThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Southbridge");
    while(1) {
        // Stop thread if off
        //if(SouthBridge.Status == sbsOff) chSchGoSleepS(THD_STATE_SUSPENDED);
        // Wait for msg with work to do
        SouthBridge.FetchAndDispatchCmd();
    }
    return 0;
}

// ==== Southbridge ====
void SouthBridge_t::FetchAndDispatchCmd() {
    // ==== Fetch Cmd ====
    if(PCmdRead->CmdType == CMD_NONE) return;

    // ==== Dispatch cmd ====
    //while(!IsNTSIdle);  // Wait previous transmission to complete
    NTSPkt.MsgType = PCmdRead->CmdType;
    switch(PCmdRead->CmdType) {
        case NTS_BEEP_HEADER: DispatchBeep(); break;

        default: break;
    } // switch

    // ==== Handle buffer ====
    PCmdRead->CmdType = CMD_NONE;   // Mark current as dispatched
    PCmdRead++;
    if(PCmdRead >= &CmdBuf[SB_CMDBUF_SZ]) PCmdRead = CmdBuf;
}

// ================ Dispatchers =============
void SouthBridge_t::DispatchBeep() {
    BeepSequence_t *PSeq = (BeepSequence_t*)PCmdRead->Ptr;
    // First stage: header. Chunk count and first chunk.
    NTSPkt.ChunkCnt = PSeq->Count;
    memcpy(&NTSPkt.Chunk0, &PSeq->Chunk[0], BEEP_CHUNK_SZ);
    Transmit(&NTSPkt, SB_PKT_SZ);
    // Second stage: remaining chunks
    uint8_t cnt = PSeq->Count - 1;
    if(cnt) {   // More chunks
        uint16_t Length = BEEP_CHUNK_SZ * cnt;
        memcpy(IBuf, &PSeq->Chunk[1], Length);
        Transmit(IBuf, Length);
    }
}

// ================= General ===============
void SouthBridge_t::Transmit(void *Ptr, uint16_t Length) {
    dmaStreamSetMemory0(SB_DMA_STREAM, Ptr);
    dmaStreamSetTransactionSize(SB_DMA_STREAM, Length);
    dmaStreamEnable(SB_DMA_STREAM);
    dmaWaitCompletion(SB_DMA_STREAM);
}

void SouthBridge_t::PutCmd(SBCmd_t *PCmd) {
    while(PCmdWrite->CmdType != CMD_NONE);      // Wait for empty cmd slot
    memcpy(PCmdWrite, PCmd, SB_CMD_SZ);     // Write cmd to buf
    // Handle buffer
    PCmdWrite++;
    if(PCmdWrite >= &CmdBuf[SB_CMDBUF_SZ]) PCmdWrite = CmdBuf;
}

void SouthBridge_t::Init() {
    // Reset South Bridge
    PinSetupOut(GPIOC, 5, omPushPull);
    Status = sbsOff;
    IResetLo();
    // Create and start thread
    chThdCreateStatic(waSBThread, sizeof(waSBThread), NORMALPRIO, SBThread, NULL);
    On();   // Switch it on
}

void SouthBridge_t::On() {
    Shutdown();
    IInitVars();
    SBUartInit();
    IResetHi();
}

void SouthBridge_t::Shutdown() {
    Status = sbsOff;
    IResetLo();
}

void SouthBridge_t::IInitVars() {
    Status = sbsOff;    // }
    FwVersion = 0;      // } Will be changed by receiving AnswerToReset
    // Init buffer
    for(uint8_t i=0; i<SB_CMDBUF_SZ; i++) CmdBuf[i].CmdType = CMD_NONE;
    PCmdRead = CmdBuf;
    PCmdWrite = CmdBuf;
    // Init external structures
    for(uint8_t i=0; i<KEY_COUNT; i++) KeyPressed[i] = 0;
    PwrStatus.ExternalPwrOn = false;
    PwrStatus.IsCharging = false;
    PwrStatus.Voltage_mV = 0;
    IR.RxSize = 0;
    for(uint8_t i=0; i<PILL_COUNT_MAX; i++) Pill[i].Status = psNotConnected;
}

// Wrapper for IRQ
//extern "C" {
//void IrqSBTxCompleted(void *p, uint32_t flags) { SouthBridge.IRQDmaTxHandler(); }
//}

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
            USART_CR1_TE |      // Transmitter enable
            USART_CR1_RE;       // Receiver enable
    // After enabling transmitter, it will send empty character
    //while(!(USART3->SR & USART_SR_TC));    // wait for transmission to complete
    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (SB_DMA_STREAM, 1, NULL, NULL);
    dmaStreamSetPeripheral(SB_DMA_STREAM, &SB_UART->DR);
    dmaStreamSetMode      (SB_DMA_STREAM,
            STM32_DMA_CR_CHSEL(5) |     // DMA2 Stream6 Channel5 is USART6_TX request
            DMA_PRIORITY_LOW |
            STM32_DMA_CR_MSIZE_BYTE |
            STM32_DMA_CR_PSIZE_BYTE |
            STM32_DMA_CR_MINC |         // Memory pointer increase
            STM32_DMA_CR_DIR_M2P        // Direction is memory to peripheral
            );
    SB_UART->CR1 |= USART_CR1_UE;        // Enable USART
}
