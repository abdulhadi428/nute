/*
 * battery.h
 *
 *  Created on: 27.07.2011
 *      Author: Kreyl
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include <avr/io.h>

#define BAT_CHRG_P          PC4
#define BAT_CHRG_DDR        DDRC
#define BAT_CHRG_PORT       PORTC
#define BAT_CHRG_PIN        PINC

#define BAT_CHARGE_T        801
#define ADC_MEASURE_T       999
#define ADC_PREPARE_T       50

#define BAT_U_DISCHARGED    703

// Enable ADC, no IRQ, CLK/8 = 1 Mhz/8 = 125 kHz
#define ADC_START_MEASUREMENT()     ADCSRA = (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
#define ADC_DISABLE()               ADCSRA = 0
#define ADC_COMPLETED()             bit_is_clear (ADCSRA, ADSC)
// Internal 1.1V, not left-adj, ADC7
#define ADC_REF_ENABLE()            ADMUX = (1<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)
#define ADC_REF_DISABLE()           ADMUX = 0
// Number of measures as power of 2: 0 means 1, 1 means 2, 3 means 8 and so on
// Needed for averaging of values
#define ADC_POWER_OF_MEASURES   3   // Power of 2
#define ADC_NUMBER_OF_MEASURES  (1<<ADC_POWER_OF_MEASURES)

enum Bat_ADC_State_t {ADCNoMeasure, ADCInit, ADCMeasuring};

class Battery_t {
    uint16_t ChargeTimer, ADCTimer;
    uint8_t MeasuresCounter;
    uint16_t ADCValue;
    Bat_ADC_State_t ADCState;
    bool IsChargingSignalled(void) { return bit_is_clear (BAT_CHRG_PIN, BAT_CHRG_P); }
public:
    bool IsCharging;
    void Init(void);
    void Task(void);
    bool IsDischarged(void) { return (ADCValue < BAT_U_DISCHARGED); }
};

void EVENT_ChargeStarted(void);
void EVENT_ChargeEnded(void);

void EVENT_ADCMeasureCompleted(void);

extern Battery_t Battery;

#endif /* BATTERY_H_ */
