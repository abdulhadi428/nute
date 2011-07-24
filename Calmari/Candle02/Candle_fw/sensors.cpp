#include "sensors.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "delay_util.h"
#include "uart_soft.h"

// ============================= Variables =====================================
Keys_t EKeys;



// =========================== Implementation ==================================
void Keys_t::Init(void) {

    DDRC &= ~(1<<PC3);
    DDRC |=  (1<<PC2);
    PORTC &= ~(1<<PC3);     // No pull-up

    // Init variables
    Key[0].EventPress = &EVENT_KeyDown;
    Key[0].MayRepeat  = true;
    Key[1].EventPress = &EVENT_KeyUp;
    Key[1].MayRepeat  = true;
    Key[2].EventPress = &EVENT_KeyOnOff;
    Key[3].EventPress = &EVENT_KeyLit;
}

void Keys_t::Task(void) {
//    if(!Delay.Elapsed(&Timer, KEY_MEASUREMENT_PERIOD)) return;
    if(!Delay.Elapsed(&Timer, 200)) return;
    // Measure
    uint16_t i;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        DDRC  &= ~((1<<SNS)|(1<<SNSK));
        PORTC &= ~((1<<SNS)|(1<<SNSK));
        for(i=0; i<8192; i++) {

            PORTC |=  (1<<SNSK);
            nop();
//            nop();
//            nop();
//            nop();
            PORTC &= ~(1<<SNSK);
            nop();

            DDRC  |=  (1<<SNS);
            nop();
            DDRC  &= ~(1<<SNS);
            //nop();
            if (bit_is_set(PINC, SNSK)) break;
        }
    }
    UARTSendUint(i);
    UARTNewLine();


//    // Check if key event triggered
//    for (uint8_t i=0; i<4; i++) {
//        if (IsTouched(i) && !Key[i].IsDown) {
//            Key[i].IsDown = true;
//            if (Key[i].MayRepeat) ATOMIC_BLOCK(ATOMIC_FORCEON) {
//                Key[i].Delay = KEY_REPEAT_TIMEOUT;
//                Delay.Reset(&Key[i].Timer);
//            }
//            if (Key[i].EventPress != 0) Key[i].EventPress();
//        }
//        else if (!IsTouched(i) && Key[i].IsDown) Key[i].IsDown = false;
//
//        // Check if continuous keypress
//        if (Key[i].MayRepeat) {
//            if (Key[i].IsDown) if (Delay.Elapsed(&Key[i].Timer, Key[i].Delay)) {
//                ATOMIC_BLOCK(ATOMIC_FORCEON) {
//                    Key[i].Delay = KEY_REPEAT_DELAY;
//                }
//                if (Key[i].EventPress != 0) Key[i].EventPress();
//            } // if is down && delay elapsed
//        } // if may repeat
//    } // for
}


