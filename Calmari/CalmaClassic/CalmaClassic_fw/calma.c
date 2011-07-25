/*
 * calma.cpp
 *
 *  Created on: 22.07.2011
 *      Author: Kreyl
 */

#include <avr/wdt.h>
#include <avr/sleep.h>
#include "delay_util.h"
#include "common.h"
#include "calma.h"

// =================================== Types ===================================
struct {
    uint8_t PWM, PWMDesired;
    uint16_t Timer;
    bool IsOn;
} ELED;
struct {
    uint16_t Timer;
    uint16_t Value;
    bool Measuring;
    uint8_t Counter;
} EADC;
struct {
    uint16_t Timer;
    bool IsPressed;
} EKey;

// =============================== Implementation ==============================
int main(void) {
    GeneralInit();

    while(1) {
        wdt_reset();
        LED_Task();
        Key_Task();
        ADC_Task();
        Sleep_Task();
    }
    return 0;
}

void GeneralInit(void) {
    wdt_enable(WDTO_2S);    // Enable WatchDog at 2 s
    ACSR |= (1<<ACD);       // Shutdown comparator
    // Setup IO
    DDRB  = (0<<KEY_P)|(1<<LED_P);
    PORTB = (0<<KEY_P)|(1<<LED_P); // P-Channel transistor, GND-pulled key

    // Init LED
    ELED.PWMDesired = PWM_START_VALUE;    // Light-up at power-on
    ELED.PWM = 0;
    ELED.IsOn = true;
    // ADC
    EADC.Measuring = false;
    // Key
    EKey.IsPressed = false;

    DelayInit();
    sei();
}

// ================================== Tasks ====================================
FORCE_INLINE void Key_Task(void) {
    if (DelayElapsed(&EKey.Timer, KEY_POLL_TIMEOUT)) {
        if ((!EKey.IsPressed) && KEY_IS_DOWN()) {
            EKey.IsPressed = true;
            EVENT_KeyPressed();
        }
        else if (EKey.IsPressed && (!KEY_IS_DOWN())) {
            EKey.IsPressed = false;
        }
    }
}

FORCE_INLINE void LED_Task(void) {
    if (ELED.PWM == ELED.PWMDesired) return;
    if (ELED.PWMDesired < ELED.PWM) {   // Lower PWM
        if (MayChangePWM()) {
            ELED.PWM--;
            OCR1A = ELED.PWM;
        }
        // Workaround hardware PWM bug: LED does not switches off totally
        if (ELED.PWM == 0) PWMDisable();
    }
    else {
        if (ELED.PWM == 0) PWMEnable();
        if (MayChangePWM()) {
            ELED.PWM++;
            OCR1A = ELED.PWM;
        } // if may change
    } // Fade or brighten
}
FORCE_INLINE void ADC_Task(void) {
    // Check if time to measure
    if (!EADC.Measuring) {
        if (DelayElapsed(&EADC.Timer, ADC_TIMEOUT)) { // Start measure
            EADC.Counter = 0;
            EADC.Value = 0;
            EADC.Measuring = true;
            ADC_Start();
        }
    }
    else { // Is measuring
        // Check if conversion completed
        if (bit_is_clear(ADCSRA, ADSC)) {   // completed
            if (EADC.Counter != 0) {        // Discard first conversion
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    EADC.Value += ADC;
                }
            } // if !=0
            EADC.Counter++;
            // Check if measure completed
            if (EADC.Counter > ADC_NUMBER_OF_MEASURES) {
                EADC.Measuring = false;
                DelayReset(&EADC.Timer);
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    EADC.Value >>= ADC_POWER_OF_MEASURES;
                }
                EVENT_ADC_Done();
            } // if measure completed
        } // if conversion completed
    }
}
FORCE_INLINE void Sleep_Task(void) {
    // Check if is off
    if ((ELED.IsOn) || (ELED.PWM != 0) || EKey.IsPressed) return;
    // No need to stop timer as in power-down mode it is stopped
    ADCSRA = 0x00;                      // Stop ADC
    MCUCR = (1<<SE)|(1<<SM1)|(0<<SM0);  // Enable sleep in power-down mode
    GIMSK = (1<<PCIE);                  // }
    PCMSK = (1<<PCINT2);                // } Enable Key IRQ
    wdt_disable();                      // Disable Watchdog
    sleep_cpu();                        // Fall asleep
    // Awake occured
    wdt_enable(WDTO_2S);                // Enable WatchDog at 2 s
    GIMSK = (0<<PCIE);                  // Disable Key IRQ
    MCUCR = (0<<SE)|(0<<SM1)|(0<<SM0);  // Disable sleep
    EKey.IsPressed = true;
    EVENT_KeyPressed();
}

