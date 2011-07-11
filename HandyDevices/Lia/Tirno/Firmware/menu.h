/* 
 * File:   menu.h
 * Author: Kreyl Laurelindo
 *
 * Created on 12 ќкт€брь 2010 г., 20:15
 */

#ifndef MENU_H
#define	MENU_H

#include <inttypes.h>

// ============================ Types ==========================================
// States
enum State_t {StateList, StateSearch, StateOptions, StateRename_ChoosePlace, StateRename_ChooseLetter, StatePair};

extern enum State_t EState;

// ============================ Prototypes =====================================
void SetState(enum State_t);

// Keys
void EVENT_KeyUp(void);
void EVENT_KeyDown(void);
void EVENT_KeyLeft(void);
void EVENT_KeyRight(void);

// Inner use
void DisplayList(uint8_t StartElement);

uint8_t GetLetter(void);

#endif	/* MENU_H */

