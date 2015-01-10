/*
 * pn.cpp
 *
 *  Created on: 09 џэт. 2015 у.
 *      Author: Kreyl
 */

#include "pn.h"
#include "ch.h"
#include "cmd_uart.h"
#include "evt_mask.h"

PN532_t Pn;

extern "C" {
    void PnDmaTxCompIrq(void *p, uint32_t flags);
    void PnDmaRxCompIrq(void *p, uint32_t flags);
}

// Thread
static WORKING_AREA(waPnThread, 256);
__attribute__ ((__noreturn__))
static void PnThread(void *arg) {
    chRegSetThreadName("PnTask");
    Pn.ITask();
}

void PN532_t::Init() {
    // ==== GPIO ====
    PinSetupOut(PN_GPIO, PN_RST_PIN, omPushPull, pudNone);
    PinSetupOut(PN_NSS_GPIO, PN_NSS_PIN, omPushPull, pudNone);
    PinSetupIn(PN_IRQ_GPIO, PN_IRQ_PIN, pudNone);
    PinSetupAlterFunc(PN_GPIO, PN_SCK_PIN, omPushPull, pudNone, AF5);
    PinSetupAlterFunc(PN_GPIO, PN_MISO_PIN, omPushPull, pudPullDown, AF5);
    PinSetupAlterFunc(PN_GPIO, PN_MOSI_PIN, omPushPull, pudNone, AF5);
    IRstLo();
    INssHi();
    // ==== SPI ====    LSB first, master, ClkLowIdle, FirstEdge, Baudrate=f/2
    ISpi.Setup(PN_SPI, boLSB, cpolIdleLow, cphaFirstEdge, sbFdiv8);
//    ISpi.DisableIrqRxNE();
//    ISpi.DisableIrqTxE();
    ISpi.Enable();
//    ISpi.SetupIrq(IRQ_PRIO_MEDIUM);
    // ==== DMA ====
    // Tx
    dmaStreamAllocate(PN_TX_DMA, IRQ_PRIO_MEDIUM, PnDmaTxCompIrq, NULL);
    dmaStreamSetPeripheral(PN_TX_DMA, &PN_SPI->DR);
    dmaStreamSetMode      (PN_TX_DMA, PN_TX_DMA_MODE_NOIRQ);
    // Rx
    dmaStreamAllocate(PN_RX_DMA, IRQ_PRIO_MEDIUM, PnDmaRxCompIrq, NULL);
    dmaStreamSetPeripheral(PN_RX_DMA, &PN_SPI->DR);
    dmaStreamSetMode      (PN_RX_DMA, PN_RX_DMA_MODE);
    // ==== IRQ ====
    IIrqPin.Setup(PN_IRQ_GPIO, PN_IRQ_PIN, ttFalling);
//    IIrqPin.EnableIrq(IRQ_PRIO_LOW);
    // ==== Variables ====
    State = psSetup;
//    Card.State = csCardOut;
    PThd = chThdCreateStatic(waPnThread, sizeof(waPnThread), NORMALPRIO, (tfunc_t)PnThread, NULL);
    Uart.Printf("\rPnHWSet");
}

__attribute__ ((__noreturn__))
void PN532_t::ITask() {
    while(true) {
        switch (State) {
            case psConfigured:
                chThdSleepMilliseconds(999);
                break;

            case psSetup:
                IReset();
                ServiceCmd(PN_CMD_GET_FIRMWARE_VERSION, WITHOUT_DATA);
                ServiceCmd(PN_CMD_GET_FIRMWARE_VERSION, WITHOUT_DATA);
//                ServiceCmd(PN_CMD_SAM_CONFIGURATION, 1, 0x01);
//                ServiceCmd(PN_CMD_RF_CONFIGURATION, 4, 0x05, 0x02, 0x01, 0x05);
//                ServiceCmd(PN_CMD_RF_CONFIGURATION, 4, 0x02, 0x00, 0x0B, 0x10);
                State = psConfigured;
                Uart.Printf("\rPnSet");
                break;

            case psOff:
                chSysLock();
                chSchGoSleepS(THD_STATE_SUSPENDED);
                chSysUnlock();
                break;
        } // switch
    } // while true
}

