/*
 * File:   leds_pca.h
 * Author: Kreyl
 *
 * Created on May 31, 2011, 8:44 PM
 */

#ifndef LEDS_PCA_H
#define	LEDS_PCA_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "i2c_mgr.h"

#define LED_OE_PIN      GPIO_Pin_3
#define LED_I2C_ADDR    0x1B

// PCA registers
struct LedsPkt_t {
    uint8_t ControlReg;
    // Registers
    uint8_t Mode1;
    uint8_t Mode2;
    uint8_t PWM[16];    // PWM channels
    uint8_t GrpPWM;
    uint8_t GrpFreq;
    uint8_t LEDOut[4];
};
#define LEDS_PKT_SIZE   sizeof(LedsPkt_t)

class Leds_t {
private:
    I2C_Cmd_t i2cCmd;
public:
    LedsPkt_t FPkt;
    void Init(void);
    //void Task(void);
    void SendCmd(void) { i2cMgr.AddCmd(i2cCmd); }
    void OutputEnable (void) { GPIOB->BRR  = LED_OE_PIN; }
    void OutputDisable(void) { GPIOB->BSRR = LED_OE_PIN; }
};

extern Leds_t Leds;

#endif	/* LEDS_PCA_H */

