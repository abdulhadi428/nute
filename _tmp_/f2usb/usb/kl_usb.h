/*
 * kl_usb.h
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#ifndef KL_USB_H_
#define KL_USB_H_

#include "kl_lib_f2xx.h"

#define RX_FIFO_SZ      (STM32_USB_OTG1_RX_FIFO_SIZE / 4)
#define EP_BUF_SZ       64
#define SETUP_BUF_SZ    8   // Standard value, do not change

enum EpMode_t {emControl, emIn, emOut};

struct Endpoint_t {
    EpMode_t Mode;
    uint8_t Buf[EP_BUF_SZ];
};

class UsbMemAllocator_t {
private:
    uint32_t PNext;     // Pointer to the next address in the packet memory
public:
    void Reset() { PNext = RX_FIFO_SZ; }
    uint32_t Alloc(uint32_t Size) {
        uint32_t next = PNext;
        PNext += Size;
        return next;
    }
};

#define USB_EP_CNT  1

class Usb_t {
private:
    // Endpoints
    Endpoint_t Ep[USB_EP_CNT];
    uint8_t SetupBuf[SETUP_BUF_SZ];
    void Ep0SetupHandler();
    uint8_t StdRequestHandler();
    // Common
    void RxHandler();
    void SetAddress(uint8_t Addr) { OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(Addr); }
    // Buffer operations
    void ReadToBuf(uint8_t *PBuf, volatile uint32_t *PFifo, uint32_t Cnt);
    UsbMemAllocator_t MemAllocator;
    void RxFifoFlush();
    void TxFifoFlush();
    // irq methods
    void IReset();
    void IEpOutHandler(uint8_t EpN);
    Thread *PThread;
public:
    void Init();
    void Connect()    { OTG_FS->GCCFG |=  GCCFG_VBUSBSEN; }
    void Disconnect() { OTG_FS->GCCFG &= ~GCCFG_VBUSBSEN; }

    // Inner use
    void ITask();
    void IHandleIrq();
};

extern Usb_t Usb;

#endif /* KL_USB_H_ */
