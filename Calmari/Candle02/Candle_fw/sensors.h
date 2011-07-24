/*
 * File:   sensors.h
 * Author: Kreyl Laurelindo
 *
 * Created on 30 ?????? 2010 ?., 15:43
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include <stdbool.h>
#include <inttypes.h>

/*
 * 1) SNS and SNSK both low to discharge Cs.
 * 2) SNS tri-state.
 * 3) SNSK high output to charge the cap formed by electrode/dielectric/finger.
 * 4) SNSK tri-state
 * 5) SNS low output - cap [e/d/f] transfers charge to Cs
 * 6) Check voltage on SNSK to see if it's reached threshold yet. If so, go to Step 9.
 * 7) SNS tri-state.
 * 8) Increment a counter and go to step 3.
 */

// ================================= Defines ===================================
#define SNS             PC0
#define SNSK            PC1
#define BOTH_LO()       DDRC  |= (1<<SNS)|(1<<SNSK); \
                        PORTC &= ~((1<<SNS)|(1<<SNSK))
#define SNS_HIZ()       DDRC  &= ~(1<<SNS)
#define SNSK_PULSE()    PORTC |= (1<<SNSK); \
                        DDRC  &= ~(1<<SNSK); \
                        PORTC &= ~(1<<SNSK)
#define SNS_LO()        DDRC  |=  (1<<PC2)

#define nop( )  asm volatile ("nop\n\t" ::)

// Measurement period in msec.
#define KEY_MEASUREMENT_PERIOD  25

#define KEY_REPEAT_TIMEOUT      702 // Time before repeat
#define KEY_REPEAT_DELAY        504 // Time between repeat

// ========================= Variables & types =================================
struct Key_t {
    bool IsDown;
    bool MayRepeat;
    uint16_t Timer, Delay;
    void (*EventPress)(void);
};

class Keys_t {
private:
    Key_t Key[4];
    uint16_t Timer;
    // Pin manipulating

    //bool IsTouched(uint8_t ASensor) { return (qt_measure_data.qt_touch_status.sensor_states[0] & (1<<ASensor)); }
public:
    void Init(void);
    void Task(void);
};

extern Keys_t EKeys;

// ================================ Prototypes =================================
void Task_Sensors(void);

void EVENT_KeyDown(void);
void EVENT_KeyUp(void);
void EVENT_KeyOnOff(void);
void EVENT_KeyLit(void);


#endif	/* SENSORS_H */

