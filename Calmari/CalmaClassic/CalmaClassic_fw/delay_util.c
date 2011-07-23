#include <avr/io.h>

#include "delay_util.h"
#include "common.h"

// Choose OCR value
#if F_CPU == 1000000
    #define DELAY_TCCR  (0<<CS02)|(1<<CS01)|(0<<CS00)   // 1 MHz/8 = 125 kHz;
    #define DELAY_OCRA  125  // 125 kHz / 125 = 1000 compares per second
#elif F_CPU == 4000000
    #define DELAY_TCCR  (0<<CS02)|(1<<CS01)|(1<<CS00)   // 4 MHz/64 = 62.5 kHz;
    #define DELAY_OCRA  62
#else
    #error "Delay_util: Set correct frequency in project settings!"
#endif

volatile uint16_t TickCounter;

FORCE_INLINE void DelayInit(void) {
    // Millisecond timer initialization, with output compare interrupt enabled
#ifdef __AVR_ATmega88__
    TCCR0A = (1<<WGM01);                    // CTC mode
    TCCR0B = (0<<CS02)|(1<<CS01)|(0<<CS00); // 1 MHz/8 = 125 kHz
    OCR0A  = 125;                           // 125 kHz / 125 = 1000 compares per second
    TIMSK0 |= (1<<OCIE0A);                  // Enable interrupt
#elif defined __AVR_ATmega16A__
    TCCR0 = (1<<WGM01)|DELAY_TCCR; // CTC mode
    OCR0  = DELAY_OCRA;
    TIMSK |= (1<<OCIE0);                              // Enable interrupt
#elif defined __AVR_ATtiny45__
    TCCR0A = (1<<WGM01);
    TCCR0B = DELAY_TCCR;
    OCR0A = DELAY_OCRA;
    TIMSK |= (1<<OCIE0A);
#endif
}

bool DelayElapsed(uint16_t *AVar, const uint16_t ADelay) {
    bool Result = false;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        if ((TickCounter - *AVar) >= ADelay) {
            *AVar = TickCounter;   // Reset delay
            Result = true;
        }
    } // Atomic
    return Result;
}

void DelayReset(uint16_t *AVar) {
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        *AVar = TickCounter;
    }
}

// ================================ Interrupts =================================
// Delay counter
#if defined __AVR_ATmega88__ || defined __AVR_ATtiny45__
ISR(TIMER0_COMPA_vect) {
    TickCounter++;
}
#elif defined __AVR_ATmega16A__
ISR(TIMER0_COMP_vect) {
    TickCounter++;
}
#endif
