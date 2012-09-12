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

#define BLINK_DELAY     306  // ms
#define STATE_DELAY     4005 // ms

#define DATA_X  10

enum State_t {
    stIdle, stChanging
};

enum Selection_t {sMinLvl, sMaxLvl};

class Interface_t {
private:
    State_t State;
    Selection_t ISelection;
    // Blink needed
    uint32_t IBlinkTmr, IStateTmr;
    bool IsDisplayed;
    void Select(Selection_t ASelection);
    void EnterIdle(void);
public:
    int32_t MinLvl; // Set between -108 and 0. -108 means "Use all". 0 is impossible.
    int32_t MaxLvl; // At this value of RSSI brightness will be full
    void Init(void);
    void Task(void);
    void DisplayRxLvl(uint8_t ID, int32_t ALvl) {
        Lcd.Printf(DATA_X+1, 5, "   ");
        Lcd.Printf(DATA_X, 6, "    ");
        if(ID != 0) {
            Lcd.Printf(DATA_X+1, 5, "%u", ID);
            Lcd.Printf(DATA_X, 6, "%i", ALvl);
        }
    }
    void DisplayMinLvl() {
        Lcd.Printf(DATA_X, 1, "    ");
        Lcd.Printf(DATA_X, 1, "%i", MinLvl);
        IsDisplayed = true;
        Delay.Reset(&IBlinkTmr);
    }
    void DisplayMaxLvl() {
        Lcd.Printf(DATA_X, 3, "    ");
        Lcd.Printf(DATA_X, 3, "%i", MaxLvl);
        IsDisplayed = true;
        Delay.Reset(&IBlinkTmr);
    }
    void KeyDown(bool IsLongPressed);
    void KeyUp(bool IsLongPressed);
    void KeyEnter(void);
    // Events
    void SettingChanged(void);  // Call after enter here
    void SettingsSave(void);
};
extern Interface_t Interface;

// Keypress events
inline void KeyDown(bool IsLongPressed)  { Interface.KeyDown(IsLongPressed); }
inline void KeyUp(bool IsLongPressed)    { Interface.KeyUp(IsLongPressed); }
inline void KeyEnter(bool IsLongPressed) { Interface.KeyEnter(); }

#endif /* MENU_H_ */
