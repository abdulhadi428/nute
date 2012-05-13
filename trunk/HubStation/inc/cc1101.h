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
#include "kl_util.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "cc1101_rf_settings.h"
#include "cc1101defins.h"

#include "cc1190.h"

// ================================== Defins ===================================
#define CC_GPIO     GPIOA
#define CC_GDO0     GPIO_Pin_4
#define CC_GDO0_N   4
#define CC_CS       GPIO_Pin_8
#define CC_MOSI     GPIO_Pin_7
#define CC_MISO     GPIO_Pin_6
#define CC_SCLK     GPIO_Pin_5

// ============================ Types & variables ==============================
struct CC_Packet_t {
    uint8_t Address;
    uint8_t PktID;
    uint8_t LastRxRawRssi;
    uint8_t Cmd;
    uint8_t Arr[18];
    uint8_t RSSI;
    uint8_t LQI;
} PACKED;

#define CC_PKT_LEN  (sizeof(CC_Packet_t)-2)

class CC_t {
private:
    cc1190_t cc1190;
    // Variables
    uint32_t Timer;
    uint8_t IState;
    bool WasTransmitting;
    // Methods
    uint8_t ReadWriteByte(uint8_t AByte);
    // Pins
    klPinIrq_t IrqPin;
    void CS_Hi(void) { CC_GPIO->BSRR = CC_CS; }
    void CS_Lo(void) { CC_GPIO->BRR  = CC_CS; }
    bool GDO0_IsHi(void) { return klGpioIsSetByMsk(CC_GPIO, CC_GDO0); }
    void BusyWait(void)  { while (klGpioIsSetByMsk(CC_GPIO, CC_MISO));}

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
    void PowerDown(void)    { WriteStrobe(CC_SPWD); }
    void Calibrate(void)    { WriteStrobe(CC_SCAL); }
    void FlushTxFIFO(void)  { WriteStrobe(CC_SFTX); }
    void GetState(void)     { WriteStrobe(CC_SNOP); }
public:
    CC_Packet_t RX_Pkt, TX_Pkt;
    bool NewPktRcvd;
    // Methods
    void Init(void);
    void Task(void);
    int16_t RSSI_dBm(uint8_t ARawRSSI);
    int16_t RSSI_dBm(void) { return RSSI_dBm(RX_Pkt.RSSI); }
    bool IsIdle(void) { return (IState == CC_STB_IDLE); }
    void SetChannel(uint8_t AChannel);
    void SetAddress(uint8_t AAddr) { WriteRegister(CC_ADDR, AAddr); }
    void SetPower(uint8_t APwrID) { if (APwrID <= 8) WriteRegister(CC_PATABLE, CC_PwrLevels[APwrID]); }
    // RX/TX
    void Transmit(void);
    void Receive(void);
    void EnterIdle(void)    { WriteStrobe(CC_SIDLE);}
    // IRQ handler
    void IRQHandler(void);
};

extern CC_t CC;

// Interrupt
extern "C" {
void EXTI4_IRQHandler(void);
}

#endif	/* _CC1101_H */

