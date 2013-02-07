/*
 * bridge.cpp
 *
 *  Created on: 06.02.2013
 *      Author: kreyl
 */

#include "ch.h"
#include "hal.h"
#include "kl_lib_f0.h"
#include "clocking_f0.h"
#include "bridge.h"
#include "string.h"     // For memcpy

#define SB_PKT_START      0xEE

Bridge_t Bridge;

// =============================== Transmitter =================================
class Transmitter_t {
    private:
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
    dmaStreamSetMemory0(STM32_DMA1_STREAM2, TxBuf);
    dmaStreamSetTransactionSize(STM32_DMA1_STREAM2, BufCounter);
    dmaStreamEnable(STM32_DMA1_STREAM2);
    // Put thread to sleep, it will wake when transmission completes
    chSysLock();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chSysUnlock();
}

// =============================== SouthBridge =================================
// Inner use
static inline void UartInit();
// Wrapper for IRQ
extern "C" {
void IrqSBTxCompleted(void *p, uint32_t flags) {
    // Later, registers may be changed only when stream is disabled
    dmaStreamDisable(STM32_DMA1_STREAM2);
    // Wake thread
    chSysLockFromIsr();
    chSchReadyI(Bridge.PTxThread);
    chSysUnlockFromIsr();
}
} // extern C

// ==== Bridge Thread ====
static WORKING_AREA(waTxThread, 128);
static msg_t TxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("BridgeTx");
    while(1) {
        if(Bridge.IsCmdQueueEmpty()) chThdYield();  // No commands - do not waste time
        else Bridge.DispatchCmd();
    }
    return 0;
}

// ==== Southbridge ====
void Bridge_t::DispatchCmd() {
    // Dispatch cmd
    Transmitter.SendCmd(PCmdRead);
    // Handle buffer
    PCmdRead->CmdType = CMD_NONE;   // Mark current as dispatched
    PCmdRead++;
    if(PCmdRead >= &CmdBuf[SB_CMDBUF_SZ]) PCmdRead = CmdBuf;
}

void Bridge_t::AddCmd(SBCmd_t *PCmd) {
    while(PCmdWrite->CmdType != CMD_NONE);  // Wait for empty cmd slot
    memcpy(PCmdWrite, PCmd, SB_CMD_SZ);     // Write cmd to buf
    // Handle buffer
    PCmdWrite++;
    if(PCmdWrite >= &CmdBuf[SB_CMDBUF_SZ]) PCmdWrite = CmdBuf;
}

void Bridge_t::Init() {
    UartInit();
    IInitVars();
    // Create and start thread
    PTxThread = chThdCreateStatic(waTxThread, sizeof(waTxThread), NORMALPRIO, TxThread, NULL);
}

void Bridge_t::IInitVars() {
    // Init cmd buffer
    for(uint8_t i=0; i<SB_CMDBUF_SZ; i++) CmdBuf[i].CmdType = CMD_NONE;
    PCmdRead = CmdBuf;
    PCmdWrite = CmdBuf;
    // Init external structures

}

void UartInit() {
    // Pins
    PinSetupAlterFunc(GPIOA, 9, omPushPull,  pudNone,   AF1);
    //PinSetupAlterFunc(SB_GPIO, SB_IN,  omOpenDrain, pudPullUp, AF8);
    // ==== USART init ====
    rccEnableUSART1(FALSE);         // Usart1 CLK, no clock in low-power
    USART1->BRR = Clk.APBFreqHz / SB_BAUDRATE;    // Baudrate
    USART1->CR2 = 0;
    USART1->CR3 = USART_CR3_DMAT;   // Enable DMA at transmitter
    USART1->CR1 =
            USART_CR1_TE |          // Transmitter enable
            USART_CR1_RE;           // Receiver enable
    // ==== DMA ====
    // Here only the unchanged parameters of the DMA are configured.
    dmaStreamAllocate     (STM32_DMA1_STREAM2, 1, IrqSBTxCompleted, NULL);
    dmaStreamSetPeripheral(STM32_DMA1_STREAM2, &USART1->TDR);
    dmaStreamSetMode      (STM32_DMA1_STREAM2,
              STM32_DMA_CR_PL(0b10)     // Priority is high
            | STM32_DMA_CR_MSIZE_BYTE
            | STM32_DMA_CR_PSIZE_BYTE
            | STM32_DMA_CR_MINC         // Memory pointer increase
            | STM32_DMA_CR_DIR_M2P      // Direction is memory to peripheral
            | STM32_DMA_CR_TCIE         // Enable Transmission Complete IRQ
            );
    USART1->CR1 |= USART_CR1_UE;        // Enable USART
}

