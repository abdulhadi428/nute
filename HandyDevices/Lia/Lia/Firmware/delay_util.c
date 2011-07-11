#include <avr/io.h>

#include "delay_util.h"
#include "common.h"

volatile uint16_t TickCounter;

FORCE_INLINE void DelayInit(void) {
    // Millisecond timer initialization, with output compare interrupt enabled
    TCCR0A = (1<<WGM01);                    // CTC mode
    TCCR0B = (0<<CS02)|(1<<CS01)|(0<<CS00); // 1 MHz/8 = 125 kHz
    OCR0A  = 125;                           // 125 kHz / 125 = 1000 compares per second
    TIMSK0 |= (1<<OCIE0A);                  // Enable interrupt
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
ISR(TIMER0_COMPA_vect) {
    TickCounter++;
}
