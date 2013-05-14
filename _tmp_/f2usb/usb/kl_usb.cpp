/*
 * kl_usb.cpp
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#include "kl_usb.h"
#include "descriptors.h"

#define TRDT        3   // AHB clock = 24, => TRDT = 24/48 +1

// Fetches a 16 bits word value from an USB message.
#define USB_FETCH_WORD(p) ((uint16_t)*(p) | ((uint16_t)*((p) + 1) << 8))

Usb_t Usb;

// Callbacks
static void Ep0InCallback();
static void Ep0OutCallback();

// Thread
static WORKING_AREA(waUsbThd, 128);
static msg_t UsbThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Usb");
    Usb.ITask();
    return 0;
}
void Usb_t::ITask() {
    while(1) {
        chSysLock();
//        if(((OTG_FS->GINTSTS & GINTSTS_RXFLVL) == 0) and !Ep[0].IsTxPending) {
        if(!NewSetupPkt) {
//            OTG_FS->GINTMSK |= GINTMSK_RXFLVLM;     // Enable RX
            chSchGoSleepS(THD_STATE_SUSPENDED);     // Will wake up by IRQ
        }
        chSysUnlock();

        // Handle SETUP
//        if(NewSetupPkt) {
//            NewSetupPkt = false;
//            Ep0SetupHandler();
//        }

        // Handle RX
//        while(OTG_FS->GINTSTS & GINTSTS_RXFLVL) RxHandler();

        // Handle TX
//        for(uint8_t i=0; i<USB_EP_CNT; i++) {
//            if(Ep[i].IsTxPending) {
//                // Block IRQ for not to interfere in FIFO filling operation
//                chSysLock();
//                OTG_FS->GAHBCFG &= ~GAHBCFG_GINTMSK;
//                Ep[i].IsTxPending = false;
//                //chSysUnlock();
//                bool Done = Ep[i].TxFifoHandler();
//                Uart.Printf("Done: %u\r\n", Done);
//                // Unblock IRQs back
//                //chSysLock();
//                OTG_FS->GAHBCFG |= GAHBCFG_GINTMSK;
//                if(!Done) OTG_FS->DIEPEMPMSK |= (1<<i);
//                chSysUnlock();
//            } // if pending
//        } // for
    } // while 1
}


void Usb_t::Init() {
    // GPIO
    PinSetupAlterFunc(GPIOA, 11, omOpenDrain, pudNone, AF10);
    PinSetupAlterFunc(GPIOA, 12, omOpenDrain, pudNone, AF10);
    PinSetupIn(GPIOA, 9, pudPullDown);
    // OTG FS clock enable and reset
    rccEnableOTG_FS(FALSE);
    rccResetOTG_FS();
    // Enable IRQ
    nvicEnableVector(STM32_OTG1_NUMBER, CORTEX_PRIORITY_MASK(IRQ_PRIO_LOW));

    // Thread
    PThread = chThdCreateStatic(waUsbThd, sizeof(waUsbThd), LOWPRIO, UsbThread, NULL);

    // ==== OTG init ====
    // Forced device mode, USB turn-around time = TRDT_VALUE, Full Speed 1.1 PHY, 0 tuning
    OTG_FS->GUSBCFG = GUSBCFG_FDMOD | GUSBCFG_TRDT(TRDT) | GUSBCFG_PHYSEL | 0;
    OTG_FS->DCFG = 0x02200000 | DCFG_DSPD_FS11; // Full-speed (other options are not available, though)
    OTG_FS->PCGCCTL = 0;    // Nothing is stopped or gated
    OTG_FS->GCCFG = GCCFG_VBUSASEN | GCCFG_VBUSBSEN | GCCFG_PWRDWN /*| GCCFG_NOVBUSSENS*/;

    // Core reset and delay of at least 3 PHY cycles
    OTG_FS->GRSTCTL = GRSTCTL_CSRST;
    while((OTG_FS->GRSTCTL & GRSTCTL_CSRST) != 0);
    __NOP();
    __NOP();
    // Wait AHB idle condition
    while((OTG_FS->GRSTCTL & GRSTCTL_AHBIDL) == 0);
    __NOP();
    __NOP();

    OTG_FS->GAHBCFG = 0;    // Interrupts on TXFIFOs half empty, mask interrupts

    // ==== Endpoints ====
    for(uint8_t i=0; i<USB_EP_CNT; i++) Ep[i].SelfN = i;
    // Ep0
    //OTG_FS->ie[0].DIEPCTL = 0x11;  // size = 8
    Ep[0].InMaxSz  = EP0_SZ;
    Ep[0].OutMaxSz = EP0_SZ;
    Ep[0].cbIn = Ep0InCallback;
    Ep[0].cbOut = Ep0OutCallback;

    // Clear all pending Device Interrupts, only the USB Reset interrupt is required initially
    OTG_FS->DIEPMSK  = 0;
    OTG_FS->DOEPMSK  = 0;
    OTG_FS->DAINTMSK = 0;
    OTG_FS->GINTMSK  = GINTMSK_ENUMDNEM | GINTMSK_USBRSTM /*| GINTMSK_USBSUSPM | GINTMSK_ESUSPM  |*/;
    OTG_FS->GINTSTS  = 0xFFFFFFFF;      // Clear all pending IRQs, if any
    OTG_FS->GAHBCFG |= GAHBCFG_GINTMSK; // Global interrupts enable
}

