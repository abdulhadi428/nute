#include "time.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

struct Time_t Time;

// ========================= Implementation ====================================
void TimeInit(void) {
    // Timer2: realtime clock counter
    ASSR  = (1<<AS2);				// Set Async mode of the timer
    //TCCR2 = (0<<CS22)|(0<<CS21)|(1<<CS20);	// DEBUG: no division
    //TCCR2B = (0<<CS22)|(1<<CS21)|(0<<CS20);	// DEBUG: div 8
    //TCCR2 = (0<<CS22)|(1<<CS21)|(1<<CS20);	// DEBUG: div 32
    TCCR2 = (1<<CS22)|(0<<CS21)|(1<<CS20);	// RELEASE: div 128, once per second
    TIMSK |= (1<<TOIE2);

    // Setup initial values
    Time.Second = 0;
    Time.MinTens = 3;
    Time.MinUnits = 6;
    Time.Hour = 9;
    Time.IsSetCorrectly = false;    // At start-up, time is not set
    Time.FlowEnabled = true;
}

// =========================== Interrupts ======================================
// Time counter
ISR(TIMER2_OVF_vect) {
    wdt_enable(WDTO_1S);
    if(!Time.FlowEnabled) return;
    Time.SecondPassed = true;
    Time.Second++;
    if(Time.Second > 59) {
        Time.Second = 0;
        Time.MinUnits++;
        if(Time.MinUnits > 9) {
            Time.MinUnits = 0;
            Time.MinTens++;
            if(Time.MinTens > 5) {
                Time.MinTens = 0;
                Time.Hour++;
                if(Time.Hour > 23) {
                    Time.Hour = 0;
                    EVENT_NewDay();
                }
                EVENT_NewHour();
            } // if(Time.MinTens > 5)
        } // if MinUnits > 59
        EVENT_NewMinute();
    }// Second
    EVENT_NewSecond();
}
