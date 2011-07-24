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
        if(Delay.Elapsed(&ELight.Timer, 1000)) {
            //LED_PORT ^= (1<<GREEN_P);
        }
        EKeys.Task();
    }
    return 0;
}

void GeneralInit(void) {
    wdt_enable(WDTO_2S);
    ACSR = 1<<ACD;  // Disable analog comparator
    clock_prescale_set(clock_div_2);    // 4 MHz

    Delay.Init();
    UARTInit();

    // Light
    LED_DDR  |=   (1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P);
    LED_PORT &= ~((1<<RED_P)|(1<<GREEN_P)|(1<<BLUE_P));
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCCR0B = (0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    TCCR2A = (1<<WGM21)|(1<<WGM20);
    TCCR2B = (0<<WGM22)|(0<<CS22)|(1<<CS21)|(0<<CS20);
    Delay.Reset(&ELight.Timer);

    EKeys.Init();

    sei();
}

// ============================= Events ========================================
void EVENT_KeyDown(void) {
    LED_PORT ^= (1<<BLUE_P);
}
void EVENT_KeyUp(void) {
    LED_PORT ^= (1<<RED_P);
}
void EVENT_KeyOnOff(void) {

}
void EVENT_KeyLit(void) {
    LED_PORT ^= (1<<GREEN_P);
}