void Usb_t::RxFifoFlush() {
    OTG_FS->GRSTCTL = GRSTCTL_RXFFLSH;
    while((OTG_FS->GRSTCTL & GRSTCTL_RXFFLSH) != 0);
    // Wait for 3 PHY Clocks
    __NOP();
    __NOP();
}
void Usb_t::TxFifoFlush() {
//    Uart.Printf("TxFlush1: %X %X\r\n", OTG_FS->GRSTCTL, OTG_FS->GINTSTS);
//    if(!(OTG_FS->GINTSTS & GINTSTS_GINAKEFF)) OTG_FS->DCTL |= DCTL_SGINAK;
//    while(!(OTG_FS->GINTSTS & GINTSTS_GINAKEFF));
//    Uart.Printf("TxFlush2: %X %X\r\n", OTG_FS->GRSTCTL, OTG_FS->GINTSTS);

    OTG_FS->GRSTCTL = GRSTCTL_TXFNUM(0) | GRSTCTL_TXFFLSH;
    while((OTG_FS->GRSTCTL & GRSTCTL_TXFFLSH) != 0);
    // Wait for 3 PHY Clocks
    __NOP();
    __NOP();
//    OTG_FS->DCTL |= DCTL_CGINAK;
}

void Usb_t::ReadFifo(uint8_t *PBuf, volatile uint32_t *PFifo, uint32_t Cnt) {
    Cnt = (Cnt + 3) / 4;    // Round cnt to 4-byte value
    while(Cnt) {
        uint32_t w = *PFifo;
        *((uint32_t*)PBuf) = w;
        PBuf += 4;
        Cnt--;
    }
}
void Usb_t::WriteFifo(volatile uint32_t *PFifo, uint8_t *PBuf, uint32_t Cnt) {
    Cnt = (Cnt + 3) / 4;    // Round cnt to 4-byte value
    while(Cnt) {
        uint32_t w = *((uint32_t*)PBuf);
        *PFifo = w;
        //Uart.Printf("< %04X\r\n", w);
        PBuf += 4;
        Cnt--;
    }
}

// ============================== Endpoints ====================================
void Usb_t::Ep0SetupHandler() {
    Uart.Printf("Ep0 setup\r\n");
    uint8_t *Ptr;
    uint32_t Len;
    // Decode request
    if(StdRequestHandler(&Ptr, &Len) == OK) {
        //Uart.Printf("< %A\r\n", Ptr, Len, ' ');
        // The transfer size cannot exceed the specified amount
        TRIM_VALUE(Len, SetupReq.wLength);
        if((SetupReq.bmRequestType & USB_RTYPE_DIR_MASK) == USB_RTYPE_DIR_DEV2HOST) {
            // IN phase
            if(Len > 0) Ep[0].PrepareTransmit(Ptr, Len);  // Start transmit phase
            else        Ep[0].Receive(0);          // No transmit phase, receive zero sized status pkt
        }
        else {
            // OUT phase
            if(Len > 0) Ep[0].Receive(Len);        // Start receive phase
            else        Ep[0].PrepareTransmit(NULL, 0);   // No receive state, send zero sized status pkt
        }
    }
    else { // If error, stall EP
        Ep[0].StallIn();
        Ep[0].StallOut();
    }
}

