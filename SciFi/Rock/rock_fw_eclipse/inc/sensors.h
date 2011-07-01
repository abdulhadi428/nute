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
#include "misc.h"

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
    bool IEnabled;
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
    void Enable(void)  { IEnabled = true; }
    void Disable(void) { IEnabled = false; }
    void PowerOn (void) { GPIOC->BSRR = GPIO_Pin_14; }
    void PowerOff(void) { GPIOC->BRR  = GPIO_Pin_14; }
};

// ================================== Ksotar's =================================
// Defines
#define IRS_HEADER_T        2400    // uS
#define IRS_ZERO_T          600     // uS
#define IRS_ONE_T           1200    // uS
#define IRS_DEVIATION_T     300     // uS
#define IRS_PKT_LENGTH      14

class IRSirc_t {
public:
    uint16_t Pkt;
    void Init(void);
    void Task(void);
    void IRQHandler(void);
private:
    uint8_t BitCounter;
    bool NewPacket;
    bool IsHeader(const uint16_t APulse) { return ((APulse > (IRS_HEADER_T - IRS_DEVIATION_T)) && (APulse < (IRS_HEADER_T + IRS_DEVIATION_T))); }
    bool IsZero  (const uint16_t APulse) { return ((APulse > (IRS_ZERO_T   - IRS_DEVIATION_T)) && (APulse < (IRS_ZERO_T   + IRS_DEVIATION_T))); }
    bool IsOne   (const uint16_t APulse) { return ((APulse > (IRS_ONE_T    - IRS_DEVIATION_T)) && (APulse < (IRS_ONE_T    + IRS_DEVIATION_T))); }
    void ResetPkt(void) { BitCounter=0; NewPacket=false; Pkt=0; }
};

#ifdef __cplusplus
extern "C" {
#endif
void TIM3_IRQHandler(void);
#ifdef __cplusplus
}
#endif

// =============================================================================
extern Sns_t ESns;
extern SnsState_t SnsState;
extern IRSirc_t EIRSirc;

// Sensors state change event
void EVENT_SensorsStateChanged(void);

// Display sensors
void SnsVerbose(void);

#endif	/* SENSORS_H */

