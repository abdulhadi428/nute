/*
 * usb2xx.cpp
 *
 *  Created on: 31.12.2012
 *      Author: kreyl
 */

#include "usb2xx.h"
#include "stm32.h"
#include "stm32_rcc.h"

Usb_t Usb;

#define USB_OTG_MAX_TX_FIFOS                 15

typedef struct USB_OTG_ep {
  uint8_t        num;
  uint8_t        is_in;
  uint8_t        is_stall;
  uint8_t        type;
  uint8_t        data_pid_start;
  uint8_t        even_odd_frame;
  uint16_t       tx_fifo_num;
  uint32_t       maxpacket;
  /* transaction level variables*/
  uint8_t        *xfer_buff;
  uint32_t       dma_addr;
  uint32_t       xfer_len;
  uint32_t       xfer_count;
  /* Transfer level variables*/
  uint32_t       rem_data_len;
  uint32_t       total_data_len;
  uint32_t       ctl_data_len;
}
USB_OTG_EP , *PUSB_OTG_EP;

typedef struct USB_OTG_core_cfg {
  uint8_t       host_channels;
  uint8_t       dev_endpoints;
  uint8_t       speed;
  uint8_t       dma_enable;
  uint16_t      mps;
  uint16_t      TotalFifoSize;
  uint8_t       phy_itface;
  uint8_t       Sof_output;
  uint8_t       low_power;
  uint8_t       coreID;

}
USB_OTG_CORE_CFGS, *PUSB_OTG_CORE_CFGS;

typedef struct _USB_OTG_GREGS  //000h
{
  __IO uint32_t GOTGCTL;      /* USB_OTG Control and Status Register    000h*/
  __IO uint32_t GOTGINT;      /* USB_OTG Interrupt Register             004h*/
  __IO uint32_t GAHBCFG;      /* Core AHB Configuration Register    008h*/
  __IO uint32_t GUSBCFG;      /* Core USB Configuration Register    00Ch*/
  __IO uint32_t GRSTCTL;      /* Core Reset Register                010h*/
  __IO uint32_t GINTSTS;      /* Core Interrupt Register            014h*/
  __IO uint32_t GINTMSK;      /* Core Interrupt Mask Register       018h*/
  __IO uint32_t GRXSTSR;      /* Receive Sts Q Read Register        01Ch*/
  __IO uint32_t GRXSTSP;      /* Receive Sts Q Read & POP Register  020h*/
  __IO uint32_t GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  __IO uint32_t DIEPTXF0_HNPTXFSIZ;   /* EP0 / Non Periodic Tx FIFO Size Register 028h*/
  __IO uint32_t HNPTXSTS;     /* Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  uint32_t Reserved30[2];     /* Reserved                           030h*/
  __IO uint32_t GCCFG;        /* General Purpose IO Register        038h*/
  __IO uint32_t CID;          /* User ID Register                   03Ch*/
  uint32_t  Reserved40[48];   /* Reserved                      040h-0FFh*/
  __IO uint32_t HPTXFSIZ; /* Host Periodic Tx FIFO Size Reg     100h*/
  __IO uint32_t DIEPTXF[USB_OTG_MAX_TX_FIFOS];/* dev Periodic Transmit FIFO */
}
USB_OTG_GREGS;