static const uint8_t ZeroStatus[] = {0x00, 0x00};
uint8_t Usb_t::StdRequestHandler(uint8_t **Ptr, uint32_t *PLen) {
    uint8_t Recipient = SetupReq.bmRequestType & USB_RTYPE_RECIPIENT_MASK;
    *Ptr = NULL;
    *PLen = 0;
    if(Recipient == USB_RTYPE_RECIPIENT_DEVICE) {
        //Uart.Printf("Dev\r\n");
        switch(SetupReq.bRequest) {
            case USB_REQ_GET_STATUS:    // Just return the current status word
                Uart.Printf("GetStatus\r\n");
                *Ptr = (uint8_t*)ZeroStatus;  // Remote wakeup = 0, selfpowered = 0
                *PLen = 2;
                return OK;
                break;
            case USB_REQ_SET_ADDRESS:
                Uart.Printf("SetAddr\r\n");
                SetAddress(SetupReq.wValue);
                return OK;
                break;
            case USB_REQ_GET_DESCRIPTOR:
          //      Uart.Printf("GetDesc\r\n");
                *PLen = GetDescriptor(SetupReq.Buf[3], SetupReq.Buf[2], (const uint8_t**)Ptr);
                if(*PLen != 0) return OK;
                break;
            case USB_REQ_GET_CONFIGURATION:
                Uart.Printf("GetCnf\r\n");
                *Ptr = &Configuration;
                *PLen = 1;
                return OK;
                break;
            case USB_REQ_SET_CONFIGURATION:
                Uart.Printf("SetCnf\r\n");
                Configuration = SetupReq.wValue;
                //if(Configuration == 0)USB_SELECTED else USB_ACTIVE
                return OK;
                break;
            default: break;
        } // switch
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_INTERFACE) {
        if(SetupReq.bRequest == USB_REQ_GET_STATUS) {
            Uart.Printf("InterfGetSta\r\n");
            *Ptr = (uint8_t*)ZeroStatus;
            *PLen = 2;
            return OK;
        }
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_ENDPOINT) {
        Uart.Printf("Ep\r\n");
        switch(SetupReq.bRequest) {
            case USB_REQ_SYNCH_FRAME:
                *Ptr = (uint8_t*)ZeroStatus;
                *PLen = 2;
                return OK;
                break;
            case USB_REQ_GET_STATUS:
                break;
        }
    }
    return FAILURE;
}

void Endpoint_t::PrepareTransmit(uint8_t *Ptr, uint32_t Len) {
    //Uart.Printf("TX: %A\r\n", Ptr, Len, ' ');
    Uart.Printf("PrepTX L=%u\r\n", Len);
    PTxBuf = Ptr;
    TxSize = Len;
    uint32_t Dieptsiz = OTG_FS->ie[SelfN].DIEPTSIZ;
    Dieptsiz &= ~((3<<19) | 0x7F);
    uint32_t DieptCtl = OTG_FS->ie[SelfN].DIEPCTL;

    // Prepare transmission
    if(Len == 0) {  // Send zero pkt
        //OTG_FS->ie[SelfN].DIEPTSIZ = DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
        Dieptsiz |= DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
    }
    else {
        uint32_t Cnt = (Len + InMaxSz - 1) / InMaxSz;
        //OTG_FS->ie[SelfN].DIEPTSIZ = DIEPTSIZ_PKTCNT(Cnt) | DIEPTSIZ_XFRSIZ(Len);
        //Uart.Printf("Cnt: %u\r\n", Cnt);
        Dieptsiz |= DIEPTSIZ_PKTCNT(Cnt) | DIEPTSIZ_XFRSIZ(Len);
    }
    //Uart.Printf("Dieptsiz: %X\r\n", Dieptsiz);
    DieptCtl |= DIEPCTL_EPENA | DIEPCTL_CNAK;

    // Start transfer
    //chSysLock();
//    if(IsTransmitting) return;  // do not start if transmitting already
    IsTransmitting = true;
    OTG_FS->ie[SelfN].DIEPTSIZ = Dieptsiz;
    OTG_FS->ie[SelfN].DIEPCTL  = DieptCtl;
    Uart.Printf("cnak1 %X\r\n", OTG_FS->ie[SelfN].DIEPINT);

    EnableInFifoEmptyIRQ();
    //chSysUnlock();
}

