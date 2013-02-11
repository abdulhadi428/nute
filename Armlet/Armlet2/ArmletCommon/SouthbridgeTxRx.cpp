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
Rcvr_t Rcvr;

// =============================== Transmitter =================================
// ==== Cmd dispatching ====
void Transmitter_t::DispatchCmd(SBCmd_t *PCmd) {
    // Build the pkt
    BufCounter = 0;
    AddByte(0xEE);              // Start of pkt
    PackByte(PCmd->CmdType);    // Msg type
    PackWord(PCmd->DataSz);     // Size of unpacked data
    // Cmd's data
    PArray = (uint8_t*)(PCmd->Ptr);
    ArraySz = PCmd->DataSz;
    PackArray();
    Transmit();                 // Transmit builded pkt
    // Check if pkt did not fit in buf and additional data sennding required
    while(ArraySz) {
        BufCounter = 0;
        AddByte(0xEE);
        PackByte(CMD_ADDITIONAL_DATA);
        PackArray();
        Transmit();             // Transmit builded pkt
    }
    // Signal that Cmd is dispatched
    PCmd->CmdType = CMD_DISPATCHED;
}
/*
 * Packs single byte, tries put result to buf. If buf overflow could occur,
 * returns 1 and does nothing with buffer. If succeded, returns 0 and increases
 * BufCounter.
 */
uint8_t Transmitter_t::PackByte(uint8_t Byte) {
    if(Byte == 0xEE) {
        if(BufCounter < SB_UARTBUF_SZ-1) {  // Check if enough place for two bytes
            TxBuf[BufCounter++] = 0xEE;
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
    //dmaWaitCompletion(SB_DMA_STREAM); // Polling
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
static WORKING_AREA(waTxThread, 128);
static msg_t TxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("BridgeTx");
    msg_t Rpl=0;
    while(1) {
        if(chMBFetch(&Transmitter.CmdMailbox, &Rpl, TIME_INFINITE) == RDY_OK) {
            Transmitter.DispatchCmd((SBCmd_t*)Rpl);
        }
    }
    return 0;
}

void Transmitter_t::AddCmd(SBCmd_t *PCmd) {
    while(1) {
        chSemWait(&CmdSem); // Wait here if other thread is spinning below
        // Try to allocate Cmd slot
        for(uint8_t i=0; i<SB_CMDBUF_SZ; i++) {
            if(CmdBuf[i].CmdType == CMD_DISPATCHED) {
                memcpy(&CmdBuf[i], PCmd, SB_CMD_SZ);    // Copy cmd to cmd buffer
                chMBPost(&CmdMailbox, (msg_t)&CmdBuf[i], TIME_INFINITE);    // Send cmd pointer
                chSemSignal(&CmdSem);   // Allow others to spin too
                return;
            }
        } // for
    } // while
}

void Transmitter_t::Init() {
    // Init cmd queue
    chSemInit(&CmdSem, 1);
    chMBInit(&CmdMailbox, Msgs, SB_CMDBUF_SZ);
    for(uint8_t i=0; i<SB_CMDBUF_SZ; i++) CmdBuf[i].CmdType = CMD_DISPATCHED;
    // Create and start thread
    PThread = chThdCreateStatic(waTxThread, sizeof(waTxThread), NORMALPRIO, TxThread, NULL);
}

// =============================== Receiver ====================================
static WORKING_AREA(waRxThread, 128);
static msg_t RxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("BridgeRx");
    msg_t Msg;
    while(1) {
        // Fetch byte from queue
        Msg = chIQGetTimeout(&Rcvr.iqueue, TIME_INFINITE);
        if(Msg >= Q_OK) Rcvr.ProcessByte((uint8_t)Msg);
    }
    return 0;
}

// UART RX irq
extern "C" {
CH_IRQ_HANDLER(SB_UART_RX_IRQ) {
    CH_IRQ_PROLOGUE();
    chSysLockFromIsr();
    uint8_t b = SB_UART_RX_REG;
    // Put byte to queue
    chIQPutI(&Rcvr.iqueue, b);
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}
} // extern C

void Rcvr_t::Init() {
    chIQInit(&iqueue, RxBuf, SB_UARTBUF_SZ, NULL, NULL);
    Reset();
    // Create and start threads
    chThdCreateStatic(waRxThread, sizeof(waRxThread), NORMALPRIO, RxThread, NULL);
}

void Rcvr_t::ProcessByte(uint8_t b) {
    if(RState == rsStart) {
        if(b == 0xEE) RState = rsMsgType; // Wait for correct pkt start
    }
    else {
        if(WasEE) {
            WasEE = false;
            if(b == 0xEE) SortByte(b);  // EE EE means EE
            else Reset();               // EE xx means error
        }
        else { // Was not EE
            if(b == 0xEE) WasEE = true;
            else SortByte(b);
        }
    } // if start
}

void Rcvr_t::SortByte(uint8_t b) {
    switch(RState) {
        case rsMsgType:
            IMsgType = b;
            if(IMsgType == CMD_ADDITIONAL_DATA) {
                if(PFeeder == NULL) Reset();
                else RState = rsData;
            }
            else RState = rsLength1;
            break;

        case rsLength1: // lower byte of length
            RState = rsLength2;
            ILength = b;
            break;

        case rsLength2: // high byte of length
            *(((uint8_t*)&ILength)+1) = b;
            // Find Feeder
            for(uint8_t i=0; i<FeederCnt; i++) {
                if(PFeeders[i]->FeedStart(IMsgType) == frvOk) {
                    PFeeder = PFeeders[i];
                    break; // get out of for
                }
            } // for
            if(PFeeder != 0) RState = rsData;   // was found
            else Reset();                       // was not found
            break;

        case rsData:
            ILength--;
            if((PFeeder->FeedData(b) == frvNoMore) or (ILength == 0)) { // This byte was last one
                PFeeder->FeederEndPkt();
                Reset();
            }
            break;

        default: Reset(); break;
    } // switch
}

void Rcvr_t::EndOfPkt() {
    if(PFeeder == NULL) Reset();
    else PFeeder->FeederEndPkt();
}