void PN532_t::IReset() {
//    FirstCmd = true;
    IRstLo();
    chThdSleepMilliseconds(99); // FIXME: edit timeout
    IRstHi();
    chThdSleepMilliseconds(99); // FIXME: edit timeout
}

uint8_t PN532_t::ServiceCmd(uint8_t CmdID, uint32_t ADataLength, ...) {
    uint8_t Rslt = OK;
    *PSeqType = PN_PRE_DATA_WRITE;
    // Prologue
    PrologueExt->Preamble = 0x00;     // Always
    PrologueExt->SoP0     = 0x00;     // Always
    PrologueExt->SoP1     = 0xFF;     // Always
    PrologueExt->NPLC     = 0xFF;     // Always
    PrologueExt->NPL      = 0xFF;     // Always
    // Length = 1 (TFI) + 1 (CMD == PD0) + ADataLength
    ILength = 1 + 1 + ADataLength;
    PrologueExt->LengthHi = (uint8_t)((ILength >> 8) & 0xFF);
    PrologueExt->LengthLo = (uint8_t)( ILength       & 0xFF);
    PrologueExt->CalcLCS();           // LCS + LENGTH == 0
    // Data
    PData = &IBuf[PN_DATA_EXT_INDX];  // Beginning of TFI+Data
    *PData++ = PN_FRAME_TFI_TRANSMIT;
    *PData++ = CmdID;
    if(ADataLength != 0) {              // If data present copy it to ComboData Buffer
        va_list Arg;                    // Init Agr
        va_start(Arg, ADataLength);     // Set pointer to last argument
        for(uint32_t i=0; i<ADataLength; i++) *PData++ = (uint8_t)va_arg(Arg, int);
        va_end(Arg);
    }
    // Epilogue
    WriteEpilogue(ILength);
    // ==== Transmit frame ====
    Uart.Printf("\r>> %A   ", IBuf, PN_TX_SZ(ILength), ' ');
    INssLo();
    ITransmit(IBuf, PN_TX_SZ(ILength));
    INssHi();

    // ======= Receive =======
    if((Rslt = ReceiveAck()) != OK) return Rslt;
    return ReceiveData();
}

uint8_t PN532_t::ReceiveAck() {
    uint8_t Rslt = OK;
    if((Rslt = WaitReplyReady(PN_ACK_TIMEOUT)) != OK) return Rslt;
    PnAckNack_t *PAckNack = (PnAckNack_t*)IBuf;
    *PSeqType = PN_PRE_DATA_READ;
    INssLo();
    ITransmit(PSeqType, 1);
    ISpi.ReadDummy();   // remove byte red during transmitting
    IReceive(PAckNack, PN_ACK_NACK_LENGTH);   // Receive ACK
    INssHi();
    Rslt = (*PAckNack == PnPktAck)? OK : FAILURE;
    Uart.Printf("\r<< %A   ", PAckNack, PN_ACK_NACK_LENGTH, ' ');
    Uart.Printf("\rRslt=%u", Rslt);
    return Rslt;
}

uint8_t PN532_t::ReceiveData() {
    uint8_t Rslt = OK;
    if((Rslt = WaitReplyReady(PN_DATA_TIMEOUT)) != OK) return Rslt;
    *PSeqType = PN_PRE_DATA_READ;
    INssLo();
    ITransmit(PSeqType, 1);
    ISpi.ReadDummy();
    // Receive reply's prologue
    IReceive(Prologue, PROLOGUE_SZ);
    Uart.Printf("\r<< %A", Prologue, PROLOGUE_SZ, ' ');
    INssHi();
    return Rslt;
}

