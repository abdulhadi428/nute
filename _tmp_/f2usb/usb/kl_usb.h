/*
 * kl_usb.h
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#ifndef KL_USB_H_
#define KL_USB_H_

#include "kl_lib_f2xx.h"
#include "kl_buf.h"

#define RX_FIFO_SZW     128
#define TX0_FIFO_SZW    32
#define EP0_SZ          64
#define SETUP_BUF_SZ    8   // Standard value, do not change

//enum EpMode_t {emControl, emIn, emOut};

typedef void (*EpCallback_t)(void);

class Endpoint_t {
private:
    uint8_t *IPBuf;
    uint32_t ILen;
public:
    bool IsTransmitting, IsReceiving, IsTxPending;
    uint8_t SelfN;
    uint8_t InMaxSz, OutMaxSz;
//    EpCallback_t cbIn, cbOut;
    // Transfer section
    Buf_t<uint8_t> Tx, Rx;
    bool TxFifoHandler();
    // Methods
    void StallIn()  { OTG_FS->ie[SelfN].DIEPCTL |= DIEPCTL_STALL; }
    void StallOut() { OTG_FS->oe[SelfN].DOEPCTL |= DOEPCTL_STALL; }
    void PrepareTransmit(uint8_t *Ptr, uint32_t Len);
    //void PrepareReceive(uint32_t Len);
    void Receive(uint32_t Len);
    // State change
    inline void EnableInFifoEmptyIRQ()  { OTG_FS->DIEPEMPMSK |=  (1 << SelfN); }
    inline void DisableInFifoEmptyIRQ() { OTG_FS->DIEPEMPMSK &= ~(1 << SelfN); }
    inline bool FifoEmtyIRQEnabled()    { return (OTG_FS->DIEPEMPMSK & (1 << SelfN)); }
    inline void SetInNAK()    { OTG_FS->ie[SelfN].DIEPCTL |= DIEPCTL_SNAK; }
    inline void ClearInNAK()  { OTG_FS->ie[SelfN].DIEPCTL |= DIEPCTL_CNAK; }
    inline void SetOutNAK()   { OTG_FS->oe[SelfN].DOEPCTL |= DOEPCTL_SNAK; }
    inline void ClearOutNAK() { OTG_FS->oe[SelfN].DOEPCTL |= DOEPCTL_CNAK; }
    inline void EnableOut()   { OTG_FS->oe[SelfN].DOEPCTL |= DOEPCTL_EPENA; }
};

class UsbMemAllocator_t {
private:
    uint32_t PNext;     // Pointer to the next address in the packet memory
public:
    void Reset() { PNext = RX_FIFO_SZW; }
    uint32_t Alloc(uint32_t Size) {
        uint32_t next = PNext;
        PNext += Size;
        return next;
    }
};

union UsbSetupReq_t {
    uint8_t Buf[SETUP_BUF_SZ];
    struct {
        uint8_t  bmRequestType;
        uint8_t  bRequest;
        uint16_t wValue;
        uint16_t wIndex;
        uint16_t wLength;
    };
};

#define USB_EP_CNT  1

enum Ep0State_t {esWaitingSetup, esTX, esWaitingSTS, esRX, esSendingSTS, esError};

class Usb_t {
private:
    // Endpoints
    Endpoint_t Ep[USB_EP_CNT];
    // Common
    Thread *PThread;
    uint8_t Configuration;
    void SetAddress(uint8_t Addr) { OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(Addr); }
    // Buffer operations
    void ReadFifo(uint8_t *PBuf, volatile uint32_t *PFifo, uint32_t Cnt);
    void WriteFifo(volatile uint32_t *PFifo, uint8_t *PBuf, uint32_t Cnt);
    UsbMemAllocator_t MemAllocator;
    void RxFifoFlush();
    void TxFifoFlush();
    // irq methods
    void IReset();
    void IRxHandler();
    void IEpOutHandler(uint8_t EpN);
    void IEpInHandler(uint8_t EpN);
    inline void DisableIrqs() { OTG_FS->GAHBCFG &= ~GAHBCFG_GINTMSK; }
    inline void EnableIrqs()  { OTG_FS->GAHBCFG |= GAHBCFG_GINTMSK; }
    // Ep0 related
    Ep0State_t Ep0State;
    UsbSetupReq_t SetupReq;
    uint8_t Ep0RxBuf[99];
    void Ep0SetupHandler();
    uint8_t StdReqHandler(uint8_t **Ptr, uint32_t *PLen);
    uint8_t NonStdReqHandler();
    void Ep0InCallback();
    void Ep0OutCallback();
public:
    void Init();
    void Connect()    { OTG_FS->GCCFG |=  GCCFG_VBUSBSEN; }
    void Disconnect() { OTG_FS->GCCFG &= ~GCCFG_VBUSBSEN; }

    // Inner use
    void ITask();
    void IHandleIrq();
};

extern Usb_t Usb;


// ============================== Constants ====================================
#define USB_REQ_GET_STATUS                  0
#define USB_REQ_CLEAR_FEATURE               1
#define USB_REQ_SET_FEATURE                 3
#define USB_REQ_SET_ADDRESS                 5
#define USB_REQ_GET_DESCRIPTOR              6
#define USB_REQ_SET_DESCRIPTOR              7
#define USB_REQ_GET_CONFIGURATION           8
#define USB_REQ_SET_CONFIGURATION           9
#define USB_REQ_GET_INTERFACE               10
#define USB_REQ_SET_INTERFACE               11
#define USB_REQ_SYNCH_FRAME                 12

#define USB_FEATURE_ENDPOINT_HALT           0
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP    1
#define USB_FEATURE_TEST_MODE               2

#endif /* KL_USB_H_ */