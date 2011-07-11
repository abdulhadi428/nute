/* 
 * File:   keys.h
 * Author: Kreyl Laurelindo
 *
 * Created on 6 ќкт€брь 2010 г., 23:28
 */

#ifndef KEYS_H
#define	KEYS_H

#include <avr/io.h>

#define KEYS_DDR    DDRA
#define KEYS_PORT   PORTA
#define KEYS_PIN    PINA
#define KEY_DOWN    PA5
#define KEY_UP      PA4
#define KEY_LEFT    PA3
#define KEY_RIGHT   PA2

#define KEY_DOWN_PRESSED()  bit_is_clear(KEYS_PIN, KEY_DOWN)
#define KEY_UP_PRESSED()    bit_is_clear(KEYS_PIN, KEY_UP)
#define KEY_LEFT_PRESSED()  bit_is_clear(KEYS_PIN, KEY_LEFT)
#define KEY_RIGHT_PRESSED() bit_is_clear(KEYS_PIN, KEY_RIGHT)

// =============================== Prototypes ==================================
void Keys_Task(void);
void KeysInit(void);

#endif	/* KEYS_H */