// ================================= Events ====================================
FORCE_INLINE void EVENT_ADC_Done(void) {
    if (!ELED.IsOn) return;
    // Adjust current
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (EADC.Value > ILedNominal && ELED.PWMDesired > 0)       ELED.PWMDesired--;
        if (EADC.Value < ILedNominal && ELED.PWMDesired < PWM_MAX) ELED.PWMDesired++;
    }
}
FORCE_INLINE void EVENT_KeyPressed(void) {
    if (ELED.IsOn) {
        ELED.IsOn = false;
        ELED.PWMDesired = 0;
    }
    else {
        ELED.IsOn = true;
        // Left existing PWM if key pressed during fade-out
        if (ELED.PWM > PWM_START_VALUE) ELED.PWMDesired = ELED.PWM;
        else ELED.PWMDesired = PWM_START_VALUE;
    }
}

// =================================== PWM =====================================
FORCE_INLINE void PWMEnable(void){
    // Setup timer1
    TCNT1 = 0x00;
    OCR1C = 0xFF;
    OCR1A = 0x00;
    OCR1B = 0x00;
    // Disable Timer1 interrupts
    TIMSK &= ~((1<<OCIE1A)|(1<<OCIE1B)|(1<<TOIE1));
    // Init clock
    PLLCSR |= (1<<LSM);  // Set low-speed mode (32MHz), need for low-voltage operation
    PLLCSR |= (1<<PLLE); // Set PLL enable
    loop_until_bit_is_set (PLLCSR, PLOCK); // wait until PLOCK become 1
    PLLCSR |= (1<<PCKE); // Enable PCK
    // Single Channel VCC-based PWM
    TCCR1 = (1<<PWM1A)|(1<<COM1A1)|(1<<COM1A0)|(0<<CS13)|(0<<CS12)|(0<<CS11)|(1<<CS10);
}
FORCE_INLINE void PWMDisable(void) {
    TCCR1  = 0x00; // Stop Timer1
    PLLCSR = 0x00; // Stop clock
}
bool MayChangePWM(void) {
    if (ELED.PWM <= PWM_STEPOVER1) {
        return DelayElapsed(&ELED.Timer, PWMDelayLong);        // Low speed
    }
    else if (ELED.PWM > PWM_STEPOVER1 && ELED.PWM <= PWM_STEPOVER2) {
        return DelayElapsed(&ELED.Timer, PWMDelayMid);         // Mid-speed
    }
    else if (ELED.PWM > PWM_STEPOVER2) {
        return DelayElapsed(&ELED.Timer, PWMDelayFast);        // High-speed
    }
    return false;
}

FORCE_INLINE void ADC_Start(void) {
    ADMUX = CALMA_ADMUX;
    // Unipolar mode, not reversed polarity
    ADCSRB = (0<<BIN)|(0<<IPR);
    // Start first conversion
    // ADC enabled, IRQ disabled, 125 kHz
    ADCSRA = (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(1<<ADIF)|(0<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    // leave ADC enabled
}

// ******************** Interrupts **************************
// Key interrupt
EMPTY_INTERRUPT (PCINT0_vect);


