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

#include "cc1101_rf_settings.h"
#include "cc1101defins.h"

// ================================== Defins ===================================
#define CC_GPIO     GPIOA
#define CC_GDO0     GPIO_Pin_3
#define CC_GDO0_N   3
#define CC_CS       GPIO_Pin_4
#define CC_MOSI     GPIO_Pin_7
#define CC_MISO     GPIO_Pin_6
#define CC_SCLK     GPIO_Pin_5

enum CCAim_t {caIdle, caRx, caTx};

// ============================ Types & variables ==============================
// ==== Packet ====
struct Pkt_t {
    uint32_t IdArr[3];
    uint8_t RSSI;
    uint8_t LQI;
} PACKED;
#define CC_PKT_LEN  (sizeof(Pkt_t)-2)

extern Pkt_t PktRx, PktTx;
extern klPin_t gp;

class CC_t {
private:
    uint8_t ReadWriteByte(uint8_t AByte);
    // Pins
    klPinIrq_t IrqPin;
    void CS_Hi(void) { CC_GPIO->BSRR = CC_CS; }
    void CS_Lo(void) { CC_GPIO->BRR  = CC_CS; }
    bool GDO0_IsHi(void) { return klGpioIsSetByMsk(CC_GPIO, CC_GDO0); }
    void BusyWait(void)  { while (klGpioIsSetByMsk(CC_GPIO, CC_MISO));}

    void RfConfig(void);
    void WriteTX(uint8_t* PArr, uint8_t ALen);
    bool ReadRX(uint8_t* PArr);
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
    void FlushTxFIFO(void)  { WriteStrobe(CC_SFTX); }
    void GetState(void)     { WriteStrobe(CC_SNOP); }
public:
    CCAim_t Aim;
    uint8_t IState;
    // Methods
    void Init(void);
    void Task(void);
    int16_t RSSI_dBm(uint8_t ARawRSSI);
    bool IsIdle(void) { return (IState == CC_STB_IDLE); }
    void SetChannel(uint8_t AChannel);
    void SetAddress(uint8_t AAddr) { WriteRegister(CC_ADDR, AAddr); }
    void SetPower(uint8_t APwrID) { if (APwrID <= 8) WriteRegister(CC_PATABLE, CC_PwrLevels[APwrID]); }
    // RX/TX
    void Transmit(void);
    void TransmitAndWaitIdle(void);
    void Receive(void);
    void EnterIdle(void) { WriteStrobe(CC_SIDLE); Aim = caIdle; }
    void Recalibrate(void) {
        while (IState != CC_STB_IDLE) EnterIdle();
        WriteStrobe(CC_SCAL);
    }
    // IRQ handler
    void IRQHandler(void);
    void TxEndHandler(void);
    void NewPktHandler(void);
};

extern CC_t CC;

// Interrupt
extern "C" {
void EXTI3_IRQHandler(void);
}

#endif	/* _CC1101_H */

