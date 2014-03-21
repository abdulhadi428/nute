/*
 * File:   main.c
 * Author: Kreyl
 *
 * Created on April 11, 2011, 7:29 PM
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "common.h"
#include "delay_util.h"

// =================================== Types ===================================
struct {
    uint16_t Timer;
    bool IsOn;
} Avada;

uint16_t BuzzTimer;

struct {
    uint16_t Timer;
    bool IsPressed;
} EKey;

void BuzzReset(void);
 
// =============================== Implementation ==============================
int main(void) {
    // Initialization
    wdt_enable(WDTO_500MS);
    DelayInit();
    // Key
    KEY_DDR  &= ~(1<<KEY_P);   // Make it input
    KEY_PORT |=  (1<<KEY_P);   // Switch on pull-up
    DelayReset(&EKey.Timer);
    EKey.IsPressed = false;

    // Avada
    AVADA_OFF();    // This first to avoid undesired kills
    AVADA_DDR |= (1<<AVADA_P);
    Avada.IsOn = false;
    // ==== Buzzer ====
    BUZZER_DDR |= (1<<BUZZER_P);
    BUZZER_PORT &= ~(1<<BUZZER_P);
    // Setup Timer1: main generator
    DDRB |= (1<<PB1); // Enable PWM output
    TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(0<<WGM11)|(0<<WGM10); // PWM, phase & freq correct
    TCCR1B = (1<<WGM13)|(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
    BuzzReset();
    sei();

    // ========= Main cycle ==========
    while (1) {
        wdt_reset();
        Key_Task();
        Buzz_Task();
        Avada_Task();
    }
}

FORCE_INLINE void BuzzReset(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        ICR1 = PERIOD_MAX;
        OCR1A = VOLUME_MIN;
    }
    DelayReset(&BuzzTimer);
}

// ================================== Tasks ====================================
FORCE_INLINE void Key_Task(void) {
    if (!DelayElapsed(&EKey.Timer, KEY_POLL_TIMEOUT)) return;
    if ((!EKey.IsPressed) && KEY_IS_DOWN()) {
        EKey.IsPressed = true;
        EVENT_KeyPressed();
    }
    else if (EKey.IsPressed && (!KEY_IS_DOWN())) {
        EKey.IsPressed = false;
    }
}
FORCE_INLINE void Buzz_Task(void){
    if (ICR1 <= PERIOD_MIN) return; // Highest frequency achieved
    if (!DelayElapsed(&BuzzTimer, BUZZ_DELAY)) return;  // Not a time for changes
    // Delay passed, increase frequency
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ICR1--;
        if (OCR1A < VOLUME_MAX) OCR1A++;
    }
}
FORCE_INLINE void Avada_Task(void){
    if (Avada.IsOn) {   // We are shining with green light
        // Check if time to hide
        if (DelayElapsed(&Avada.Timer, SHINE_DURATION)){
            AVADA_OFF();    // All is finished now
            Avada.IsOn = false;
            BuzzReset();
        }
    } // if
}
// =============================== Events ======================================
void EVENT_KeyPressed(void){
    DelayReset(&Avada.Timer);
    Avada.IsOn = true;
    AVADA_ON(); // Say bye-bye...
}

