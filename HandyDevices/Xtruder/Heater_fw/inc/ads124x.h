/*
 * ads124x.h
 *
 *  Created on: 21.08.2012
 *      Author: kreyl
 */

#ifndef ADS124X_H_
#define ADS124X_H_

#include "kl_lib.h"

// Channels. Ain7 is alwais negative
#define ADS_CH_1    0x07
#define ADS_CH_2    0x17
#define ADS_CH_3    0x27
#define ADS_CH_4    0x37
#define ADS_CH_5    0x47
#define ADS_CH_6    0x57
#define ADS_CH_7    0x67

// ==== Measurements ====
/*
 * Every channel requires two coefficients, as t = U / a + b.
 * Two temperatures and two voltages should be provided.
 */
#define ADS_CH1_T1  39.3
#define ADS_CH1_T2  100
#define ADS_CH1_U1  4740000
#define ADS_CH1_U2  5690000
#define ADS_CH1_A   (int32_t)((ADS_CH1_U2 - ADS_CH1_U1) / (ADS_CH1_T2 - ADS_CH1_T1))
#define ADS_CH1_B   (int32_t)(ADS_CH1_T1 - (ADS_CH1_U1 / ADS_CH1_A))

struct AdsChData_t {
    uint8_t Mux;
    int32_t a, b;
};

const AdsChData_t AdsChs[] = {
        { ADS_CH_1, ADS_CH1_A, ADS_CH1_B},
};
#define ADS_CH_COUNT    (countof(AdsChs))

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

class Ads124x_t {
private:
    uint8_t IChIndx;
    // GPIOs
    void PwdnHi(void) { klGpioSetByMsk  (GPIOA, GPIO_Pin_2); }
    void PwdnLo(void) { klGpioClearByMsk(GPIOA, GPIO_Pin_2); }
    void CsHi(void)   { klGpioSetByMsk  (GPIOA, GPIO_Pin_4); }
    void CsLo(void)   { klGpioClearByMsk(GPIOA, GPIO_Pin_4); }
    klPinIrq_t Drdy;
    // SPI
    void IReplyDelay(void) { Delay.Loop(ADS_REPLY_DELAY_TICKS);  }
    uint8_t WriteReadByte(uint8_t AByte);
    // Data flow
    void WriteStrobe(uint8_t AStrobe);
    void WriteReg(uint8_t AAddress, uint8_t AData);
    //void ReadReg(uint8_t AAddress, uint8_t ACount, uint8_t *PData);
    // High-level
    void Reset(void) { WriteStrobe(0b11111110); }
    void SelfCalibrate(void);
    void SetGain(uint8_t AGain) { WriteReg(ADS_REG_SETUP, AGain); } // Do not use burnout currents
    void SetChannel(uint8_t AChannel) { WriteReg(ADS_REG_MUX, AChannel); }
public:
    bool NewData;
    int32_t Temperature[ADS_CH_COUNT];
    void Init(void);

    void IrqHandler(void);
};

extern Ads124x_t Ads;

// Interrupt
extern "C" {
void EXTI3_IRQHandler(void);
}

#endif /* ADS124X_H_ */
