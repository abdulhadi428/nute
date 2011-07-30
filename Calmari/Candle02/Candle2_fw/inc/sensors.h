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
#define KEY0_SNS    PC0
#define KEY0_SNSK   PC1
#define KEY0_DDR    DDRC
#define KEY0_PORT   PORTC
#define KEY0_PIN    PINC

#define KEY1_SNS    PC2
#define KEY1_SNSK   PC3
#define KEY1_DDR    DDRC
#define KEY1_PORT   PORTC
#define KEY1_PIN    PINC

#define KEY2_SNS    PB0
#define KEY2_SNSK   PB1
#define KEY2_DDR    DDRB
#define KEY2_PORT   PORTB
#define KEY2_PIN    PINB

#define KEY3_SNS    PB6
#define KEY3_SNSK   PB7
#define KEY3_DDR    DDRB
#define KEY3_PORT   PORTB
#define KEY3_PIN    PINB

#define KEY_THRESHOLD           27
#define KEY_CALIBRATION_TIMEOUT 8

#define nop( )  asm volatile ("nop\n\t" ::)

// Measurement period in msec.
#define KEY_MEASUREMENT_PERIOD  200

#define KEY_REPEAT_TIMEOUT      999 // Time before repeat
#define KEY_REPEAT_DELAY        999 // Time between repeat

// ========================= Variables & types =================================
struct Key_t {
    bool IsDown;
    bool MayRepeat;
    uint16_t Timer, Delay;
    uint16_t CapValue, CapDefault;
    void (*EventPress)(void);
    uint16_t (*Measure)(void);
    void Calibrate(void);
    bool IsTouched(void) { return ((CapValue > (CapDefault + KEY_THRESHOLD)) || (CapValue < (CapDefault - KEY_THRESHOLD))); }
};

enum KeysEnabled_t {keAll, keOnOff, keDisable};

class Keys_t {
private:
    Key_t Key[4];
    uint16_t Timer;
    KeysEnabled_t Enabled;
public:
    void Init(void);
    void Task(void);
    void DisableAll(void) { Enabled = keDisable; }
    void DisableAllButOnOff(void) { Enabled = keOnOff; }
    void EnableAll(void)  { Enabled = keAll;  }
};

extern Keys_t EKeys;

// ================================ Prototypes =================================
void Task_Sensors(void);

void EVENT_KeyDown(void);
void EVENT_KeyUp(void);
void EVENT_KeyOnOff(void);
void EVENT_KeyLit(void);


#endif	/* SENSORS_H */

