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

#define WATER_SEARCH_PERIOD         999     // ms
#define WATER_FOUND_ALARM_DELAY     30000   // ms

class WaterSns_t {
private:
    uint32_t SearchTimer, DelayTimer;
public:
    uint16_t ADCValue;
    void Init(void);
    void Task(void);
    // Events

};

extern WaterSns_t WaterSensor;

// Interrupt
extern "C" {
void ADC1_2_IRQHandler(void);
}

#endif /* WATER_SENSOR_H_ */
