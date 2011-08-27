#include "sensors.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "delay_util.h"
#include "uart_soft.h"


// ============================= Variables etc. ================================
Keys_t EKeys;

// =========================== Implementation ==================================
void Keys_t::Init(void) {
    DDRC &= ~((1<<PC1)|(1<<PC3));
    PORTC |= (1<<PC1)|(1<<PC3);
    UpWasDown = false;
    DownWasDown = false;
}

void Keys_t::Task(void) {
    void (*FEvent)(void);
    FEvent = 0;

    // Keypress
    if(Delay.Elapsed(&Timer, KEY_MEASUREMENT_PERIOD)) {
        if (!UpWasDown && UpIsDown()) {
            UpWasDown = true;
            Delay.Reset(&RepeatTimer);
            if (DownWasDown) FEvent = &EVENT_KeyBoth;
            else {
                RepeatDelay = KEY_REPEAT_TIMEOUT;
                FEvent = &EVENT_KeyUp;
            }
        }
        else if (UpWasDown && !UpIsDown()) UpWasDown = false;

        if (!DownWasDown && DownIsDown()) {
            DownWasDown = true;
            Delay.Reset(&RepeatTimer);
            if (UpWasDown) FEvent = &EVENT_KeyBoth;
            else {
                RepeatDelay = KEY_REPEAT_TIMEOUT;
                FEvent = &EVENT_KeyDown;
            }
        }
        else if (DownWasDown && !DownIsDown()) DownWasDown = false;
    }

    // Repeat
    if (Delay.Elapsed(&RepeatTimer, RepeatDelay) && (!(UpWasDown && DownWasDown))) {
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            RepeatDelay = KEY_REPEAT_TIMEOUT;
        }
        if(UpWasDown)   FEvent = &EVENT_KeyUp;
        if(DownWasDown) FEvent = &EVENT_KeyDown;
    }

    if (FEvent != 0) FEvent();
}


