/* 
 * File:   calma3c.h
 * Author: Laurelindo
 *
 * Created on 20 Декабрь 2009 г., 0:16
 */

#ifndef _CALMA3C_H
#define	_CALMA3C_H

#include <avr/io.h>
#include <inttypes.h>

// =============================== General =====================================

// ================================= Sensors ===================================
// Timings

// Ports & pins
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_P       PD3

#define LED_ON()    LED_PORT |=  (1<<LED_P)
#define LED_OFF()   LED_PORT &= ~(1<<LED_P)

// =============================== Prototypes ==================================
// General
void GeneralInit(void);

// Tasks
void CC_Task (void);

#endif	/* _CALMA3C_H */