void Endpoint_t::Receive(uint32_t Len) {
    // Prepare reception
    uint32_t Cnt = (Len + OutMaxSz - 1) / OutMaxSz;
    OTG_FS->oe[SelfN].DOEPTSIZ |= DOEPTSIZ_PKTCNT(Cnt) | DOEPTSIZ_XFRSIZ(OutMaxSz);
    // Start reception
    chSysLockFromIsr();
    if(IsReceiving) return;
    IsReceiving = true;
    OTG_FS->oe[SelfN].DOEPCTL |= DOEPCTL_CNAK;
    chSysUnlockFromIsr();
}

//static inline void Push(volatile uint32_t *fifop, uint8_t *Ptr, uint32_t

bool Endpoint_t::TxFifoHandler() {
    // The TXFIFO is filled until there are space and data to be transmitted
    while(TxSize) {
        uint32_t n = MIN(TxSize, InMaxSz);  // Number of bytes to transmit now
        // Check if enough space in FIFO
        if(((OTG_FS->ie[SelfN].DTXFSTS & DTXFSTS_INEPTFSAV_MASK) * 4) < n) return false;
        TxSize -= n;
        // Write to FIFO
        n = (n + 3) / 4;    // Number of 32bit words to write
        while(n) {
            uint32_t t = *((uint32_t*)PTxBuf);
            Uart.Printf("< %08X\r\n", t);
            *(OTG_FS->FIFO[SelfN]) = t;
            PTxBuf += 4;
            n--;
        }
    }
    return true;    // TxSize == 0
}

// ================================ IRQ ========================================
void Usb_t::IHandleIrq() {
    uint32_t sts, src;
    // Get irq flag
    sts = OTG_FS->GINTSTS & OTG_FS->GINTMSK;
    OTG_FS->GINTSTS = sts;
    Uart.Printf("u %X %u\r\n", sts, chTimeNow());

    // Reset
    if(sts & GINTSTS_USBRST) IReset();
    // Enumeration done
    if (sts & GINTSTS_ENUMDNE) {
        (void)OTG_FS->DSTS;
    }

    // RX FIFO not empty handling
    if (sts & GINTSTS_RXFLVL) {
        Uart.Printf("rx\r\n");
        IRxHandler();
//        chSysLockFromIsr();
//        OTG_FS->GINTMSK &= ~GINTMSK_RXFLVLM;    // Disable RX irq until current is processed
//        if(PThread->p_state == THD_STATE_SUSPENDED) chThdResumeI(PThread);
//        chSysUnlockFromIsr();
    }

    // OUT & IN event handling
    if (sts & GINTSTS_IEPINT) {
        src = OTG_FS->DAINT;
        if(src & (1 << 0)) IEpInHandler(0);
        if(src & (1 << 1)) IEpInHandler(1);
        if(src & (1 << 2)) IEpInHandler(2);
        if(src & (1 << 3)) IEpInHandler(3);
    }
    if (sts & GINTSTS_OEPINT) {
        src = OTG_FS->DAINT;
        if(src & (1 << 16)) IEpOutHandler(0);
        if(src & (1 << 17)) IEpOutHandler(1);
        if(src & (1 << 18)) IEpOutHandler(2);
        if(src & (1 << 19)) IEpOutHandler(3);
    }
}

