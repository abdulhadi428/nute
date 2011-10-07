/*
 * File:   battery.h
 * Author: Kreyl Laurelindo
 *
 * Created on 9 �?юнь 2010 г., 23:43
 */

#ifndef _BATTERY_H
#define	_BATTERY_H

#include <avr/io.h>

// ============================== Defins =======================================
#define BAT_ADC_P       PA3
#define BAT_ADC_DDR     DDRA
#define BAT_ADC_PORT    PORTA
#define BAT_ADC_PIN     PINA

// Number of measures as power of 2: 0 means 1, 1 means 2, 3 means 8 and so on
// Needed for averaging of values
#define BAT_POWER_OF_MEASURES   3   // Power of 2
#define BAT_NUMBER_OF_MEASURES  (1<<BAT_POWER_OF_MEASURES)

#define BAT_U_DISCHARGED    828
#define BAT_U_FULL          924 // About 12 V

// ============================== Types ========================================
extern uint16_t BatteryADCValue;

#define BATTERY_OK()                (BatteryADCValue >= BAT_U_DISCHARGED)
#define ADC_MEASUREMENT_COMPLETED() bit_is_clear (ADCSRA, ADSC)
// Enable ADC, no IRQ, CLK/8 = 1 Mhz/8 = 125 kHz
//#define ADC_START_MEASUREMENT()     ADCSRA = (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)
// Enable ADC, no IRQ, CLK/32 = 4 Mhz/32 = 125 kHz
#define ADC_START_MEASUREMENT()     ADCSRA = (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0)
#define ADC_DISABLE()               ADCSRA = 0
// Internal 2.56 V, not left-adj, ADC7
//#define ADC_REF_ENABLE()            ADMUX = (1<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)
// External, not left-adj, ADC3
#define ADC_REF_ENABLE()            ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0)
#define ADC_REF_DISABLE()           ADMUX = 0

// ============================ Prototypes =====================================
void BatteryMeasure(void);
uint8_t BatteryGaugeValue(void);

#endif	/* _BATTERY_H */

