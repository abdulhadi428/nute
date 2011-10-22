/*
 * water_sensor.h
 *
 *  Created on: 14.10.2011
 *      Author: Kreyl
 */

#ifndef WATER_SENSOR_H_
#define WATER_SENSOR_H_

#include "misc.h"
#include "stm32f10x_gpio.h"
#include "delay_util.h"
#include "arm_common.h"

#define WATER_SEARCH_PERIOD         999     // ms
#define WATER_FOUND_ALARM_DELAY     30000   // ms
#define WATER_THRESHOLD             2007    // ADC value
#define WATER_COUNTER_MAX           7       // Count of water detections

class WaterSns_t {
private:
    uint32_t SearchTimer, DelayTimer;
    uint32_t ThresholdCounter;
    bool WaterIsHere, WaterWasHere;
public:
    uint16_t ADCValue;
    void Init(void);
    void Task(void);
    void IRQHandler(void);
    // Events
    ftVoid_Void EVT_WaterHere;
};

extern WaterSns_t WaterSensor;

// Interrupt
extern "C" {
void ADC1_IRQHandler(void);
}

#endif /* WATER_SENSOR_H_ */
