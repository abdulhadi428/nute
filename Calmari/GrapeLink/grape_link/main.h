/* 
 * File:   calma3c.h
 * Author: Laurelindo
 *
 * Created on 20 Декабрь 2009 г., 0:16
 */

#ifndef _GRAPE_H
#define	_GRAPE_H

// =============================== General =====================================
#define EE_GRAPE_ADDR   (uint8_t*)4

#define PBHI    PORTB |= (1<<PB0)
#define PBLO    PORTB &= ~(1<<PB0)

#define BTN_DDR     DDRD
#define BTN_PORT    PORTD
#define BTN_PIN     PIND
#define BTN         PD0
#define BTN_IS_PRESSED()    bit_is_clear(BTN_PIN, BTN)

// Mood leds
#define MDLED_DDR   DDRC
#define MDLED_PORT  PORTC
#define MDLED_RED   PC6
#define MDLED_GREEN PC7
#define MDLED_RED_ON()      PORTC |=  (1<<MDLED_RED)
#define MDLED_RED_OFF()     PORTC &= ~(1<<MDLED_RED)
#define MDLED_GREEN_ON()    PORTC |=  (1<<MDLED_GREEN)
#define MDLED_GREEN_OFF()   PORTC &= ~(1<<MDLED_GREEN)

// ================================== Timer ====================================
// Divider = 256, resulting in 64 uS per pulse
#define TCCR1B_ON   (1<<WGM13)|(1<<WGM12)|(1<<CS02)|(0<<CS01)|(0<<CS00)
#define TCCR1B_OFF  (1<<WGM13)|(1<<WGM12)|(0<<CS02)|(0<<CS01)|(0<<CS00)

#define CYC_TIMER_START()    TCCR1B = TCCR1B_ON
#define CYC_TIMER_STOP()     TCCR1B = TCCR1B_OFF

#define CYCLE_COUNT         4
#define PKT_DURATION        351     // 22 ms/64us
#define SUBCYCLE_DURATION   PKT_DURATION*9


// =============================== Prototypes ==================================
// General
void GeneralInit(void);

// Tasks
void CC_Task (void);
void LEDs_Task(void);
void Btn_Task(void);

// General
void ChooseNextLED(void);

// Events
void EVENT_NewPacket(void);

#endif	/* _GRAPE_H */

