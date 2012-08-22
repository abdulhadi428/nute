/*
 * ads124x.h
 *
 *  Created on: 21.08.2012
 *      Author: kreyl
 */

#ifndef ADS124X_H_
#define ADS124X_H_

#include "kl_lib.h"

/*
 * ADS124x reply delay = Tosc * 50 = 0.2us * 50 = 10us (for about 5MHz clock).
 * If CPU clock == 8MHz, CPU tics = 10us / (1000000 / 8000000)
 */
#define ADS_REPLY_DELAY_us      ((uint32_t)10)
#define ADS_REPLY_DELAY_TICKS   (uint32_t)((ADS_REPLY_DELAY_us * SystemCoreClock) / 1000000)

// Registers
#define ADS_REG_SETUP   0x00
#define ADS_REG_MUX     0x01
#define ADS_REG_ACR     0x02

// Gain values
#define ADS_GAIN_1      0x00
#define ADS_GAIN_2      0x01
#define ADS_GAIN_4      0x02
#define ADS_GAIN_8      0x03
#define ADS_GAIN_16     0x04
#define ADS_GAIN_32     0x05
#define ADS_GAIN_64     0x06
#define ADS_GAIN_128    0x07

// Channels. Ain7 is alwais negative
#define ADS_CH_1    0x07
#define ADS_CH_2    0x17
#define ADS_CH_3    0x27
#define ADS_CH_4    0x37
#define ADS_CH_5    0x47
#define ADS_CH_6    0x57
#define ADS_CH_7    0x67


class Ads124x_t {
private:
    // GPIOs
    void PwdnHi(void) { klGpioSetByMsk  (GPIOA, GPIO_Pin_2); }
    void PwdnLo(void) { klGpioClearByMsk(GPIOA, GPIO_Pin_2); }
    void CsHi(void)   { klGpioSetByMsk  (GPIOA, GPIO_Pin_4); }
    void CsLo(void)   { klGpioClearByMsk(GPIOA, GPIO_Pin_4); }
    //bool DataIsReady(void) { return klGpioIsClearByMsk(GPIOA, GPIO_Pin_3); }
    klPinIrq_t Drdy;
    // SPI
    void IReplyDelay(void) { Delay.Loop(ADS_REPLY_DELAY_TICKS);  }
    uint8_t WriteReadByte(uint8_t AByte);
    // Data flow
    void WriteStrobe(uint8_t AStrobe);
    void ReadResult(uint8_t *PData);
public:
    uint32_t Value;
    uint8_t ChannelToSet;
    bool NewData;
    void Init(void);
    //void ReadReg(uint8_t AAddress, uint8_t ACount, uint8_t *PData);
    void WriteReg(uint8_t AAddress, uint8_t AData);
    void Reset(void) { WriteStrobe(0b11111110); }
    void SelfCalibrate(void);
    void SetGain(uint8_t AGain) { WriteReg(ADS_REG_SETUP, AGain); } // Do not use burnout currents
    void SetChannel(uint8_t AChannel) { WriteReg(ADS_REG_MUX, AChannel); }

    void IrqHandler(void);
};

extern Ads124x_t Ads;

// Interrupt
extern "C" {
void EXTI3_IRQHandler(void);
}

#endif /* ADS124X_H_ */
