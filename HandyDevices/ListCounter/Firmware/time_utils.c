#include "time_utils.h"

volatile uint16_t TickCounter;

void TimerInit (void) {
    // Millisecond timer initialization, with output compare interrupt enabled
    TCCR1A = 0;
    TCCR1B = (0<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10); // CTC mode, 1 MHz/8 = 125 kHz
    OCR1A  = 125;                           // 125 kHz / 125 = 1000 compares per second
    TIMSK1 |= (1<<OCIE1A);                  // Enable interrupt
}

bool TimerDelayElapsed (uint16_t *AVar, const uint16_t ADelay) {
    uint16_t ttmp;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        ttmp = TickCounter;
    }
    if ((ttmp - *AVar) >= ADelay) {
        *AVar = ttmp;
        return true;
    }
    else return false;
}

void TimerResetDelay (uint16_t *AVar) {
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        *AVar = TickCounter;
    }
}

/*
void PrintTime (struct Time_t *ATime) {
    printf_P (PSTR("\r\n %d:%d:%d"), ATime->Hour, ATime->Minute, ATime->Second);
}

void IncSecond (struct Time_t *ATime) {
    if (ATime->Second == 59) {
        ATime->Second = 0;
        if (ATime->Minute == 59) {
            ATime->Minute = 0;
            if (ATime->Hour == 23) ATime->Hour = 0;
            else ATime->Hour++;
        } // if min==59
        else ATime->Minute++;
    } // if s==59
    else ATime->Second++;
}
*/

// ================================ Interrupts =================================
// ISR for the timer 0 compare vector. This ISR fires once each millisecond,
// and increments the tick counter.
ISR (TIMER1_COMPA_vect, ISR_BLOCK) {
	TickCounter++;
}