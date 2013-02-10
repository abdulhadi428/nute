/*
 * SouthbridgeTxRx.cpp
 *
 *  Created on: 09.02.2013
 *      Author: kreyl
 */

#include "SouthbridgeTxRx.h"
#include "southbridge.h"
#include <string.h>

Transmitter_t Transmitter;
//Rcvr_t Rcvr;

// =============================== Transmitter =================================
// ==== Cmd dispatching ====
void Transmitter_t::DispatchCmd() {
    // ==== Build the pkt ====
    BufCounter = 0;
    AddByte(SB_PKT_START);          // Start of pkt
    PackByte(PCmdRead->CmdType);    // Msg type
    // Cmd's data
    PArray = (uint8_t*)(PCmdRead->Ptr);
    ArraySz = PCmdRead->DataSz;
    PackArray();
    AddByte(SB_PKT_END);            // End of pkt
    Transmit();                     // Transmit builded pkt
    // Check if pkt did not fit in buf and additional data sennding required
    while(ArraySz) {
        BufCounter = 0;
        AddByte(SB_PKT_START);
        PackByte(CMD_ADDITIONAL_DATA);
        PackArray();
        AddByte(SB_PKT_END);        // End of pkt
        Transmit();                 // Transmit builded pkt
    }
    // ==== Handle buffer ====
    PCmdRead->CmdType = CMD_NONE;   // Mark current as dispatched
    PCmdRead++;
    if(PCmdRead >= &CmdBuf[SB_CMDBUF_SZ]) PCmdRead = CmdBuf;
}
/*
 * Packs single byte, tries put result to buf. If buf overflow could occur,
 * returns 1 and does nothing with buffer. If succeded, returns 0 and increases
 * BufCounter.
 */
uint8_t Transmitter_t::PackByte(uint8_t Byte) {
    if((Byte == 0xEE) or (Byte == 0xFE)) {
        if(BufCounter < SB_UARTBUF_SZ-2) {  // Check if enough place for two bytes+End of pkt
            TxBuf[BufCounter++] = 0xEE;
            TxBuf[BufCounter++] = Byte;
            return 0;
        }
    }
    else {
        if (BufCounter < SB_UARTBUF_SZ-1)  {  // Check if enough space for one byte+End of pkt
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
        if(PackByte(*PArray)) return;
        PArray++;
        ArraySz--;
    }
}

void Transmitter_t::Transmit() {
    dmaStreamSetMemory0(SB_DMA_STREAM, TxBuf);
    dmaStreamSetTransactionSize(SB_DMA_STREAM, BufCounter);
    dmaStreamEnable(SB_DMA_STREAM);
    // Put thread to sleep, it will wake when transmission completes
    //dmaWaitCompletion(SB_DMA_STREAM);
    chSysLock();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chSysUnlock();
}

// Wrapper for IRQ
extern "C" {
void IrqSBTxCompleted(void *p, uint32_t flags) {
    // Later, registers may be changed only when stream is disabled
    dmaStreamDisable(SB_DMA_STREAM);
    // Wake thread
    chSysLockFromIsr();
    chSchReadyI(Transmitter.PThread);
    chSysUnlockFromIsr();
}
} // extern C

// ==== General ====
void Transmitter_t::AddCmd(SBCmd_t *PCmd) {
    while(PCmdWrite->CmdType != CMD_NONE);  // Wait for empty cmd slot
    memcpy(PCmdWrite, PCmd, SB_CMD_SZ);     // Write cmd to buf
    // Handle buffer
    PCmdWrite++;
    if(PCmdWrite >= &CmdBuf[SB_CMDBUF_SZ]) PCmdWrite = CmdBuf;
}

static WORKING_AREA(waTxThread, 128);
static msg_t TxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("BridgeTx");
    while(1) {
        if(Transmitter.IsCmdQueueEmpty()) chThdYield();  // No commands - do not waste time
        else Transmitter.DispatchCmd();
    }
    return 0;
}

void Transmitter_t::Init() {
    // Init cmd buffer
    for(uint8_t i=0; i<SB_CMDBUF_SZ; i++) CmdBuf[i].CmdType = CMD_NONE;
    PCmdRead = CmdBuf;
    PCmdWrite = CmdBuf;
    SBUartInit();
    // Create and start thread
    PThread = chThdCreateStatic(waTxThread, sizeof(waTxThread), NORMALPRIO, TxThread, NULL);
}

// =============================== Receiver ====================================
void Rcvr_t::UnpackByte(uint8_t b) {
    // Process the byte: remove start of pkt, unpack byte
    if(WaitingStart) {
        if(b == 0xEE) WaitingStart = false; // Wait for correct pkt start
    }
    else {
        if(WasEE) {
            WasEE = false;
            // EE EE means EE, EE FE means FE
            if((b == 0xEE) or (b == 0xFE)) SortByte(b);
            else Reset();  // EE xx means error
        }
        else { // Was not EE
            if(b == 0xEE) WasEE = true;
            else if(b == 0xFE) EndOfPkt();
            else SortByte(b);
        }
    } // if start
}

//void Rcvr_t::SortByte(uint8_t b) {
//    if(PFeeder == NULL) {   // Iterate them all
//        for(uint8_t i=0; i<FEEDER_CNT; i++) {
//            if(PFeeders[i]->FeedStart(b) == frvOk) {
//                PFeeder = PFeeders[i];
//                return;
//            }
//        }
//        // Noone agreed
//        Reset();
//    }
//    else if(PFeeder->FeedData(b) == frvNoMore) Reset();
//}



