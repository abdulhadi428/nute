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

#define SB_PKT_START      0xEE

class Transmitter_t {
private:
    //uint8_t *PSrc, *Dst;
    //uint32_t SrcSz, DstSz, LastPackSz;
    uint32_t BufCounter;
    void Transmit();
    uint8_t TxBuf[SB_UARTBUF_SZ];
    uint8_t *PArray;
    uint32_t ArraySz;
    uint8_t AddBytePacked(uint8_t Byte);
    void PackArray();
public:
    void SendCmd(SBCmd_t *PCmd);
} Transmitter;

// ========================== Peripheral functions =============================
void Beep(const BeepSequence_t *PSequence) {
    SBCmd_t Cmd;
    Cmd.CmdType = NTS_BEEP;
    Cmd.Ptr = (void*)PSequence;
    // Count of bytes of sequence: 1 + N*5
    Cmd.DataSz = 1 + BEEP_CHUNK_SZ * PSequence->Count;
    SouthBridge.AddCmd(&Cmd);
}


// =============================== Transmitter =================================
void Transmitter_t::SendCmd(SBCmd_t *PCmd) {
    PArray = (uint8_t*)(PCmd->Ptr);
    ArraySz = PCmd->DataSz;
    // Build the pkt
    TxBuf[0] = SB_PKT_START;        // Start of pkt
    BufCounter = 1;
    AddBytePacked(PCmd->CmdType);   // Cmd code
    PackArray();
    Transmit();  // Transmit builded pkt
    // Check if pkt did not fit in buf and additional data sennding required
    while(ArraySz) {
        BufCounter = 1;
        AddBytePacked(CMD_ADDITIONAL_DATA);  // Cmd code
        PackArray();
        Transmit();  // Transmit builded pkt
    }
}

/*
 * Packs single byte, tries put result to buf. If buf overflow could occur,
 * returns 1 and do nothing with buffer. If succeded, returns 0 and increases
 * BufCounter.
 */
uint8_t Transmitter_t::AddBytePacked(uint8_t Byte) {
    if(Byte == 0xEE) {
        if(BufCounter < SB_UARTBUF_SZ-1) {  // Check if enough place for two bytes
            TxBuf[BufCounter++] = Byte;
            TxBuf[BufCounter++] = Byte;
            return 0;
        }
    }
    else {
        if (BufCounter < SB_UARTBUF_SZ)  {  // Check if enough space for one byte
            TxBuf[BufCounter++] = Byte;
            return 0;
        }
    }
    return 1;
}
/*
 * Packs Array byte-by-byte to buf, decreasing ArraySz. If buf overflow could occur,
 * just
 * After return, *Src contains pointer to next element of Src,
 * PSrcSz contains number of bytes left in Src.
 * Returns pointer to next element of Dst.
 */
void Transmitter_t::PackArray() {
    while(ArraySz) {
        if(AddBytePacked(*PArray)) return;
        PArray++;
        ArraySz--;
    }
}

void Transmitter_t::Transmit() {
    dmaStreamSetMemory0(SB_DMA_STREAM, TxBuf);
    dmaStreamSetTransactionSize(SB_DMA_STREAM, BufCounter);
    dmaStreamEnable(SB_DMA_STREAM);
    // FIXME
    dmaWaitCompletion(SB_DMA_STREAM);
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
    // Fetch Cmd
    if(PCmdRead->CmdType == CMD_NONE) return;
    // Dispatch cmd
    Transmitter.SendCmd(PCmdRead);
    // Handle buffer
    PCmdRead->CmdType = CMD_NONE;   // Mark current as dispatched
    PCmdRead++;
    if(PCmdRead >= &CmdBuf[SB_CMDBUF_SZ]) PCmdRead = CmdBuf;
}


// ================= General ===============
void SouthBridge_t::AddCmd(SBCmd_t *PCmd) {
    while(PCmdWrite->CmdType != CMD_NONE);  // Wait for empty cmd slot
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
    IsTxIdle = true;
    // Init cmd buffer
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
