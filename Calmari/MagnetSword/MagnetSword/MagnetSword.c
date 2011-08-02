/*
 * MagnetSword.c
 *
 * Created: 11.07.2011 23:14:07
 *  Author: Kreyl
 */ 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "delay_util.h"

#define MAGNET_IS_NEAR()	bit_is_set(PINB, PB2)
// Timings
#define QUICK_DELAY		7
#define SLOW_DELAY		27
#define STEP_OVER		40
#define OFF_DELAY		4005

void MagnetTask(void);
void LightTask(void);
void TimerDisable(void);
void TimerEnable(void);

struct {
	uint8_t PWM_Needed;
	uint8_t PWM;
	uint16_t Timer;
	uint16_t Delay;
	bool ItsDelayBeforeFade;
} Led;


struct {
	uint16_t Timer;
	bool IsNear;
} Magnet;

int main(void)
{
	// ==== Hardware init ====
    wdt_enable(WDTO_2S);    // Enable WatchDog at 2 s
    ACSR |= (1<<ACD);       // Shutdown comparator
	// Magnet
	DDRB  &= ~(1<<PB2);	
	PORTB |=  (1<<PB2);	
	// Output
	DDRB  |= (1<<PB1);	
	PORTB |= (1<<PB1);	
	// Others
	TimerEnable();
    DelayInit();
	Magnet.IsNear = false;
	// Led
	Led.PWM_Needed = 0;
	Led.PWM = 0;
	Led.Delay = QUICK_DELAY;
	Led.ItsDelayBeforeFade = false;
    sei();
	
	// Main cycle
    while(1)
    {
		wdt_reset();
        MagnetTask();
		LightTask();
    }
}

void TimerEnable(void) {
    OCR1C = 0xFF;
    OCR1A = 0;
    // Init clock
    PLLCSR |= (1<<LSM);		// Set low-speed mode (32MHz), need for low-voltage operation
    PLLCSR |= (1<<PLLE);	// Set PLL enable
    loop_until_bit_is_set (PLLCSR, PLOCK); // wait until PLOCK become 1
    PLLCSR |= (1<<PCKE);	// Enable PCK
    // Enable PWM1A Set COM1A1/A0: 11 for P-Channel transistor
    TCCR1 = (1<<PWM1A)|(1<<COM1A1)|(1<<COM1A0)|(0<<CS13)|(0<<CS12)|(0<<CS11)|(1<<CS10);
}
void TimerDisable(void) {
	TCCR1 = 0;
}		


// ====================== Events ====================
void EVENT_MagnetNear(void) {
	Led.PWM_Needed = 250;
	Led.ItsDelayBeforeFade = false;
}
void EVENT_MagnetFar(void) {
	Led.ItsDelayBeforeFade = true;
	DelayReset(&Led.Timer);
}
void EVENT_SleepNeeded(void) {
	TimerDisable(); // Disable Timer1
	
	// Enable PinChange IRQ
	MCUCR = (1<<SE)|(1<<SM1)|(0<<SM0);  // Enable sleep in power-down mode
	GIMSK = (1<<PCIE);                  // 
	PCMSK = (1<<PCINT2);                // Enable Key IRQ
	wdt_disable();                      // Disable Watchdog
	sleep_cpu();                        // Fall asleep
	// Awake occured
	wdt_enable(WDTO_2S);                // Enable WatchDog at 2 s
	GIMSK = (0<<PCIE);                  // Disable Key IRQ
	MCUCR = (0<<SE)|(0<<SM1)|(0<<SM0);  // Disable sleep
	
	TimerEnable();		// Enable Timer1
	Magnet.IsNear = true;
	EVENT_MagnetNear();	// Rise MagnetNear event	
}

// ====================== Tasks =====================
void MagnetTask(void) {
	if (!DelayElapsed(&Magnet.Timer, 306)) return;
	// Check if state changed
	if (MAGNET_IS_NEAR() && !Magnet.IsNear) {
		Magnet.IsNear = true;
		EVENT_MagnetNear();
	}
	else if (!MAGNET_IS_NEAR() && Magnet.IsNear) {
		Magnet.IsNear = false;
		EVENT_MagnetFar();
	}
}

void LightTask(void) {
	if (Led.ItsDelayBeforeFade) {
		if (DelayElapsed(&Led.Timer, OFF_DELAY)) {
			Led.PWM_Needed = 0;
			Led.ItsDelayBeforeFade = false;	// Waiting time is over
		}
		return;	// Do not change PWM during waiting
	}
	
    if (!DelayElapsed(&Led.Timer, Led.Delay)) return;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
		// Change PWM
        if      (Led.PWM < Led.PWM_Needed) Led.PWM++;
        else if (Led.PWM > Led.PWM_Needed) Led.PWM--;
        // Setup delay
        Led.Delay = (Led.PWM < STEP_OVER)? SLOW_DELAY : QUICK_DELAY;
        // Change pwm value
        OCR1A = Led.PWM;
    } // Atomic
	// check if sleep
	if (Led.PWM == 0) EVENT_SleepNeeded();
}

// ====================== Interrupt =====================
// Key interrupt
EMPTY_INTERRUPT (PCINT0_vect);
