/* 
 * File:   calma3c.h
 * Author: Laurelindo
 *
 * Created on 20 Декабрь 2009 г., 0:16
 */

#ifndef _CALMA3C_H
#define	_CALMA3C_H

// =============================== General =====================================

// ============================== CC timings ===================================
#define CC_RX_ON_DELAY      54 // ms
#define CC_RX_OFF_DELAY     504 // ms

// ================================== Light ====================================
// Timings

// LED
#define LED_DDR         DDRD
#define LED_PORT        PORTD
#define LED_P           PD3
#define LED_TOGGLE()    LED_PORT ^=  (1<<LED_P)
#define LED_ON()        LED_PORT |=  (1<<LED_P)
#define LED_OFF()       LED_PORT &= ~(1<<LED_P)


// =============================== Prototypes ==================================
// General
void GeneralInit(void);

// Tasks
void CC_Task (void);

// Events
void EVENT_NewPacket(void);

#endif	/* _CALMA3C_H */

