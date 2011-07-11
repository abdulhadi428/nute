#include <avr/io.h>
#include "leds.h"
#include "delay_util.h"
#include "common.h"
#include <avr/interrupt.h>

uint8_t Reds  [RED_COUNT]   = {(1<<PD4), (1<<PD5), (1<<PD6), (1<<PD7)};
uint8_t Greens[GREEN_COUNT] = {(1<<PA0), (1<<PA1), (1<<PA2), (1<<PA3), (1<<PA4), (1<<PA5)};
struct Leds_t ELeds;

void LedsInit(void) {
    RED_DDR   |= Reds[0]   | Reds[1]   | Reds[2]   | Reds[3];
    GREEN_DDR |= Greens[0] | Greens[1] | Greens[2] | Greens[3] | Greens[4] | Greens[5];
    ELeds.LedColor = Red;
    ELeds.LedRedNumber = 0;
    DelayReset(&ELeds.Timer);
    PWMStart();
}

FORCE_INLINE void LedSwitch(enum LedState_t AState) {
    if (ELeds.LedColor == Red) {
        if (AState == On) RED_PORT |=  Reds[ELeds.LedRedNumber];
        else              RED_PORT &= ~Reds[ELeds.LedRedNumber];
    }
    else {
        if (AState == On) GREEN_PORT |=  Greens[ELeds.LedGreenNumber];
        else              GREEN_PORT &= ~Greens[ELeds.LedGreenNumber];
    }
}

FORCE_INLINE void PWMStop(void) {
    TCCR2 = 0;
}
FORCE_INLINE void PWMStart(void) {
    OCR2 = 0;
    // Timer: normal mode, fastest clock
    //TCCR2 = (0<<WGM21)|(0<<WGM20)|(1<<CS20);
    TCCR2 = (0<<WGM21)|(0<<WGM20)|(1<<CS21)|(1<<CS20);
    TIMSK |= (1<<OCIE2)|(1<<TOIE2);
}
FORCE_INLINE void PWMSet(uint8_t APWM) {
    OCR2 = APWM;
}

// ================================ IRQs =======================================
// Switch LED on
ISR(TIMER2_OVF_vect) {
    LedSwitch(On);
}
// Switch LED off
ISR(TIMER2_COMP_vect) {
    LedSwitch(Off);
}

