/*
 * menu.h
 *
 *  Created on: 26.08.2012
 *      Author: kreyl
 */

#ifndef MENU_H_
#define MENU_H_

#include "lcd1200.h"
#include "kl_lib.h"
#include "ads124x.h"

#define BLINK_DELAY     306  // ms
#define STATE_DELAY     4005 // ms

#define T_TOSET_COUNT   2

enum State_t {
    stIdle, stChanging
};

class Interface_t {
private:
    State_t State;
    uint8_t SelectionIndx;
    // Blink needed
    uint16_t IBlinkTmr, IStateTmr;
    bool IsDisplayed;
    void Select(uint8_t AIndx);
    void DisplayTToset(uint8_t AIndx) {
        Lcd.Printf(10, 3+AIndx*2, "    ");
        Lcd.Printf(10, 3+AIndx*2, "%i", tToSet[AIndx]);
        IsDisplayed = true;
        Delay.Reset(&IBlinkTmr);
    }
    void EnterIdle(void);
public:
    int32_t tToSet[T_TOSET_COUNT];
    void Init(void);
    void Task(void);
    void DisplayT(uint8_t AIndx, int32_t t) {
        Lcd.Printf(3, 1+AIndx*2, "    ");
        Lcd.Printf(3, 1+AIndx*2, "%i", t);
    }
    void KeyDown(bool IsLongPressed);
    void KeyUp(bool IsLongPressed);
    void KeyEnter(void);
};
extern Interface_t Interface;

// Keypress events
inline void KeyDown(bool IsLongPressed)  { Interface.KeyDown(IsLongPressed); }
inline void KeyUp(bool IsLongPressed)    { Interface.KeyUp(IsLongPressed); }
inline void KeyEnter(bool IsLongPressed) { Interface.KeyEnter(); }

#endif /* MENU_H_ */