void Usb_t::IReset() {
    Uart.Printf("========= Rst =========\r\n");
    TxFifoFlush();
    NewSetupPkt = false;
    // Set all EPs in NAK mode and clear Irqs
//    for(uint8_t i=0; i < USB_EP_CNT; i++) {
//        OTG_FS->ie[i].DIEPCTL = DIEPCTL_SNAK;
//        OTG_FS->oe[i].DOEPCTL = DOEPCTL_SNAK;
//        OTG_FS->ie[i].DIEPINT = 0xFF;
//        OTG_FS->oe[i].DOEPINT = 0xFF;
//        Ep[i].IsReceiving = false;
//        Ep[i].IsTransmitting = false;
//        Ep[i].IsTxPending = false;
//    }

    // Disable and clear all EP irqs
    OTG_FS->DAINT = 0xFFFFFFFF;
    OTG_FS->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);
    OTG_FS->DIEPEMPMSK &= 0xFFFF0000;
    MemAllocator.Reset();

    // RX FIFO init, the address is always zero
    OTG_FS->GRXFSIZ = RX_FIFO_SZ;
    RxFifoFlush();
    // Reset device address to zero
    OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(0);

    // Enable EP-related irqs
    OTG_FS->GINTMSK |= GINTMSK_RXFLVLM | GINTMSK_OEPM  | GINTMSK_IEPM;
    OTG_FS->DIEPMSK  = DIEPMSK_XFRCM;
    OTG_FS->DOEPMSK  = DOEPMSK_STUPM   | DOEPMSK_XFRCM;

    // Init EP0
#if EP0_SZ == 8
#define EPSZBITS    0x03    // Last two bits = 11 => 8 bytes
#elif EP0_SZ == 64
#define EPSZBITS    0
#endif
    OTG_FS->ie[0].DIEPINT |= 0x5B;  // }
    OTG_FS->oe[0].DOEPINT |= 0x5B;  // } Clear interrupts
    OTG_FS->oe[0].DOEPTSIZ = DOEPTSIZ_STUPCNT(3);
    OTG_FS->oe[0].DOEPCTL = EPSZBITS;
    OTG_FS->ie[0].DIEPTSIZ = 0;
    OTG_FS->ie[0].DIEPCTL = DIEPCTL_TXFNUM(0) | EPSZBITS;
    OTG_FS->DIEPTXF0 = DIEPTXF_INEPTXFD(EP0_SZ / 4) | DIEPTXF_INEPTXSA(MemAllocator.Alloc(EP0_SZ / 4));

    Ep[0].SetInNAK();
    //Ep[0].EnableInFifoEmptyIRQ();
}

void Usb_t::IRxHandler() {
    OTG_FS->GINTMSK &= ~GINTMSK_RXFLVLM;    // Disable RX irq until current is processed
    uint32_t sts = OTG_FS->GRXSTSP;         // Get the Status from the top of the FIFO
    uint32_t EpN = (sts & GRXSTSP_EPNUM_MASK);
    uint32_t cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
    switch(sts & GRXSTSP_PKTSTS_MASK) {
        case GRXSTSP_SETUP_COMP:
            Uart.Printf(" setup comp\r\n");
            break;
        case GRXSTSP_SETUP_DATA:
            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            Uart.Printf(" setup %u %u\r\n", cnt, EpN);
            ReadFifo(SetupReq.Buf, OTG_FS->FIFO[0], cnt);
//            Uart.Printf("> %A\r\n", SetupReq.Buf, 8, ' ');
            Uart.Printf("%X %X %X %u %u\r\n", SetupReq.bmRequestType, SetupReq.bRequest, SetupReq.wValue, SetupReq.wIndex, SetupReq.wLength);
            break;
        case GRXSTSP_OUT_DATA:
            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            Uart.Printf(" out %u %u\r\n", cnt, EpN);
//            if(usbp->epc[ep]->out_state->rxqueued) {
//              /* Queue associated.*/
//              otg_fifo_read_to_queue(usbp->otg->FIFO[0],
//                                     usbp->epc[ep]->out_state->mode.queue.rxqueue,
//                                     cnt);
//            }
//            else {
//              otg_fifo_read_to_buffer(usbp->otg->FIFO[0],
//                                      usbp->epc[ep]->out_state->mode.linear.rxbuf,
//                                      cnt,
//                                      usbp->epc[ep]->out_state->rxsize -
//                                      usbp->epc[ep]->out_state->rxcnt);
//              usbp->epc[ep]->out_state->mode.linear.rxbuf += cnt;
//            }
//            usbp->epc[ep]->out_state->rxcnt += cnt;
            break;
        default:
            Uart.Printf(" RX sts: %X\r\n", sts);
            break;
    } // switch
    OTG_FS->GINTMSK |= GINTMSK_RXFLVLM;     // Enable RX irq
}


