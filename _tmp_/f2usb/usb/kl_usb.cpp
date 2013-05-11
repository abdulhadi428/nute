/*
 * kl_usb.cpp
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#include "kl_usb.h"
#include "descriptors.h"

#define TRDT        3   // AHB clock = 24, => TRDT = 24/48 +1

#define EP0_OUT_SZ  0x40
#define EP0_IN_SZ   0x40

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
        if(((OTG_FS->GINTSTS & GINTSTS_RXFLVL) == 0) and !Ep[0].IsTxPending) {
            OTG_FS->GINTMSK |= GINTMSK_RXFLVLM;     // Enable RX
            chSchGoSleepS(THD_STATE_SUSPENDED);     // Will wake up by IRQ
        }
        chSysUnlock();

        // Handle RX
        while(OTG_FS->GINTSTS & GINTSTS_RXFLVL) RxHandler();

        // Handle TX
        for(uint8_t i=0; i<USB_EP_CNT; i++) {
            if(Ep[i].IsTxPending) {
                // Block IRQ for not to interfere in FIFO filling operation
                chSysLock();
                OTG_FS->GAHBCFG &= ~GAHBCFG_GINTMSK;
                Ep[i].IsTxPending = false;
                //chSysUnlock();
                bool Done = Ep[i].TxFifoHandler();
                if(Done) Uart.Printf("Done\r\n");
                // Unblock IRQs back
                //chSysLock();
                OTG_FS->GAHBCFG |= GAHBCFG_GINTMSK;
                if(!Done) OTG_FS->DIEPEMPMSK |= (1<<i);
                chSysUnlock();
            } // if pending
        } // for
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
    Delay_ms(1);
    // Wait AHB idle condition
    while((OTG_FS->GRSTCTL & GRSTCTL_AHBIDL) == 0);

    OTG_FS->GAHBCFG = 0;    // Interrupts on TXFIFOs half empty, mask interrupts

    // ==== Endpoints ====
    for(uint8_t i=0; i<USB_EP_CNT; i++) Ep[i].SelfN = i;
    // Ep0
    OTG_FS->ie[0].DIEPCTL = 0;  // size = 64
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
    while ((OTG_FS->GRSTCTL & GRSTCTL_RXFFLSH) != 0);
    Delay_ms(1);    // Wait for 3 PHY Clocks
}
void Usb_t::TxFifoFlush() {
    OTG_FS->GRSTCTL = GRSTCTL_TXFNUM(0) | GRSTCTL_TXFFLSH;
    while ((OTG_FS->GRSTCTL & GRSTCTL_TXFFLSH) != 0);
    Delay_ms(1);    // Wait for 3 PHY Clocks
}

void Usb_t::RxHandler() {
    uint32_t sts = OTG_FS->GRXSTSP, cnt, ep;
    switch(sts & GRXSTSP_PKTSTS_MASK) {
        case GRXSTSP_SETUP_COMP:
            Uart.Printf(" setup comp\r\n");
            break;
        case GRXSTSP_SETUP_DATA:
            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
            Uart.Printf(" setup %u %u\r\n", cnt, ep);
            if((ep == 0) and (cnt == 8)) ReadToBuf(SetupBuf, OTG_FS->FIFO[0], cnt);
//            Uart.Printf("> %A\r\n", SetupBuf, 8, ' ');
            break;
        case GRXSTSP_OUT_DATA:
            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
            Uart.Printf(" out %u %u\r\n", cnt, ep);
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
        //case GRXSTSP_OUT_GLOBAL_NAK:
        //case GRXSTSP_OUT_COMP:
        default: break;
    } // switch
}

void Usb_t::ReadToBuf(uint8_t *PBuf, volatile uint32_t *PFifo, uint32_t Cnt) {
    Cnt = (Cnt + 3) / 4;    // Round cnt to 4-byte value
    while(Cnt) {
        uint32_t w = *PFifo;
        *((uint32_t*)PBuf) = w;
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
        // The transfer size cannot exceed the specified amount
        uint32_t max = USB_FETCH_WORD(&SetupBuf[6]);
        TRIM_VALUE(Len, max);
        if((SetupBuf[0] & USB_RTYPE_DIR_MASK) == USB_RTYPE_DIR_DEV2HOST) {
            // IN phase
            if(Len > 0) Ep[0].Transmit(Ptr, Len);  // Start transmit phase
            else        Ep[0].Receive(0);          // No transmit phase, receive zero sized status pkt
        }
        else {
            // OUT phase
            if(Len > 0) Ep[0].Receive(Len);        // Start receive phase
            else        Ep[0].Transmit(NULL, 0);   // No receive state, send zero sized status pkt
        }
    }
    else { // If error, stall EP
        Ep[0].StallIn();
        Ep[0].StallOut();
    }
}

static const uint8_t ZeroStatus[] = {0x00, 0x00};
uint8_t Usb_t::StdRequestHandler(uint8_t **Ptr, uint32_t *PLen) {
    uint8_t Recipient = SetupBuf[0] & USB_RTYPE_RECIPIENT_MASK;
    uint8_t bRequest = SetupBuf[1];
    *Ptr = NULL;
    *PLen = 0;
    if(Recipient == USB_RTYPE_RECIPIENT_DEVICE) {
        Uart.Printf("Dev\r\n");
        switch(bRequest) {
            case USB_REQ_GET_STATUS:    // Just return the current status word
                Uart.Printf("GetStatus\r\n");
                *Ptr = (uint8_t*)&IStatus;
                *PLen = 2;
                return OK;
                break;
            case USB_REQ_CLEAR_FEATURE:
                Uart.Printf("ClrFeature\r\n");
                // Only the DEVICE_REMOTE_WAKEUP is handled here, any other feature number is handled as an error
                if(SetupBuf[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
                    IStatus &= ~2;
                    return OK;
                }
                break;
            case USB_REQ_SET_FEATURE:
                Uart.Printf("SetFeature\r\n");
                // Only the DEVICE_REMOTE_WAKEUP is handled here, any other feature number is handled as an error
                if(SetupBuf[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
                    IStatus |= 2;
                    return OK;
                }
                break;
            case USB_REQ_SET_ADDRESS:
                Uart.Printf("SetAddr\r\n");
                SetAddress(SetupBuf[2]);
                return OK;
                break;
            case USB_REQ_GET_DESCRIPTOR:
                Uart.Printf("GetDesc\r\n");
                *PLen = GetDescriptor(SetupBuf[3], SetupBuf[2], (const uint8_t**)Ptr);
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
                Configuration = SetupBuf[2];
                //if(Configuration == 0)USB_SELECTED else USB_ACTIVE
                return OK;
                break;
            default: break;
        } // switch
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_INTERFACE) {
        if(bRequest == USB_REQ_GET_STATUS) {
            Uart.Printf("InterfGetSta\r\n");
            *Ptr = (uint8_t*)ZeroStatus;
            *PLen = 2;
            return OK;
        }
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_ENDPOINT) {
        Uart.Printf("Ep\r\n");
        switch(bRequest) {
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

void Endpoint_t::Transmit(uint8_t *Ptr, uint32_t Len) {
    Uart.Printf("TX1: %A\r\n", Ptr, Len, ' ');
    PTxBuf = Ptr;
    TxSize = Len;
    // Prepare transmission
    if(Len == 0) {  // Send zero pkt
        OTG_FS->ie[SelfN].DIEPTSIZ = DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
    }
    else {
        uint32_t Cnt = (Len + InMaxSz - 1) / InMaxSz;
        OTG_FS->ie[SelfN].DIEPTSIZ = DIEPTSIZ_PKTCNT(Cnt) | DIEPTSIZ_XFRSIZ(Len);
//        Uart.Printf("Cnt: %u\r\n", Cnt);
    }
    // Start transfer
    chSysLockFromIsr();
    if(IsTransmitting) return;  // do not start if transmitting already
    IsTransmitting = true;
    OTG_FS->ie[SelfN].DIEPCTL |= DIEPCTL_EPENA | DIEPCTL_CNAK;
    OTG_FS->DIEPEMPMSK |= DIEPEMPMSK_INEPTXFEM(SelfN);
    chSysUnlockFromIsr();
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
    Uart.Printf("u %X\r\n", sts);

    // Reset
    if(sts & GINTSTS_USBRST) IReset();
    // Enumeration done
    if (sts & GINTSTS_ENUMDNE) {
        (void)OTG_FS->DSTS;
//        uint32_t x = OTG_FS->DSTS;
//        Uart.Printf("ed %X\r\n", x);
    }

    // RX FIFO not empty handling
    if (sts & GINTSTS_RXFLVL) {
        Uart.Printf("rx\r\n");
        chSysLockFromIsr();
        OTG_FS->GINTMSK &= ~GINTMSK_RXFLVLM;    // Disable RX irq until current is processed
        if(PThread->p_state == THD_STATE_SUSPENDED) chThdResumeI(PThread);
        chSysUnlockFromIsr();
    }

    // OUT EP event handling
    src = OTG_FS->DAINT;
    Uart.Printf("src %X\r\n", src);
    if (sts & GINTSTS_IEPINT) {
        if(src & (1 << 0)) IEpInHandler(0);
        if(src & (1 << 1)) IEpInHandler(1);
        if(src & (1 << 2)) IEpInHandler(2);
        if(src & (1 << 3)) IEpInHandler(3);
    }
    if (sts & GINTSTS_OEPINT) {
        if(src & (1 << 16)) IEpOutHandler(0);
        if(src & (1 << 17)) IEpOutHandler(1);
        if(src & (1 << 18)) IEpOutHandler(2);
        if(src & (1 << 19)) IEpOutHandler(3);
    }
}

void Usb_t::IReset() {
    Uart.Printf("rst\r\n");
    TxFifoFlush();
    // Set all EPs in NAK mode and clear Irqs
    for(uint8_t i=0; i <= USB_EP_CNT; i++) {
        OTG_FS->ie[i].DIEPCTL = DIEPCTL_SNAK;
        OTG_FS->oe[i].DOEPCTL = DOEPCTL_SNAK;
        OTG_FS->ie[i].DIEPINT = 0xFF;
        OTG_FS->oe[i].DOEPINT = 0xFF;
    }

    // Disable and clear all EP irqs
    OTG_FS->DAINT = 0xFFFFFFFF;
    OTG_FS->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);
    MemAllocator.Reset();

    // RX FIFO init, the address is always zero
    OTG_FS->GRXFSIZ = RX_FIFO_SZ;
    RxFifoFlush();
    // Reset device address to zero
    OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(0);

    // Enable EP-related irqs
    OTG_FS->GINTMSK |= GINTMSK_RXFLVLM | GINTMSK_OEPM  | GINTMSK_IEPM;
    OTG_FS->DIEPMSK  = DIEPMSK_TOCM    | DIEPMSK_XFRCM;
    OTG_FS->DOEPMSK  = DOEPMSK_STUPM   | DOEPMSK_XFRCM;

    // Init EP0
    OTG_FS->oe[0].DOEPTSIZ = DOEPTSIZ_STUPCNT(3);
    OTG_FS->oe[0].DOEPCTL = 0;  // Last two bits = 00 => 64 bytes
    OTG_FS->ie[0].DIEPTSIZ = 0;
    OTG_FS->ie[0].DIEPCTL = DIEPCTL_TXFNUM(0);  // Last two bits = 00 => 64 bytes
    OTG_FS->DIEPTXF0 = DIEPTXF_INEPTXFD(EP0_IN_SZ / 4) | DIEPTXF_INEPTXSA(MemAllocator.Alloc(EP0_IN_SZ / 4));
}

void Usb_t::IEpInHandler(uint8_t EpN) {
    uint32_t epint = OTG_FS->ie[EpN].DIEPINT;
    OTG_FS->ie[EpN].DIEPINT = 0xFFFFFFFF;   // Reset IRQs
    Uart.Printf("in %X i%X\r\n", EpN, epint);
    // Transmit transfer complete
    if ((epint & DIEPINT_XFRC) and (OTG_FS->DIEPMSK & DIEPMSK_XFRCM)) {
        Uart.Printf("XFRC\r\n");
        Ep[EpN].IsTransmitting = false;
        if(Ep[EpN].cbIn != NULL) Ep[EpN].cbIn();
    }
    // TX FIFO empty
    if ((epint & DIEPINT_TXFE) and (OTG_FS->DIEPEMPMSK & (1 << EpN))) {
        Uart.Printf("TXFE\r\n");
        chSysLockFromIsr();
        Ep[EpN].IsTxPending = true;
        OTG_FS->DIEPEMPMSK &= ~(1 << EpN);  // Disable TxFifoEmpty irq for this EP
        if(PThread->p_state == THD_STATE_SUSPENDED) chThdResumeI(PThread);
        chSysUnlockFromIsr();
    }
}

void Usb_t::IEpOutHandler(uint8_t EpN) {
    Uart.Printf("out %u\r\n", EpN);
    uint32_t epint = OTG_FS->oe[EpN].DOEPINT;
    OTG_FS->oe[EpN].DOEPINT = 0xFFFFFFFF;   // Reset IRQs
    // Setup packets handling
    if ((epint & DOEPINT_STUP) and (OTG_FS->DOEPMSK & DOEPMSK_STUPM)) {
        Ep0SetupHandler();
    }
    // Receive transfer complete
    if ((epint & DOEPINT_XFRC) and (OTG_FS->DOEPMSK & DOEPMSK_XFRCM)) {
        Uart.Printf("  outh\r\n");
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
