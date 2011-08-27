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
#include <avr/io.h>

// ================================= Defines ===================================
// Measurement period in msec.
#define KEY_MEASUREMENT_PERIOD  200

#define KEY_REPEAT_TIMEOUT      999 // Time before repeat
#define KEY_REPEAT_DELAY        999 // Time between repeat

// ========================= Variables & types =================================
class Keys_t {
private:
    uint16_t Timer, RepeatTimer, RepeatDelay;
    bool UpWasDown, DownWasDown;
    bool UpIsDown(void)   { return bit_is_clear(PINC, PC1); }
    bool DownIsDown(void) { return bit_is_clear(PINC, PC3); }
public:
    void Init(void);
    void Task(void);
};

extern Keys_t EKeys;

// ================================ Prototypes =================================
void Task_Sensors(void);

void EVENT_KeyDown(void);
void EVENT_KeyUp(void);
void EVENT_KeyBoth(void);


#endif	/* SENSORS_H */

