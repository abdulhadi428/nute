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
#include "touch_qt_config.h"
#include "touch_api.h"

// ================================= Defines ===================================
// Measurement period in msec.
#define QT_MEASUREMENT_PERIOD   25

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
    bool IsTouched(uint8_t ASensor) { return (qt_measure_data.qt_touch_status.sensor_states[0] & (1<<ASensor)); }
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

