/*
 * File:   cc2500.h
 * Author: Laurelindo
 *
 * Created on 11 Декабрь 2009 г., 2:08
 */

#ifndef _CC1101_H
#define	_CC1101_H

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "cc1101defins.h"

// ================================== Defins ===================================
// Ports
#define CC_GPIO         GPIOB
#define CC_GPIO_CLK     RCC_APB2Periph_GPIOB
#define CC_AFIO_CLK     RCC_APB2Periph_AFIO

#define CC_GDO0         GPIO_Pin_4
#define CC_CS           GPIO_Pin_5
#define CC_MOSI         GPIO_Pin_15
#define CC_MISO         GPIO_Pin_14
#define CC_SCLK         GPIO_Pin_13
// SPI
#define CC_SPI          SPI2
#define CC_SPI_CLK      RCC_APB1Periph_SPI2
// IRQ
//#define CC_GDO0_PINSOURCE   GPIO_PinSource0
//#define CC_GDO0_PORTSOURCE  GPIO_PortSourceGPIOA
//#define CC_GDO0_EXTI_LINE   EXTI_Line0              // As EXTI0 connected to Pin0
//#define CC_GDO0_EXTI_IRQn   EXTI0_IRQn

// ============================ Types & variables ==============================
#define CC_PKT_DATA_LEN     4
#define CC_PKT_LEN          (2+CC_PKT_DATA_LEN)
struct CC_Packet_t{
    uint8_t PacketID;
    uint8_t CommandID;
    uint8_t Data[CC_PKT_DATA_LEN];
    uint8_t RSSI;
    uint8_t LQI;
};

class CC_t {
private:
    // Variables
    uint32_t Timer;
    uint8_t State;
    union {
        uint8_t RX_PktArray[CC_PKT_LEN+2];
        CC_Packet_t RX_Pkt;
    };
    union {
        uint8_t TX_PktArray[CC_PKT_LEN+2];
        CC_Packet_t TX_Pkt;
    };
    bool GDO0_WasHi;
    // Methods
    uint8_t ReadWriteByte(uint8_t AByte);
    // Pins
    void CS_Hi(void) { CC_GPIO->BSRR = CC_CS; }
    void CS_Lo(void) { CC_GPIO->BRR  = CC_CS; }
    bool GDO0_IsHi(void);
    void BusyWait(void);

    void RfConfig(void);
    void WriteTX (uint8_t *PData, uint8_t ALength);
    void ReadRX  (uint8_t *PData, uint8_t ALength);
    void EnterTXAndWaitToComplete(void);
    // Registers
    void WriteRegister (const uint8_t Addr, const uint8_t AData);
    uint8_t ReadRegister (const uint8_t Addr);
    void WriteStrobe(uint8_t AStrobe);
    // IRQ
    //void IRQDisable(void) { /* NVIC_DisableIRQ(CC_GDO0_EXTI_IRQn);  */}   // Disable Exti line
    //void IRQEnable (void) { /* NVIC_EnableIRQ (CC_GDO0_EXTI_IRQn); */}
    //void IRQReset  (void) { /* EXTI_ClearFlag (CC_GDO0_EXTI_LINE); */}
    // Strobes
    void Reset(void)        { WriteStrobe(CC_SRES); }
    void FlushRxFIFO(void)  { WriteStrobe(CC_SFRX); }
    void EnterTX(void)      { WriteStrobe(CC_STX);  }
    void EnterRX(void)      { WriteStrobe(CC_SRX);  }
    void EnterIdle(void)    { WriteStrobe(CC_SIDLE);}
    void PowerDown(void)    { WriteStrobe(CC_SPWD); }
    void Calibrate(void)    { WriteStrobe(CC_SCAL); }
    void FlushTxFIFO(void)  { WriteStrobe(CC_SFTX); }
    void GetState(void)     { WriteStrobe(CC_SNOP); }
public:
    uint8_t Channel;
    // Methods
    void Init(void);
    void Task(void);
    void SetChannel(uint8_t AChannel);
};

extern CC_t CC;


// ================================ Prototypes =================================
void EVENT_NewPacket(void);


#endif	/* _CC1101_H */

