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

// Timebase
#define CYCLE_COUNT         4
#define PKT_DURATION        45  // ms
#define SUBCYCLE_DURATION   (PKT_DURATION*18) // ms

// ============================ Types & variables ==============================
struct CC_Packet_t {
    uint8_t CommandID;
    uint8_t SenderAddr;
    uint8_t SenderCycle;
    uint16_t SenderTime;
    uint8_t ArtType;
    uint8_t ChargeCount;
    uint8_t RSSI;
    uint8_t LQI;
};
#define CC_PKT_LEN  (sizeof(CC_Packet_t)-2)

class CC_t {
private:
    // Variables
    uint32_t Timer;
    uint8_t State;
    bool GDO0_WasHi;
    // Methods
    uint8_t ReadWriteByte(uint8_t AByte);
    // Pins
    void CS_Hi(void) { CC_GPIO->BSRR = CC_CS; }
    void CS_Lo(void) { CC_GPIO->BRR  = CC_CS; }
    bool GDO0_IsHi(void);
    void BusyWait(void);

    void RfConfig(void);
    void ReadRX  (uint8_t *PData, uint8_t ALength);
    void EnterTXAndWaitToComplete(void);
    // Registers
    void WriteRegister (const uint8_t Addr, const uint8_t AData);
    uint8_t ReadRegister (const uint8_t Addr);
    void WriteStrobe(uint8_t AStrobe);
    // Strobes
    void Reset(void)        { WriteStrobe(CC_SRES); }
    void FlushRxFIFO(void)  { WriteStrobe(CC_SFRX); }
    void EnterRX(void)      { WriteStrobe(CC_SRX);  }
    void PowerDown(void)    { WriteStrobe(CC_SPWD); }
    void Calibrate(void)    { WriteStrobe(CC_SCAL); }
    void FlushTxFIFO(void)  { WriteStrobe(CC_SFTX); }
    void GetState(void)     { WriteStrobe(CC_SNOP); }
public:
    uint8_t Addr;
    uint8_t CycleCounter;
    union {
        uint8_t RX_PktArray[CC_PKT_LEN+2];
        CC_Packet_t RX_Pkt;
    };
    union {
        uint8_t TX_PktArray[CC_PKT_LEN+2];
        CC_Packet_t TX_Pkt;
    };
    // Methods
    void Init(void);
    void Task(void);
    void SetChannel(uint8_t AChannel);
    void TimebaseInit(void);
    // Interrupt used
    void TimerStart(void) { TIM_Cmd(TIM5, ENABLE); }
    void TimerStop(void)  { TIM_Cmd(TIM5, DISABLE); }
    void WriteTX (uint8_t *PData, uint8_t ALength);
    void EnterTX(void)      { WriteStrobe(CC_STX);  }
    void EnterIdle(void)    { WriteStrobe(CC_SIDLE);}
};

extern CC_t CC;

extern uint8_t OthersIDs[18];

// ================================ Prototypes =================================
void EVENT_NewPacket(void);

// Declare Timer IRQ. Use externC to make it visible from asm file.
extern "C" {
void TIM5_IRQHandler(void);
}

#endif	/* _CC1101_H */

