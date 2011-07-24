#include "sensors.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "delay_util.h"
#include "uart_soft.h"

// ============================= Variables etc. ================================
Keys_t EKeys;

uint16_t Measure0(void);

// =========================== Implementation ==================================
void Keys_t::Init(void) {
    // Init variables
    Key[0].EventPress = &EVENT_KeyDown;
    Key[0].MayRepeat  = true;
    Key[0].Measure    = &Measure0;
    Key[1].EventPress = &EVENT_KeyUp;
    Key[1].MayRepeat  = true;
    Key[1].Measure    = &Measure0;
    Key[2].EventPress = &EVENT_KeyOnOff;
    Key[2].Measure    = &Measure0;
    Key[3].EventPress = &EVENT_KeyLit;
    Key[3].Measure    = &Measure0;
    // Calibrate
    for (uint8_t i=0; i<4; i++) Key[i].Calibrate();
}

void Keys_t::Task(void) {
//    if(!Delay.Elapsed(&Timer, KEY_MEASUREMENT_PERIOD)) return;
    if(!Delay.Elapsed(&Timer, 200)) return;

    for (uint8_t i=0; i<4; i++) {
        // Measure
        Key[i].CapValue = Key[i].Measure();
        UARTSendUint(Key[i].CapValue);
        if (Key[i].IsTouched()) UARTSendString(" T");
        UARTNewLine();


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
    } // for
    UARTNewLine();
}

void Key_t::Calibrate() {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        CapDefault = 0;
        for (uint8_t i=0; i<4; i++) CapDefault += Measure();
        CapDefault >>= 2;  // == /4
    }
}

// Measures
#define MEASURE_F(ASNS, ASNSK, ADDR, APORT, APIN)   \
    uint16_t i; \
    ATOMIC_BLOCK(ATOMIC_FORCEON) { \
        APORT &= ~((1<<ASNS)|(1<<ASNSK)); \
        ADDR  |=  ((1<<ASNS)|(1<<ASNSK)); \
        ADDR  &= ~((1<<ASNS)|(1<<ASNSK)); \
        for(i=0; i<4096; i++) { \
            /* Charge Cx pulsing SNSK */ \
            APORT |=  (1<<ASNSK); \
            ADDR  |=  (1<<ASNSK); \
            ADDR  &= ~(1<<ASNSK); \
            APORT &= ~(1<<ASNSK); \
            /* Connect Cx to Cs */ \
            ADDR  |=  (1<<ASNS); \
            ADDR  &= ~(1<<ASNS); \
            /* Check if completed */ \
            if (bit_is_set(APIN, ASNSK)) break; \
        } \
    } \
    return i


uint16_t Measure0(void) {
    MEASURE_F(KEY0_SNS, KEY0_SNSK, KEY0_DDR, KEY0_PORT, KEY0_PIN);
}