typedef struct _USB_OTG_DREGS // 800h
{
  __IO uint32_t DCFG;         /* dev Configuration Register   800h*/
  __IO uint32_t DCTL;         /* dev Control Register         804h*/
  __IO uint32_t DSTS;         /* dev Status Register (RO)     808h*/
  uint32_t Reserved0C;           /* Reserved                     80Ch*/
  __IO uint32_t DIEPMSK;   /* dev IN Endpoint Mask         810h*/
  __IO uint32_t DOEPMSK;  /* dev OUT Endpoint Mask        814h*/
  __IO uint32_t DAINT;     /* dev All Endpoints Itr Reg    818h*/
  __IO uint32_t DAINTMSK; /* dev All Endpoints Itr Mask   81Ch*/
  uint32_t  Reserved20;          /* Reserved                     820h*/
  uint32_t Reserved9;       /* Reserved                     824h*/
  __IO uint32_t DVBUSDIS;    /* dev VBUS discharge Register  828h*/
  __IO uint32_t DVBUSPULSE;  /* dev VBUS Pulse Register      82Ch*/
  __IO uint32_t DTHRCTL;     /* dev thr                      830h*/
  __IO uint32_t DIEPEMPMSK; /* dev empty msk             834h*/
  __IO uint32_t DEACHINT;    /* dedicated EP interrupt       838h*/
  __IO uint32_t DEACHMSK;    /* dedicated EP msk             83Ch*/
  uint32_t Reserved40;      /* dedicated EP mask           840h*/
  __IO uint32_t DINEP1MSK;  /* dedicated EP mask           844h*/
  uint32_t  Reserved44[15];      /* Reserved                 844-87Ch*/
  __IO uint32_t DOUTEP1MSK; /* dedicated EP msk            884h*/
}
USB_OTG_DREGS;

typedef struct _USB_OTG_INEPREGS
{
  __IO uint32_t DIEPCTL; /* dev IN Endpoint Control Reg 900h + (ep_num * 20h) + 00h*/
  uint32_t Reserved04;             /* Reserved                       900h + (ep_num * 20h) + 04h*/
  __IO uint32_t DIEPINT; /* dev IN Endpoint Itr Reg     900h + (ep_num * 20h) + 08h*/
  uint32_t Reserved0C;             /* Reserved                       900h + (ep_num * 20h) + 0Ch*/
  __IO uint32_t DIEPTSIZ; /* IN Endpoint Txfer Size   900h + (ep_num * 20h) + 10h*/
  __IO uint32_t DIEPDMA; /* IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h*/
  __IO uint32_t DTXFSTS;/*IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h*/
  uint32_t Reserved18;             /* Reserved  900h+(ep_num*20h)+1Ch-900h+ (ep_num * 20h) + 1Ch*/
}
USB_OTG_INEPREGS;

typedef struct _USB_OTG_OUTEPREGS
{
  __IO uint32_t DOEPCTL;       /* dev OUT Endpoint Control Reg  B00h + (ep_num * 20h) + 00h*/
  uint32_t Reserved04;         /* Reserved                      B00h + (ep_num * 20h) + 04h*/
  __IO uint32_t DOEPINT;       /* dev OUT Endpoint Itr Reg      B00h + (ep_num * 20h) + 08h*/
  uint32_t Reserved0C;         /* Reserved                      B00h + (ep_num * 20h) + 0Ch*/
  __IO uint32_t DOEPTSIZ;      /* dev OUT Endpoint Txfer Size   B00h + (ep_num * 20h) + 10h*/
  __IO uint32_t DOEPDMA;       /* dev OUT Endpoint DMA Address  B00h + (ep_num * 20h) + 14h*/
  uint32_t Reserved18[2];      /* Reserved B00h + (ep_num * 20h) + 18h - B00h + (ep_num * 20h) + 1Ch*/
}
USB_OTG_OUTEPREGS;

typedef struct _USB_OTG_HREGS
{
  __IO uint32_t HCFG;             /* Host Configuration Register    400h*/
  __IO uint32_t HFIR;      /* Host Frame Interval Register   404h*/
  __IO uint32_t HFNUM;         /* Host Frame Nbr/Frame Remaining 408h*/
  uint32_t Reserved40C;                   /* Reserved                       40Ch*/
  __IO uint32_t HPTXSTS;   /* Host Periodic Tx FIFO/ Queue Status 410h*/
  __IO uint32_t HAINT;   /* Host All Channels Interrupt Register 414h*/
  __IO uint32_t HAINTMSK;   /* Host All Channels Interrupt Mask 418h*/
}
USB_OTG_HREGS;

