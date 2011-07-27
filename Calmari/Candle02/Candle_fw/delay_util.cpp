#include <avr/io.h>

#include "delay_util.h"

Delay_t Delay;
volatile uint16_t TickCounter;

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


void Delay_t::Init(void) {
    // Millisecond timer initialization, with output compare interrupt enabled
#ifdef __AVR_ATmega88__

#ifdef DELAY_USE_TIMER0
    TCCR0A = (1<<WGM01);    // CTC mode
    TCCR0B = DELAY_TCCR;
    OCR0A  = DELAY_OCRA;
    TIMSK0 |= (1<<OCIE0A);  // Enable interrupt
#elif defined DELAY_USE_TIMER1
    TCCR1A = 0;
    TCCR1B = (0<<WGM13)|(1<<WGM12)|DELAY_TCCR;
    OCR1A  = DELAY_OCRA;
    TIMSK1 |= (1<<OCIE1A);                  // Enable interrupt
#endif

#elif defined __AVR_ATmega16A__
    TCCR0 = (1<<WGM01)|DELAY_TCCR; // CTC mode
    OCR0  = DELAY_OCRA;
    TIMSK |= (1<<OCIE0);                              // Enable interrupt
#endif
}

bool Delay_t::Elapsed(uint16_t *AVar, const uint16_t ADelay) {
    bool Result = false;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        if ((TickCounter - *AVar) >= ADelay) {
            *AVar = TickCounter;   // Reset delay
            Result = true;
        }
    } // Atomic
    return Result;
}

void Delay_t::Bypass(uint16_t *AVar, const uint16_t ADelay) {
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        *AVar = TickCounter - ADelay - 1;   // Reset delay
    } // Atomic
}

void Delay_t::Reset(uint16_t *AVar) {
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        *AVar = TickCounter;
    }
}

// ================================ Interrupts =================================
// Delay counter
#ifdef __AVR_ATmega88__
#ifdef DELAY_USE_TIMER0
ISR(TIMER0_COMPA_vect) {
    TickCounter++;
}
#elif defined DELAY_USE_TIMER1
ISR (TIMER1_COMPA_vect, ISR_BLOCK) {
    TickCounter++;
}
#endif


#elif defined __AVR_ATmega16A__
ISR(TIMER0_COMP_vect) {
    TickCounter++;
}
#endif