void Usb_t::IEpInHandler(uint8_t EpN) {
    uint32_t epint = OTG_FS->ie[EpN].DIEPINT;
    OTG_FS->ie[EpN].DIEPINT = 0xFFFFFFFF;   // Reset IRQs
    Uart.Printf("in %X i%X\r\n", EpN, epint);
    // Transmit transfer complete
    if(epint & DIEPINT_XFRC) {
        Uart.Printf("###########\r\n");
        //Ep[EpN].DisableInFifoEmptyIRQ();
//        Ep[EpN].IsTransmitting = false;
        if(Ep[EpN].cbIn != NULL) Ep[EpN].cbIn();
    }
    // TX FIFO empty
    if(epint & DIEPINT_TXFE) {
        Uart.Printf("TXFE\r\n");
        chSysLockFromIsr();
        Ep[EpN].DisableInFifoEmptyIRQ();
        if(Ep[EpN].TxSize == 0) return; // FIXME

        uint32_t len = Ep[EpN].TxSize;
        TRIM_VALUE(len, Ep[EpN].InMaxSz);
        uint32_t len32 = (len + 3) / 4;
        uint32_t SpaceAvailable = (OTG_FS->ie[EpN].DTXFSTS & 0xFFFF);
        Uart.Printf(" SpAv1 %u\r\n", SpaceAvailable);

        while((SpaceAvailable > len32) and (Ep[EpN].TxSize != 0)) {
            len = Ep[EpN].TxSize;
            TRIM_VALUE(len, Ep[EpN].InMaxSz);
            len32 = (len + 3) / 4;
            WriteFifo(OTG_FS->FIFO[EpN], Ep[EpN].PTxBuf, len);
            Ep[EpN].TxSize -= len;
            Ep[EpN].PTxBuf += len;
            SpaceAvailable = (OTG_FS->ie[EpN].DTXFSTS & 0xFFFF);
            Uart.Printf(" SpAv2 %u\r\n", SpaceAvailable);
        }

//        OTG_FS->DCTL |= DCTL_CGINAK;
        Ep[EpN].ClearInNAK();

        if(Ep[EpN].TxSize != 0)
            Ep[EpN].EnableInFifoEmptyIRQ();
//        Ep[EpN].IsTxPending = true;
//        OTG_FS->DIEPEMPMSK &= ~(1 << EpN);  // Disable TxFifoEmpty irq for this EP
//        if(PThread->p_state == THD_STATE_SUSPENDED) chThdResumeI(PThread);
        chSysUnlockFromIsr();
    }
}

void Usb_t::IEpOutHandler(uint8_t EpN) {
    uint32_t epint = OTG_FS->oe[EpN].DOEPINT;
    OTG_FS->oe[EpN].DOEPINT = 0xFFFFFFFF;   // Reset IRQs
    Uart.Printf("out %u: %X\r\n", EpN, epint);
    // Setup packets handling
    if ((epint & DOEPINT_STUP) and (OTG_FS->DOEPMSK & DOEPMSK_STUPM)) {
        chSysLockFromIsr();
//        NewSetupPkt = true;
//        if(PThread->p_state == THD_STATE_SUSPENDED) chThdResumeI(PThread);
        Ep0SetupHandler();
        chSysUnlockFromIsr();
    }
    // Receive transfer complete
    if ((epint & DOEPINT_XFRC) and (OTG_FS->DOEPMSK & DOEPMSK_XFRCM)) {
        Uart.Printf(" XFR cmp\r\n");
        Ep[EpN].IsReceiving = false;
        if(Ep[EpN].cbOut != NULL) Ep[EpN].cbOut();
    }
}


extern "C" {
CH_IRQ_HANDLER(STM32_OTG1_HANDLER) {
    CH_IRQ_PROLOGUE();
    Usb.IHandleIrq();
    CH_IRQ_EPILOGUE();
}
} // extern C

// =============================== Callbacks ===================================
void Ep0InCallback() {
    Uart.Printf("EP0 In\r\n");
}

void Ep0OutCallback() {
    Uart.Printf("EP0 Out\r\n");
}
