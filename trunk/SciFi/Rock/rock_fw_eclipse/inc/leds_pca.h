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
#include "delay_util.h"

#define LED_OE_PIN      GPIO_Pin_3
#define LED_I2C_ADDR    0x1B

// Colors
struct Color_t {
    uint8_t Red, Green, Blue;
};
#define COLOR_MAX       250

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

// Modes of operation
enum LedModes_t {lmEqualAll, lmBlinkAll, lmRunning, lmRunAndBlink};

class Leds_t {
private:
    LedsPkt_t FPkt;
    I2C_Cmd_t i2cCmd;
    uint32_t Timer, CurrentDelay, Timer2;
    uint8_t LedID;
    // Colors array
    Color_t *Colors[5];
    void SetAll(uint8_t APWM) { for (uint8_t i=0; i<=14; i++) FPkt.PWM[i] = APWM; }
public:
    // Light effects settings
    LedModes_t Mode;
    Color_t RunColor, BlinkColor;
    uint32_t RunDelay, BlinkOnTime, BlinkOffTime;
    uint8_t RunLedCount;
    bool LedIsOn;
    // General methods
    void Init(void);
    void Task(void);
    void SendCmd(void) { i2cMgr.AddCmd(i2cCmd); }
    void OutputEnable (void) { GPIOB->BRR  = LED_OE_PIN; }
    void OutputDisable(void) { GPIOB->BSRR = LED_OE_PIN; }
    // LCD backlight
    void BacklightOn(void);
    void BacklightOff(void);
    // Light effects
    void SetEqualAll(uint8_t AValue);
    void SetRunning(void);
    void SetBlinkAll(void);
    void SetRunningWithBlink(void);
};

extern Leds_t Leds;

#endif	/* LEDS_PCA_H */
