#include <avr/io.h>

#include "delay_util.h"
#include "common.h"

volatile uint16_t TickCounter;

FORCE_INLINE void DelayInit(void) {
    // Millisecond timer initialization, with output compare interrupt enabled
#ifdef __AVR_ATmega88__
    TCCR0A = (1<<WGM01);                    // CTC mode
    TCCR0B = (0<<CS02)|(1<<CS01)|(0<<CS00); // 1 MHz/8 = 125 kHz
    OCR0A  = 125;                           // 125 kHz / 125 = 1000 compares per second
    TIMSK0 |= (1<<OCIE0A);                  // Enable interrupt
#elif defined __AVR_ATmega16A__
    TCCR0 = (1<<WGM01)|(0<<CS02)|(1<<CS01)|(0<<CS00); // CTC mode, 1 MHz/8 = 125 kHz;
    OCR0  = 125;                                      // 125 kHz / 125 = 1000 compares per second
    TIMSK |= (1<<OCIE0);                              // Enable interrupt 
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
#ifdef __AVR_ATmega88__
ISR(TIMER0_COMPA_vect) {
    TickCounter++;
}
#elif defined __AVR_ATmega16A__
ISR(TIMER0_COMP_vect) {
    TickCounter++;
}
#endif
