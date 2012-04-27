/*
 * File:   main.h
 * Author: Kreyl
 *
 * Created on April 11, 2011, 7:38 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <stdbool.h>
#include <avr/io.h>

// =================================== Defines =================================
#define AVADA_PORT  PORTD
#define AVADA_DDR   DDRD
#define AVADA_P     PD6
#define AVADA_OFF() AVADA_PORT |=  (1<<AVADA_P)
#define AVADA_ON()  AVADA_PORT &= ~(1<<AVADA_P)

#define BUZZER_DDR  DDRB
#define BUZZER_PORT PORTB
#define BUZZER_P    PB1

#define KEY_DDR             DDRD
#define KEY_PORT            PORTD
#define KEY_PIN             PIND
#define KEY_P               PD2
#define KEY_POLL_TIMEOUT    99 // ms
#define KEY_IS_DOWN()       bit_is_clear(KEY_PIN, KEY_P)

// Settings
#define SHINE_DURATION  153 // ms
#define BUZZ_DELAY      9

// Sound
#define PERIOD_MAX  900 // Lowest frequency
#define PERIOD_MIN  315 // Highest frequency
// Sound volume
#define VOLUME_MIN  18
#define VOLUME_MAX  36


// ============================ Prototypes =====================================
// Tasks
void Key_Task(void);
void Buzz_Task(void);
void Avada_Task(void);

// Events
void EVENT_KeyPressed(void);

#endif	/* MAIN_H */

