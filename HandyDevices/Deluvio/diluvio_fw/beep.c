#include "beep.h"
#include "common.h"
#include "delay_util.h"
#include <avr/io.h>
#include <stdbool.h>

struct Beep_t EBeep;

void BeepOn(void);
void BeepOff(void);

// ============================ Implementation =================================
FORCE_INLINE void Task_Beep(void) {
    // Check if time to silence
    if(EBeep.IsYelling) 
        if(DelayElapsed(&EBeep.Timer, EBeep.Duration)) BeepOff();
}

FORCE_INLINE void Beep(uint16_t ADuration) {
    EBeep.Duration = ADuration;
    DelayReset(&EBeep.Timer);
    BeepOn();
}

FORCE_INLINE void BeepInit(void) {
    EBeep.IsYelling = false;
    DDRD  |= 1<<PD5;
}

FORCE_INLINE void BeepOn(void) {
    EBeep.IsYelling = true;
    // Setup Timer1
    ICR1L = BEEP_TOP_VALUE;
    OCR1AL = BEEP_TOP_VALUE/2;
    TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(1<<WGM11)|(0<<WGM10);
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
}
FORCE_INLINE void BeepOff(void) {
    EBeep.IsYelling = false;
    TCCR1A = 0;
    TCCR1B = 0;
}
