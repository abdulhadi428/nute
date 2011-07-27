/*
 * main.cpp
 *
 *  Created on: 18.07.2011
 *      Author: Kreyl
 */

#include "main.h"
#include "avr/power.h"
#include "avr/pgmspace.h"

#include "uart_soft.h"
#include "sensors.h"

Light_t ELight;

// ========================= Implementation ====================================
int main(void) {
    GeneralInit();
    UARTSendString("Candle\r");

    while(1) {
        wdt_reset();
        EKeys.Task();
        ELight.Task();
    }
    return 0;
}

void GeneralInit(void) {
    wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator
    clock_prescale_set(clock_div_8);    // 1 MHz

    Delay.Init();
    UARTInit();
    EKeys.Init();
    ELight.Init();

    sei();
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

    LED_PORT ^= (1<<RED_P);
}
void EVENT_KeyLit(void) {
    UARTSendString("Lit\r");
    LED_PORT ^= (1<<GREEN_P);
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
    //IsOn = false;
}

void Light_t::SetDesiredColor (uint8_t ARed, uint8_t AGreen, uint8_t ABlue) {
    R.Desired = ARed;
    G.Desired = AGreen;
    B.Desired = ABlue;
}
void Light_t::SetTableColor(void) {
    R.Desired = GetColor(Indx, 0);
    G.Desired = GetColor(Indx, 1);
    B.Desired = GetColor(Indx, 2);
    UARTSendUint(Indx);
    UARTSend(':');
    UARTSendAsHex(R.Desired);
    UARTSend(' ');
    UARTSendAsHex(G.Desired);
    UARTSend(' ');
    UARTSendAsHex(B.Desired);
    UARTNewLine();
}

void Light_t::Task() {
    if (!Delay.Elapsed(&Timer, LED_STEP_DELAY)) return;
    R.Adjust();
    G.Adjust();
    B.Adjust();
}

void Channel_t::Adjust() {
    if (Current == Desired) return;
    if (Desired < Current) {
        Current--;
        if (Current == 0) *TCCRxA &= TccrOffValue;
    }
    else {
        if (Current == 0) *TCCRxA |= TccrOnValue;
        Current++;
    }
    *OCRx = Current;
}
