/*
 * main.h
 *
 *  Created on: 18.07.2011
 *      Author: Kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <inttypes.h>
#include <avr/wdt.h>

#include "delay_util.h"
#include "color_table.h"

// ================================== Light ====================================
// Timings
#define LED_STEP_T          11  // ms
#define LED_BLINK_ON_T      999
#define LED_BLINK_OFF_T     45

// Ports & pins
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define RED_P       PD6
#define GREEN_P     PD5
#define BLUE_P      PD3

#define LED_RED_DISABLE()   TCCR0A &= ~((1<<COM0A1)|(1<<COM0A0));
#define LED_RED_ENABLE()    TCCR0A |=  ((1<<COM0A1)|(0<<COM0A0));
#define LED_GREEN_DISABLE() TCCR0A &= ~((1<<COM0B1)|(1<<COM0B0));
#define LED_GREEN_ENABLE()  TCCR0A |=  ((1<<COM0B1)|(0<<COM0B0));
#define LED_BLUE_DISABLE()  TCCR2A &= ~((1<<COM2B1)|(1<<COM2B0));
#define LED_BLUE_ENABLE()   TCCR2A |=  ((1<<COM2B1)|(0<<COM2B0));

struct Channel_t {
    uint8_t Desired, Current, Saved;
    volatile uint8_t *TCCRxA, *OCRx;
    uint8_t TccrOnValue, TccrOffValue;
    void Adjust(void);
    void Off(void)  { *OCRx = 0; }
    void On(void) { *OCRx = Current; }
};

typedef enum {BlinkDisabled, BlinkOff, BlinkOn} BlinkState_t;

class Light_t {
private:
    uint16_t Timer;
    Channel_t R, G, B;
    void AllOn (void)    { R.On();     G.On();     B.On(); }
    void AllOff(void)    { R.Off();    G.Off();    B.Off(); }
    void AllAdjust(void) { R.Adjust(); G.Adjust(); B.Adjust(); }
    void SetDesiredColor(uint8_t ARed, uint8_t AGreen, uint8_t ABlue) { R.Desired = ARed; G.Desired = AGreen; B.Desired = ABlue; }
public:
    uint8_t Indx;
    uint16_t BlinkTimer;
    BlinkState_t BlinkState;
    void Init(void);
    void Task(void);
    void SetTableColor(void);
};

// ============================== Prototypes ===================================
void GeneralInit(void);

extern Light_t ELight;

#endif /* MAIN_H_ */
