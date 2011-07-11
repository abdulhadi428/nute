#include "time.h"
#include "main.h"
#include "menu.h"
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
    Time.Minute = 36;
    Time.Hour = 18;
    Time.IsSetCorrectly = false;    // At start-up, time is not set
}

uint8_t TimeGetMinuteUnits(void) {
    uint8_t Result = Time.Minute;
    while(Result >= 10) Result -= 10;
    return Result;
}

// =========================== Interrupts ======================================
// Time counter
ISR(TIMER2_OVF_vect) {
    wdt_enable(WDTO_1S);
    if((EState == StSetTimeHours) || (EState == StSetTimeMinTens) || (EState == StSetTimeMinUnits)) return;
    Time.SecondPassed = true;
    Time.Second++;
    if(Time.Second > 59) {
        Time.Second = 0;
        Time.Minute++;
        if(Time.Minute > 59) { // 24 HyperMinutes in one hour
            Time.Minute = 0;
            Time.Hour++;
            if(Time.Hour > 23) {
                Time.Hour = 0;
                EVENT_NewDay();
            }
            EVENT_NewHour();
        } // if minute > 59
        EVENT_NewMinute();
    }// Second
    EVENT_NewSecond();
}