void PN532_t::ITransmit(void *Ptr, uint32_t ALength) {
    chSysLock();
    ISpi.EnableTxDma();
    dmaStreamSetMemory0(PN_TX_DMA, Ptr);
    dmaStreamSetTransactionSize(PN_TX_DMA, ALength);
    dmaStreamSetMode(PN_TX_DMA, PN_TX_DMA_MODE_IRQ);
    dmaStreamEnable(PN_TX_DMA);
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chSysUnlock();
    ISpi.WaitBsyLo();
}

void PN532_t::IReceive(void *Ptr, uint32_t ALength) {
//    uint8_t Dummy;
//    if(Ptr == nullptr) Ptr = &Dummy;
    chSysLock();
    ISpi.EnableRxDma();
    dmaStreamSetMemory0(PN_RX_DMA, Ptr);
    dmaStreamSetTransactionSize(PN_RX_DMA, ALength);
    dmaStreamSetMode(PN_RX_DMA, PN_RX_DMA_MODE);
    dmaStreamEnable(PN_RX_DMA);
    ISpi.EnableTxDma();
    dmaStreamSetMemory0(PN_TX_DMA, NULL);
    dmaStreamSetTransactionSize(PN_TX_DMA, ALength);
    dmaStreamSetMode(PN_TX_DMA, PN_TX_DMA_MODE_NOIRQ);
    dmaStreamEnable(PN_TX_DMA);
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chSysUnlock();
    ISpi.WaitBsyLo();
}

uint8_t PN532_t::WaitReplyReady(uint32_t ATimeout) {
    // Enable IRQ and wait event
    IIrqPin.EnableIrq(IRQ_PRIO_MEDIUM);
    if(chEvtWaitOneTimeout(EVTMASK_PN_NEW_PKT, MS2ST(ATimeout)) == 0) {
        Uart.Printf("\rTimeout");
        return TIMEOUT;
    }
    else return OK;
}

#if 1 // ========================= IRQs ========================================
//void PN532_t::IrqSpiHandler() {
//    ISpi.DisableIrqTxE();
//    if(PThd->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThd);
//}

void PN532_t::IrqPinHandler() { // Interrupt caused by Low level on IRQ_Pin
//    if(Card.State == csBusy) Uart.Printf("RplRdy\r");
    IIrqPin.CleanIrqFlag();     // Clear IRQ Pending Bit
    IIrqPin.DisableIrq();       // Disable IRQ
    chEvtSignalI(PThd, EVTMASK_PN_NEW_PKT);
}

extern "C" {
//CH_IRQ_HANDLER(SPI1_IRQHandler) {
//    CH_IRQ_PROLOGUE();
//    chSysLockFromIsr();
//    Pn.IrqSpiHandler();
//    chSysUnlockFromIsr();
//    CH_IRQ_EPILOGUE();
//}

CH_IRQ_HANDLER(PN_IRQ_HANDLER) {
    CH_IRQ_PROLOGUE();
    chSysLockFromIsr();
    Pn.IrqPinHandler();
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}

// DMA transmission complete
void PnDmaTxCompIrq(void *p, uint32_t flags) {
    dmaStreamDisable(PN_TX_DMA);    // Disable DMA
    Pn.ISpi.DisableTxDma();         // Disable SPI DMA
//    Pn.ISpi.EnableIrqTxE();         // Wait TxBuffer Empty In Interrupt while Thread suspended
    chSysLockFromIsr();
    if(Pn.PThd->p_state == THD_STATE_SUSPENDED) chSchReadyI(Pn.PThd);
    chSysUnlockFromIsr();
}
// DMA reception complete
void PnDmaRxCompIrq(void *p, uint32_t flags) {
    dmaStreamDisable(PN_RX_DMA); // Disable DMA
    Pn.ISpi.DisableRxDma();
    chSysLockFromIsr();
    if(Pn.PThd->p_state == THD_STATE_SUSPENDED) chSchReadyI(Pn.PThd);
    chSysUnlockFromIsr();
}
} // extern C
#endif
