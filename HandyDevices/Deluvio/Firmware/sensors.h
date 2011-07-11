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
#include "menu.h"

// ================================= Defines ===================================
// Ports & pins
#define QTOUCH_STUDIO_MASKS 1
#define NUMBER_OF_PORTS     1
#define SNS1                B
#define SNSK1               B

// Measurement period in msec.
#define QT_MEASUREMENT_PERIOD   25

#define KEY_REPEAT_TIMEOUT      702 // Time before repeat
#define KEY_REPEAT_DELAY        252 // Time between repeat

// ========================= Variables & types =================================
struct Keys_t {
    bool KeyDownPressed:1;
    bool KeyUpPressed:1;
    bool KeyMenuPressed:1;
    bool KeyAquaPressed:1;
    uint16_t Timer;
    uint16_t KeypressTimer;
    uint16_t KeypressDelay;
};

extern struct Keys_t EKeys;
extern uint16_t current_time_ms_touch;

// ================================ Prototypes =================================
void QTouchInit(void);
bool QTouchActivityDetected(void);

bool SensorIsTouched(uint8_t ASensor);
void SensorsMeasure(void);

void Task_Sensors(void);

void EVENT_KeyDown(void);
void EVENT_KeyUp(void);
void EVENT_KeyMenu(void);
void EVENT_KeyAquaPressed(void);
void EVENT_KeyAquaDepressed(void);

void EVENT_AnyKey(void);    // triggered first if any key pressed
//void EVENT_KeyAquaDepressed(void);


#endif	/* SENSORS_H */

