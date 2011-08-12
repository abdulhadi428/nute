/*
 * File:   menu.h
 * Author: Kreyl Laurelindo
 *
 * Created on 12-11 2010 ?., 20:15
 */

#ifndef MENU_H
#define	MENU_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#define PRINT_TIME_X    6
#define PRINT_TIME_Y    4

// Time to get out of menu, ms
#define MENU_EXIT_TIMEOUT   7000

// ============================ Types ==========================================
// States
enum State_t {StIdle, StBacklight, StMenu};

extern enum State_t EState;

// ============================ Prototypes =====================================
void Task_Menu(void);
void EnterIdle(void);

// Events
void EVENT_KeyDown(void);
void EVENT_KeyUp(void);
void EVENT_KeyMenu(void);
void EVENT_KeyAquaPressed(void);
void EVENT_KeyAquaDepressed(void);
void EVENT_AnyKey(void);


#endif	/* MENU_H */

