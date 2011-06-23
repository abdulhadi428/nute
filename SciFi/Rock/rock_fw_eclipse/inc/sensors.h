/*
 * File:   sensors.h
 * Author: Kreyl
 *
 * Created on June 19, 2011, 11:59 AM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"

#include "i2c_mgr.h"
#include "acc_mma.h"
#include "delay_util.h"

// Constants
#define ADC_REQUEST_PERIOD      500  // ms

enum BatteryState_t {BatOk, BatHalf, BatEmpty};
enum Acceleration_t {gX, gY, gZ, gNone};

// State of sensors
struct SnsState_t {
    bool KeyTouched[3];
    bool MagnetNear;
    Acceleration_t Acceleration;
};

class Sns_t {
private:
    uint32_t Timer;
    BatteryState_t Battery;
    SnsState_t OldState;
    // Sensors
    bool Touched (uint8_t Indx);
    bool MagnetNear(void)     { return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13); }
    void MeasureBattery(void);
    bool SensorsStateChanged(void);
public:
    union {
        uint16_t ADCValues[2];
        struct {
            uint16_t BatteryADC, LumiADC;
        };
    };
    // General
    void Init(void);
    void Task(void);
    void PowerOn (void) { GPIOC->BSRR = GPIO_Pin_14; }
    void PowerOff(void) { GPIOC->BRR  = GPIO_Pin_14; }
};

extern Sns_t ESns;
extern SnsState_t SnsState;

// Sensors state change event
void EVENT_SensorsStateChanged(void);

// Display sensors
void SnsVerbose(void);

#endif	/* SENSORS_H */

