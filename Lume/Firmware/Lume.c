#include "Lume.h"
#include "common.h"
#include "led_io.h"
#include "delay_util.h"
#include <util/delay.h>
#include <util/atomic.h>

// ============================ Types & variables ==============================
struct {
    uint8_t Hour;
    uint8_t HyperMinute;
    uint8_t Second;
} Time;

struct {
    bool MenuPressed, UpPressed, DownPressed;
    uint16_t Timer;
} Keys;

enum {ModeRegular, ModeSetHours, ModeSetMinutes} Mode;

// ============================ Implementation =================================
int main (void) {
    GeneralInit ();
    sei();
    while (1) {  // forever
        if(POWER_OK()) {    // Handle tasks only if power is ok. Time is counted in interrupt.
            TASK_Toggle();
            TASK_Keys();
        } // If Power ok
    }	// while 1
}

FORCE_INLINE void GeneralInit(void) {
    // Light control
    LedIOInit();

    // Keys
    KEYS_DDR  &= ~((1<<KEY_MENU)|(1<<KEY_UP)|(1<<KEY_DOWN));    // Keys are inputs
    KEYS_PORT |=   (1<<KEY_MENU)|(1<<KEY_UP)|(1<<KEY_DOWN);     // Pull-ups on
    Keys.DownPressed = false;
    Keys.MenuPressed = false;
    Keys.UpPressed   = false;
    DelayReset(&Keys.Timer);

    // Timer2: realtime clock counter
    TCCR2B = (0<<CS22)|(0<<CS21)|(1<<CS20);	// DEBUG: no division
    //TCCR2B = (0<<CS22)|(1<<CS21)|(0<<CS20);	// DEBUG: div 8
    //TCCR2B = (0<<CS22)|(1<<CS21)|(1<<CS20);	// DEBUG: div 32
    ASSR  = (1<<AS2);				// Set Async mode of the timer
    TCCR2A = 0;
    //TCCR2B = (1<<CS22)|(0<<CS21)|(1<<CS20);	// RELEASE: div 128, once per second
    TIMSK2 |= (1<<TOIE2);                       // Interrupt is in delay_util

    // Setup initial values
    Time.Second = 1;
    Time.HyperMinute = 0;
    Time.Hour = 1;

    Mode = ModeRegular;

// DEBUG
    DDRC |= 1<<PC4;
    PORTC &= ~(1<<PC4);

    // Start-up time setup
    EVENT_NewHour();
    EVENT_NewHyperMinute();
}

// =============================== Tasks =======================================
// Checks if need toggle something
void TASK_Toggle(void) {
    // Toggle minutes & hours PWM
    TogglePWM(&LControl.Min0PWM);
    TogglePWM(&LControl.Min1PWM);
    TogglePWM(&LControl.Hr0PWM);
    TogglePWM(&LControl.Hr1PWM);
}

void TASK_Keys(void) {
    if (!DelayElapsed(&Keys.Timer, KEYS_POLL_TIME)) return;

    // ******** Up key *******
    if(!Keys.UpPressed && KEY_UP_PRESSED()) {       // Keypress occured
        Keys.UpPressed = true;
        EVENT_KeyUpPressed();
    }
    else if(Keys.UpPressed && !KEY_UP_PRESSED()) {  // Depress occured
        Keys.UpPressed = false;
    }

    // ******** Down key *******
    if(!Keys.DownPressed && KEY_DOWN_PRESSED()) {       // Keypress occured
        Keys.DownPressed = true;
        EVENT_KeyDownPressed();
    }
    else if(Keys.DownPressed && !KEY_DOWN_PRESSED()) {  // Depress occured
        Keys.DownPressed = false;
    }

    // ******** Menu key *******
    if(!Keys.MenuPressed && KEY_MENU_PRESSED()) {       // Keypress occured
        Keys.MenuPressed = true;
        EVENT_KeyMenuPressed();
    }
    else if (Keys.MenuPressed && !KEY_MENU_PRESSED()) { // Depress occured
        Keys.MenuPressed = false;
    }
}

