/*
 * main.cpp
 *
 *  Created on: 18.07.2011
 *      Author: Kreyl
 */

#include "main.h"
#include "avr/power.h"
#include "avr/pgmspace.h"
#include <avr/sleep.h>

#include "uart_soft.h"
#include "sensors.h"
#include "battery.h"

Light_t ELight;

// ========================= Implementation ====================================
int main(void) {
    GeneralInit();
    UARTSendString("Candle\r");

    while(1) {
        wdt_reset();
        EKeys.Task();
        ELight.Task();
        Battery.Task();
        IndicateCharging_Task();
    }
    return 0;
}

void GeneralInit(void) {
    wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator
    clock_prescale_set(clock_div_8);    // 1 MHz
    UARTInit();
    Battery.Init();
    Delay.Init();
    ELight.Init();
    // Must be last init
    EKeys.Init();
    sei();
}

void Shutdown (void) {
    ELight.IsOn = false;
    ELight.SetDesiredColor(0, 0, 0);
    EKeys.DisableAllButOnOff();
}

// ============================== Tasks ========================================
void IndicateCharging_Task(void) {
    if (!Battery.IsCharging) return;
    // Fade and brighten
    if (Delay.Elapsed(&ELight.RampTimer, CHARGE_BLINK_T)) {
        if (ELight.Ramp == RampUp) {
            ELight.Ramp = RampDown;
            ELight.SetDesiredColor(0, 0, 1);
        }
        else {
            ELight.Ramp = RampUp;
            ELight.SetDesiredColor(0, 0, 18);
        }
    }
}

void Light_t::Task() {
    // Adjust colors
    if (Delay.Elapsed(&Timer, LED_STEP_T)) AllAdjust();
    // Check if sleep
    if (!IsOn && (!R.IsOn() && !G.IsOn() && !B.IsOn())) {
        // Enter sleep
        set_sleep_mode(SLEEP_MODE_IDLE);    // Left timer enabled
        sleep_enable();
        sleep_cpu();
    }
}


// ============================= Events ========================================
void EVENT_KeyDown(void) {
    UARTSendString("Down\r");
    if (ELight.Indx == 0) return;
    ELight.Indx--;
    ELight.SetTableColor();
}
void EVENT_KeyUp(void) {
    UARTSendString("Up\r");
    if (ELight.Indx == (COLOR_COUNT-1)) return;
    ELight.Indx++;
    ELight.SetTableColor();
}
void EVENT_KeyOnOff(void) {
    UARTSendString("OnOff\r");
    if (ELight.IsOn) {
        Shutdown();
    }
    else {
        ELight.IsOn = true;
        ELight.SetTableColor();
        EKeys.EnableAll();
    }
}

// ==== Battery ====
void EVENT_ChargeStarted(void) {
    EKeys.DisableAll();
    ELight.Ramp = RampUp;
}
void EVENT_ChargeEnded(void) {
    EKeys.EnableAll();
    ELight.Ramp = RampDown;
    Shutdown();
}

void EVENT_ADCMeasureCompleted(void) {
//    UARTSendString("ADC: ");
//    UARTSendUint(Battery.ADCValue);
//    UARTNewLine();
    if (Battery.IsDischarged() && !Battery.IsCharging) Shutdown();
}


// ============================= Light =========================================
void Light_t::Init() {
    LED_DDR  |=   (1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P);
    LED_PORT &= ~((1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P));
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCCR0B = (0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    TCCR2A = (1<<WGM21)|(1<<WGM20);
    TCCR2B = (0<<WGM22)|(0<<CS22)|(1<<CS21)|(0<<CS20);
    Delay.Reset(&Timer);
    R.TCCRxA = &TCCR0A;
    G.TCCRxA = &TCCR0A;
    B.TCCRxA = &TCCR2A;
    R.OCRx   = &OCR0A;
    G.OCRx   = &OCR0B;
    B.OCRx   = &OCR2B;
    R.TccrOnValue  = ((1<<COM0A1)|(0<<COM0A0));
    G.TccrOnValue  = ((1<<COM0B1)|(0<<COM0B0));
    B.TccrOnValue  = ((1<<COM2B1)|(0<<COM2B0));
    R.TccrOffValue = (uint8_t)(~((1<<COM0A1)|(1<<COM0A0)));
    G.TccrOffValue = (uint8_t)(~((1<<COM0B1)|(1<<COM0B0)));
    B.TccrOffValue = (uint8_t)(~((1<<COM2B1)|(1<<COM2B0)));

    Indx = 0;
    SetTableColor();
    IsOn = true;
}

void Light_t::SetTableColor(void) {
    R.Desired = GetColor(Indx, 0);
    G.Desired = GetColor(Indx, 1);
    B.Desired = GetColor(Indx, 2);
}

void Channel_t::Adjust() {
    if (Desired < Current) {
        Current--;
        if (Current == 0) ATOMIC_BLOCK(ATOMIC_FORCEON) { *TCCRxA &= TccrOffValue; }
    }
    else if (Desired > Current) {
        if (Current == 0) ATOMIC_BLOCK(ATOMIC_FORCEON) { *TCCRxA |= TccrOnValue; }
        Current++;
    }
    ATOMIC_BLOCK(ATOMIC_FORCEON) { *OCRx = Current; }
}
