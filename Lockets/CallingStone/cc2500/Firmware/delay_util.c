#include <avr/io.h>

#include "delay_util.h"
#include "common.h"

volatile uint16_t TickCounter;

FORCE_INLINE void DelayInit(void) {
    // Millisecond timer initialization, with output compare interrupt enabled
    TCCR0 = (1<<WGM01)|(0<<WGM00)|(0<<CS02)|(1<<CS01)|(0<<CS00);// CTC mode, 1 MHz/8 = 125 kHz
    OCR0  = 125;                                                // 125 kHz / 125 = 1000 compares per second
    TIMSK |= (1<<OCIE0);                                        // Enable interrupt
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
ISR (TIMER0_COMP_vect, ISR_BLOCK) {
    TickCounter++;
}