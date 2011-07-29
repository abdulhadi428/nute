/*
 * File:   calma3c.h
 * Author: Laurelindo
 *
 * Created on 20 Декабрь 2009 г., 0:16
 */

#ifndef _CALMA3C_H
#define	_CALMA3C_H

#include <avr/io.h>
#include <inttypes.h>

// =============================== General =====================================
#define PKT_ID0_CALE    0xCA
#define PKT_ID1_CALE    0x1E

// ================================= Sensors ===================================
// Timings
#define SENS_POLL_TIME  99 // ms
#define SENS_BOTHDOWN_IGNORE_TIME    999 // ms
#define SENS_UF_TOGGLE_IGNORE_TIME   999 // ms
#define SENS_HOLD_TICK_TIMEOUT  499 //ms

// Ports & pins
#define SENS_UP     PC1
#define SENS_DOWN   PC0
#define SENS_UF_SWITCH PC2
//#define SENS_PWR    PC5
#define SENS_DDR    DDRC
#define SENS_PORT   PORTC
#define SENS_PIN    PINC

// state mashine states
#define SENS_STATE_START 1
#define SENS_STATE_UP   2
#define SENS_STATE_DOWN 3
#define SENS_STATE_UF_SWITCH    4
#define SENS_STATE_BOTH         5
#define SENS_COVER_CLOSED		6
#define SENS_STATE_WHATE_COVER_OPEN		7


// cover hol sensor
#define COVER_DDR    DDRB
#define COVER_PORT   PORTB
#define COVER_P    PB7
#define COVER_PIN    PINB

// cristal hol sensor
#define CRISTAL_DDR    DDRC
#define CRISTAL_PORT   PORTC
#define CRISTAL_P      PC3
#define CRISTAL_PIN    PINC
#define CRISTAL_IS_INSERT()     bit_is_set (CRISTAL_PIN, CRISTAL_P)

// IRQ датчик крышки на PB6
#define COVER_IRQ_SETUP_MASK() PCMSK0 |=  (1<<PCINT7)
#define COVER_IRQ_ENABLE()     PCICR  |=  (1<<PCIE0)   // Handle at PB6 == PCINT6 => PCIE0
#define COVER_IRQ_DISABLE()    PCICR  &= ~(1<<PCIE0)
#define COVER_IRQ_CLEAR()      PCIFR  |=  (1<<PCIF0)
//#define COVER_IRQ_SETUP_MASK() PCMSK1 |=  (1<<PCINT12)
//#define COVER_IRQ_ENABLE()     PCICR  |=  (1<<PCIE1)   // Handle at PC4 == PCINT12 => PCIE1
//#define COVER_IRQ_DISABLE()    PCICR  &= ~(1<<PCIE1)
//#define COVER_IRQ_CLEAR()      PCIFR  |=  (1<<PCIF1)

// Pseudo-functions
#define SENS_UP_IS_ON()     bit_is_set (SENS_PIN, SENS_UP)
#define SENS_DOWN_IS_ON()   bit_is_set (SENS_PIN, SENS_DOWN)
#define SENS_UF_SWITCH_IS_ON() bit_is_set (SENS_PIN, SENS_UF_SWITCH)
#define COVER_IS_UP()     bit_is_set (COVER_PIN, COVER_P)

 
// ================================== Light ====================================
// Timings
#define LED_STEP_DELAY  5  // ms
// "Shutdown" color
#define CLR_SHDN_RED    7
#define CLR_SHDN_GREEN  0
#define CLR_SHDN_BLUE   0
#define CLR_SHDN_UF     0

// Ports & pins
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define RED_P       PD3
#define GREEN_P     PD5
#define BLUE_P      PD6
#define LED_UF_DDR     DDRB
#define LED_UF_PORT    PORTB
#define UF_P           PB1

/*
#define LED_PWR_P       PB7
#define LED_PWR_DDR     DDRB
#define LED_PWR_PORT    PORTB
#define LED_PWR_ON()    LED_PWR_DDR |=  (1<<LED_PWR_P)
#define LED_PWR_OFF()   LED_PWR_DDR &= ~(1<<LED_PWR_P)
*/

#define LED_RED_DISABLE()  TCCR2A &= ~((1<<COM2B1)|(1<<COM2B0)); 
#define LED_RED_ENABLE()    TCCR2A |=  ((1<<COM2B1)|(0<<COM2B0));

#define LED_GREEN_DISABLE() TCCR0A &= ~((1<<COM0B1)|(1<<COM0B0));
#define LED_GREEN_ENABLE()  TCCR0A |=  ((1<<COM0B1)|(0<<COM0B0));

#define LED_BLUE_DISABLE()  TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0));
#define LED_BLUE_ENABLE()   TCCR0A |=  ((1<<COM0A1)|(0<<COM0A0));
#define LED_UF_DISABLE()  TCCR1A &= ~((1<<COM1A1)|(1<<COM1A0));
#define LED_UF_ENABLE()   TCCR1A |=  ((1<<COM1A1)|(0<<COM1A0));

// =============================== Prototypes ==================================
// General
void GeneralInit(void);

void SetDesiredColor (uint8_t ARed, uint8_t AGreen, uint8_t ABlue, uint8_t AUf);
void SetTableColor (void);

// Tasks
void SENS_Task (void);
void Light_Task(void);
void CC_Task (void);
void Sleep_Task (void);

// Events
void EVENT_BothTouched(void);
void EVENT_UfSwitchTouched(void);
void EVENT_DownHoldTick(void);
void EVENT_UpHoldTick(void);

void EVENT_CoverClosed(void);
void EVENT_CoverOpened(void);

#endif	/* _CALMA3C_H */