typedef struct _USB_OTG_HC_REGS
{
  __IO uint32_t HCCHAR;
  __IO uint32_t HCSPLT;
  __IO uint32_t HCINT;
  __IO uint32_t HCINTMSK;
  __IO uint32_t HCTSIZ;
  __IO uint32_t HCDMA;
  uint32_t Reserved[2];
}
USB_OTG_HC_REGS;

typedef struct USB_OTG_core_regs {
  USB_OTG_GREGS         *GREGS;
  USB_OTG_DREGS         *DREGS;
  USB_OTG_HREGS         *HREGS;
  USB_OTG_INEPREGS      *INEP_REGS[USB_OTG_MAX_TX_FIFOS];
  USB_OTG_OUTEPREGS     *OUTEP_REGS[USB_OTG_MAX_TX_FIFOS];
  USB_OTG_HC_REGS       *HC_REGS[USB_OTG_MAX_TX_FIFOS];
  __IO uint32_t         *HPRT0;
  __IO uint32_t         *DFIFO[USB_OTG_MAX_TX_FIFOS];
  __IO uint32_t         *PCGCCTL;
}
USB_OTG_CORE_REGS , *PUSB_OTG_CORE_REGS;

typedef struct _DCD
{
  uint8_t        device_config;
  uint8_t        device_state;
  uint8_t        device_status;
  uint8_t        device_old_status;
  uint8_t        device_address;
  uint8_t        connection_status;
  uint8_t        test_mode;
  uint32_t       DevRemoteWakeup;
  USB_OTG_EP     in_ep   [USB_OTG_MAX_TX_FIFOS];
  USB_OTG_EP     out_ep  [USB_OTG_MAX_TX_FIFOS];
  uint8_t        setup_packet [8*3];
  //USBD_Class_cb_TypeDef         *class_cb;
  //USBD_Usr_cb_TypeDef           *usr_cb;
  //USBD_DEVICE                   *usr_device;
  uint8_t        *pConfig_descriptor;
 }
DCD_DEV , *DCD_PDEV;


typedef struct USB_OTG_handle
{
  USB_OTG_CORE_CFGS    cfg;
  USB_OTG_CORE_REGS    regs;
  DCD_DEV     dev;
}
USB_OTG_CORE_HANDLE , *PUSB_OTG_CORE_HANDLE;

USB_OTG_CORE_HANDLE dev;

#define USB_OTG_FS_BASE_ADDR  0x50000000
#define USB_OTG_EMBEDDED_PHY  2

#define USB_OTG_CORE_GLOBAL_REGS_OFFSET      0x000
#define USB_OTG_DEV_GLOBAL_REG_OFFSET        0x800
#define USB_OTG_DEV_IN_EP_REG_OFFSET         0x900
#define USB_OTG_EP_REG_OFFSET                0x20
#define USB_OTG_DEV_OUT_EP_REG_OFFSET        0xB00
#define USB_OTG_HOST_GLOBAL_REG_OFFSET       0x400
#define USB_OTG_HOST_PORT_REGS_OFFSET        0x440
#define USB_OTG_HOST_CHAN_REGS_OFFSET        0x500
#define USB_OTG_CHAN_REGS_OFFSET             0x20
#define USB_OTG_PCGCCTL_OFFSET               0xE00
#define USB_OTG_DATA_FIFO_OFFSET             0x1000
#define USB_OTG_DATA_FIFO_SIZE               0x1000

/*  Device Status */
#define USB_OTG_DEFAULT                          1
#define USB_OTG_ADDRESSED                        2
#define USB_OTG_CONFIGURED                       3
#define USB_OTG_SUSPENDED                        4

#define EP_TYPE_CTRL                           0
#define EP_TYPE_ISOC                           1
#define EP_TYPE_BULK                           2
#define EP_TYPE_INTR                           3
#define EP_TYPE_MSK                            3

#define USB_OTG_MAX_EP0_SIZE                 64