// ============================ Events =========================================
void EVENT_NewHour(void) {
    LControl.HByte &= 0b11000000;   // Clear hours bits
    // Here is switching logic
    uint8_t HourCurrent = Time.Hour;
    uint8_t HourPrev = (HourCurrent == 0)? 11: HourCurrent-1;
    // Setup hours
    switch(Mode) {
        case ModeRegular:
            SetupHour(HourCurrent, PWMRise);
            SetupHour(HourPrev,    PWMFade);
            break;
        case ModeSetHours:
            SetupHour(HourCurrent, PWMBlink);
            SetupHour(HourPrev,    PWMOff);
            break;
        default:
            SetupHour(HourCurrent, PWMTop);
            SetupHour(HourPrev,    PWMOff);
            break;
    } // switch
    // Control bytes will be written in minutes' handler
}
// Switch on needed minute channels, setup their start PWM and pepare to change
void EVENT_NewHyperMinute(void) {
    LControl.HByte &= 0b00111111;   // Clear minutes bits
    LControl.MByte = 0;             // Clear minutes byte
    // Here is switching logic
    if(Mode != ModeSetMinutes) {      // Regular mode
        SetupMinute(Time.HyperMinute, PWMRise);
        if(Time.HyperMinute == 0) SetupMinute(23, PWMFade);
        else SetupMinute(Time.HyperMinute-1, PWMFade);
    }

/*
    switch(Time.HyperMinute) {
        case M0_5:  SetupMinute(M0_5,  PWMRise); SetupMinute(M12,   PWMFade); break;
        case M1:    SetupMinute(M1,    PWMRise); SetupMinute(M0_5,  PWMFade); break;
        case M1_5:  SetupMinute(M1_5,  PWMRise); SetupMinute(M1,    PWMFade); break;
        case M2:    SetupMinute(M2,    PWMRise); SetupMinute(M1_5,  PWMFade); break;
        case M2_5:  SetupMinute(M2_5,  PWMRise); SetupMinute(M2,    PWMFade); break;
        case M3:    SetupMinute(M3,    PWMRise); SetupMinute(M2_5,  PWMFade); break;
        case M3_5:  SetupMinute(M3_5,  PWMRise); SetupMinute(M3,    PWMFade); break;
        case M4:    SetupMinute(M4,    PWMRise); SetupMinute(M3_5,  PWMFade); break;
        case M4_5:  SetupMinute(M4_5,  PWMRise); SetupMinute(M4,    PWMFade); break;
        case M5:    SetupMinute(M5,    PWMRise); SetupMinute(M4_5,  PWMFade); break;
        case M5_5:  SetupMinute(M5_5,  PWMRise); SetupMinute(M5,    PWMFade); break;
        case M6:    SetupMinute(M6,    PWMRise); SetupMinute(M5_5,  PWMFade); break;
        case M6_5:  SetupMinute(M6_5,  PWMRise); SetupMinute(M6,    PWMFade); break;
        case M7:    SetupMinute(M7,    PWMRise); SetupMinute(M6_5,  PWMFade); break;
        case M7_5:  SetupMinute(M7_5,  PWMRise); SetupMinute(M7,    PWMFade); break;
        case M8:    SetupMinute(M8,    PWMRise); SetupMinute(M7_5,  PWMFade); break;
        case M8_5:  SetupMinute(M8_5,  PWMRise); SetupMinute(M8,    PWMFade); break;
        case M9:    SetupMinute(M9,    PWMRise); SetupMinute(M8_5,  PWMFade); break;
        case M9_5:  SetupMinute(M9_5,  PWMRise); SetupMinute(M9,    PWMFade); break;
        case M10:   SetupMinute(M10,   PWMRise); SetupMinute(M9_5,  PWMFade); break;
        case M10_5: SetupMinute(M10_5, PWMRise); SetupMinute(M10,   PWMFade); break;
        case M11:   SetupMinute(M11,   PWMRise); SetupMinute(M10_5, PWMFade); break;
        case M11_5: SetupMinute(M11_5, PWMRise); SetupMinute(M11,   PWMFade); break;
        case M12:   SetupMinute(M12,   PWMRise); SetupMinute(M11_5, PWMFade); break;
    } // switch
*/
    // Write bytes to setup LEDs
    WriteControlBytes();
}

// Keys events
void EVENT_KeyUpPressed(void) {
    // Behave depending on current mode
    switch (Mode) {
        case ModeSetHours:
            if(Time.Hour == 11) Time.Hour = 0;
            else Time.Hour++;
            EVENT_NewHour();
            EVENT_NewHyperMinute();
            break;
        case ModeSetMinutes:
            Time.HyperMinute++;
            if(Time.HyperMinute > 23) Time.HyperMinute = 0;
            EVENT_NewHyperMinute();
            break;
        default:
            break;
    } // switch
}
void EVENT_KeyDownPressed(void) {
    // Behave depending on current mode
    switch (Mode) {
        case ModeSetHours:
            if(Time.Hour == 0) Time.Hour = 11;
            else Time.Hour--;
            EVENT_NewHour();
            EVENT_NewHyperMinute();
            break;
        case ModeSetMinutes:
            if(Time.HyperMinute == 0) Time.HyperMinute = 23;
            else Time.HyperMinute--;
            EVENT_NewHyperMinute();
            break;
        default:
            break;
    } // switch
}
void EVENT_KeyMenuPressed(void) {
    // Behave depending on current mode
    switch (Mode) {
        case ModeRegular:   // Enter SetHours mode
            Mode = ModeSetHours;
            break;
        case ModeSetHours:  // Enter SetMinutes mode
            Mode = ModeSetMinutes;
            break;
        case ModeSetMinutes:    // Return to regular
            Mode = ModeRegular;
            break;
    } // switch
    EVENT_NewHour();
    EVENT_NewHyperMinute();
}


// =========================== Interrupts ======================================
// Time counter
ISR(TIMER2_OVF_vect) { 
    //PORTC ^= 1<<PC4;
    // Get out if in mode of settings
    if (Mode != ModeRegular) return;

    Time.Second++;
    if(Time.Second > 150) {    // 150 seconds in one hyperminute
        Time.Second = 1;
        Time.HyperMinute++;
        if(Time.HyperMinute > 23) { // 24 HyperMinutes in one hour
        //if(Time.HyperMinute > 3) { // DEBUG: 4 HyperMinutes in one hour
//        if(Time.HyperMinute > 11) { // DEBUG: HyperMinutes as hours
            Time.HyperMinute = 0;
            Time.Hour++;
            if(Time.Hour > 11) Time.Hour = 0;
            if (POWER_OK()) EVENT_NewHour();
        } // if Hyperminute > 23
        if (POWER_OK()) EVENT_NewHyperMinute(); // Call after EVENT_NewHour to write correct control bytes
    }// Second
}
