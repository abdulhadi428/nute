/*
 * kl_usb.cpp
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#include "kl_usb.h"

#define TRDT        3   // AHB clock = 24, => TRDT = 24/48 +1

#define EP0_OUT_SZ  0x40
#define EP0_IN_SZ   0x40

Usb_t Usb;

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
        if((OTG_FS->GINTSTS & GINTSTS_RXFLVL) == 0) {
            OTG_FS->GINTMSK |= GINTMSK_RXFLVLM; // Enable RX
            chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by IRQ
        }
        chSysUnlock();

        while (OTG_FS->GINTSTS & GINTSTS_RXFLVL) RxHandler();
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
    nvicEnableVector(STM32_OTG1_NUMBER, CORTEX_PRIORITY_MASK(IRQ_PRIO_MEDIUM));

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

    // ==== Endpoints re-initialization ====
    Ep[0].Mode = emControl;

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
//        case GRXSTSP_SETUP_COMP:
//            Uart.Printf("Setup completed\r");
//            break;

        case GRXSTSP_SETUP_DATA:
            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
            Uart.Printf(" setup %u %u\r", cnt, ep);
            if((ep == 0) and (cnt == 8)) ReadToBuf(SetupBuf, OTG_FS->FIFO[0], cnt);
            //Uart.Printf("> %A\r", Ep[0].Buf, cnt, ' ');
            break;
        case GRXSTSP_OUT_DATA:
            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
            Uart.Printf(" out %u %u\r", cnt, ep);
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
    Uart.Printf("Ep0 setup\r");
    // Decode request
    if(StdRequestHandler() != OK) {

    }
}

uint8_t Usb_t::StdRequestHandler() {
    uint8_t Recipient = SetupBuf[0] & USB_RTYPE_RECIPIENT_MASK;
    uint8_t bRequest = SetupBuf[1];
    if(Recipient == USB_RTYPE_RECIPIENT_DEVICE) {
        switch(bRequest) {
            case USB_REQ_GET_STATUS:

                return OK;
                break;
            case USB_REQ_SET_ADDRESS:
                SetAddress(SetupBuf[2]);

                return OK;
                break;

        }
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_INTERFACE) {

    }
    else if(Recipient == USB_RTYPE_RECIPIENT_ENDPOINT) {

    }
    else return FAILURE;

    uint8_t RecAndType = (SetupBuf[0] & (USB_RTYPE_RECIPIENT_MASK | USB_RTYPE_TYPE_MASK)) | (SetupBuf[1] << 8);
    switch(RecAndType) {
        case (USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_STATUS << 8)):    // Just return current status word
            //usbSetupTransfer(usbp, (uint8_t *)&usbp->status, 2, NULL);
            break;
    }

}



// ================================ IRQ ========================================
void Usb_t::IHandleIrq() {
    uint32_t sts, src;
    // Get irq flag
    sts = OTG_FS->GINTSTS & OTG_FS->GINTMSK;
    OTG_FS->GINTSTS = sts;
    Uart.Printf("u %X\r", sts);

    // Reset
    if(sts & GINTSTS_USBRST) IReset();
    // Enumeration done
    if (sts & GINTSTS_ENUMDNE) {
        Uart.Printf("ed\r");
        (void)OTG_FS->DSTS;
    }
    // RX FIFO not empty handling
    if (sts & GINTSTS_RXFLVL) {
        Uart.Printf("rx\r");
        chSysLockFromIsr();
        OTG_FS->GINTMSK &= ~GINTMSK_RXFLVLM;    // Disable RX irq until current is processed
        if(PThread->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThread);
        chSysUnlockFromIsr();
    }

    src = OTG_FS->DAINT;
    // OUT EP event handling
    if (sts & GINTSTS_OEPINT) {
//        Uart.Printf("out %X\r", src);
        if(src & (1 << 16)) IEpOutHandler(0);
        if(src & (1 << 17)) IEpOutHandler(1);
        if(src & (1 << 18)) IEpOutHandler(2);
        if(src & (1 << 19)) IEpOutHandler(3);
    }
}

void Usb_t::IReset() {
    Uart.Printf("rst\r");
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
    OTG_FS->oe[0].DOEPTSIZ = 0;
    OTG_FS->oe[0].DOEPCTL = DOEPCTL_SD0PID | DOEPCTL_USBAEP | DOEPCTL_EPTYP_CTRL | DOEPCTL_MPSIZ(EP0_OUT_SZ);
    OTG_FS->ie[0].DIEPTSIZ = 0;
    OTG_FS->ie[0].DIEPCTL = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL | DIEPCTL_TXFNUM(0) | DIEPCTL_MPSIZ(EP0_IN_SZ);
    OTG_FS->DIEPTXF0 = DIEPTXF_INEPTXFD(EP0_IN_SZ / 4) | DIEPTXF_INEPTXSA(MemAllocator.Alloc(EP0_IN_SZ / 4));
}

void Usb_t::IEpOutHandler(uint8_t EpN) {
    Uart.Printf("oh %u\r", EpN);
    uint32_t epint = OTG_FS->oe[EpN].DOEPINT;
    // Reset IRQs
    OTG_FS->oe[EpN].DOEPINT = 0xFFFFFFFF;
    // Setup packets handling
    if ((epint & DOEPINT_STUP) and (OTG_FS->DOEPMSK & DOEPMSK_STUPM) and (EpN == 0)) {
        //Uart.Printf("  setuph\r");
        Ep0SetupHandler();
    }
    // Receive transfer complete
    if ((epint & DOEPINT_XFRC) and (OTG_FS->DOEPMSK & DOEPMSK_XFRCM)) {
        Uart.Printf("  outh\r");
    }
}


extern "C" {
CH_IRQ_HANDLER(STM32_OTG1_HANDLER) {
  CH_IRQ_PROLOGUE();
  Usb.IHandleIrq();
  CH_IRQ_EPILOGUE();
}

} // extern C