void Usb_t::On() {
/*    Status = usOff;
    // Pins
    PinSetupAlterFunc(GPIOA, 11, omPushPull, pudNone, AF10, ps100MHz);  // DM
    PinSetupAlterFunc(GPIOA, 12, omPushPull, pudNone, AF10, ps100MHz);  // DP
    // Clock
    rccEnableOTG_FS(0);

    // Set USB core params
    // Select core
    USB_OTG_CORE_HANDLE *pdev = &dev;
    pdev->cfg.dma_enable       = 0;
    pdev->cfg.speed            = 1;     // Full speed
    pdev->cfg.mps              = 64;    // Max packet size

    // initialize device cfg following its address
    uint32_t i , baseAddress;
    baseAddress                = USB_OTG_FS_BASE_ADDR;
    pdev->cfg.coreID           = 1;     // USB_OTG_FS_CORE_ID
    pdev->cfg.host_channels    = 8 ;
    pdev->cfg.dev_endpoints    = 4 ;
    pdev->cfg.TotalFifoSize    = 320;
    pdev->cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;

    pdev->regs.GREGS = (USB_OTG_GREGS*) (baseAddress + USB_OTG_CORE_GLOBAL_REGS_OFFSET);
    pdev->regs.DREGS =  (USB_OTG_DREGS*)(baseAddress + USB_OTG_DEV_GLOBAL_REG_OFFSET);

    for (i=0; i < pdev->cfg.dev_endpoints; i++) {
        pdev->regs.INEP_REGS[i]  = (USB_OTG_INEPREGS*) (baseAddress + USB_OTG_DEV_IN_EP_REG_OFFSET +  (i * USB_OTG_EP_REG_OFFSET));
        pdev->regs.OUTEP_REGS[i] = (USB_OTG_OUTEPREGS*)(baseAddress + USB_OTG_DEV_OUT_EP_REG_OFFSET + (i * USB_OTG_EP_REG_OFFSET));
    }
    pdev->regs.HREGS = (USB_OTG_HREGS *)(baseAddress + USB_OTG_HOST_GLOBAL_REG_OFFSET);
    pdev->regs.HPRT0 = (uint32_t *)(baseAddress + USB_OTG_HOST_PORT_REGS_OFFSET);

    for (i = 0; i < pdev->cfg.host_channels; i++) {
        pdev->regs.HC_REGS[i] = (USB_OTG_HC_REGS*)(baseAddress + USB_OTG_HOST_CHAN_REGS_OFFSET + (i * USB_OTG_CHAN_REGS_OFFSET));
        pdev->regs.DFIFO[i] = (uint32_t *)(baseAddress + USB_OTG_DATA_FIFO_OFFSET + (i * USB_OTG_DATA_FIFO_SIZE));
    }

    pdev->regs.PCGCCTL = (uint32_t *)(baseAddress + USB_OTG_PCGCCTL_OFFSET);

    // Proceed with init
    pdev->dev.device_status = USB_OTG_DEFAULT;
    pdev->dev.device_address = 0;
    USB_OTG_EP *ep;

    // Init ep structure
    for (i = 0; i < pdev->cfg.dev_endpoints ; i++) {
      ep = &pdev->dev.in_ep[i];
      // Init ep structure
      ep->is_in = 1;
      ep->num = i;
      ep->tx_fifo_num = i;
      // Control until ep is actvated
      ep->type = EP_TYPE_CTRL;
      ep->maxpacket =  USB_OTG_MAX_EP0_SIZE;
      ep->xfer_buff = 0;
      ep->xfer_len = 0;
    }

    for (i = 0; i < pdev->cfg.dev_endpoints; i++) {
      ep = &pdev->dev.out_ep[i];
      // Init ep structure
      ep->is_in = 0;
      ep->num = i;
      ep->tx_fifo_num = i;
      // Control until ep is activated
      ep->type = EP_TYPE_CTRL;
      ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
      ep->xfer_buff = 0;
      ep->xfer_len = 0;
    }
*/

}
