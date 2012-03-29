/*
 * File:   cc2500.h
 * Author: Laurelindo
 *
 * Created on 11 Декабрь 2009 г., 2:08
 */

#ifndef _CC1101_H
#define	_CC1101_H

#include "stm32f10x.h"
#include "kl_lib.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "cc1101defins.h"

// ================================== Defins ===================================
#define CC_GDO0         GPIO_Pin_4
#define CC_CS           GPIO_Pin_5
#define CC_MOSI         GPIO_Pin_15
#define CC_MISO         GPIO_Pin_14
#define CC_SCLK         GPIO_Pin_13

// ============================ Types & variables ==============================
#define PAYLOAD_SIZE    1

struct CC_Packet_t {
    uint8_t PktID;
    uint8_t Arr[PAYLOAD_SIZE];
    uint8_t RSSI;
    uint8_t LQI;
} PACKED;

#define CC_PKT_LEN  (sizeof(CC_Packet_t)-2)

class CC_t {
private:
    // Variables
    uint32_t Timer;
    uint8_t State;
    bool NewPktRcvd;
    // Methods
    uint8_t ReadWriteByte(uint8_t AByte);
    // Pins
    void CS_Hi(void) { GPIOB->BSRR = CC_CS; }
    void CS_Lo(void) { GPIOB->BRR  = CC_CS; }
    bool GDO0_IsHi(void) { return GPIO_ReadInputDataBit(GPIOB, CC_GDO0); }
    void BusyWait(void)  { while (GPIO_ReadInputDataBit(GPIOB, CC_MISO));}

    void RfConfig(void);
    void WriteTX();
    bool ReadRX();
    void EnterTXAndWaitToComplete(void);
    // Registers
    void WriteRegister (const uint8_t Addr, const uint8_t AData);
    uint8_t ReadRegister (const uint8_t Addr);
    void WriteStrobe(uint8_t AStrobe);
    // Strobes
    void Reset(void)        { WriteStrobe(CC_SRES); }
    void FlushRxFIFO(void)  { WriteStrobe(CC_SFRX); }
    void EnterRX(void)      { WriteStrobe(CC_SRX);  }
    void EnterTX(void)      { WriteStrobe(CC_STX);  }
    void EnterIdle(void)    { WriteStrobe(CC_SIDLE);}
    void PowerDown(void)    { WriteStrobe(CC_SPWD); }
    void Calibrate(void)    { WriteStrobe(CC_SCAL); }
    void FlushTxFIFO(void)  { WriteStrobe(CC_SFTX); }
    void GetState(void)     { WriteStrobe(CC_SNOP); }
public:
    CC_Packet_t RX_Pkt, TX_Pkt;
    ftVoid_Void EvtNewPkt;
    // Methods
    void Init(void);
    void Task(void);
    void SetChannel(uint8_t AChannel);
    void SetAddress(uint8_t AAddr) { WriteRegister(CC_ADDR, AAddr); }
    // IRQ handler
    void IRQHandler(void);
};

extern CC_t CC;

// Interrupt
extern "C" {
void EXTI4_IRQHandler(void);
}

#endif	/* _CC1101_H */

