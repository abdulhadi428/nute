/*
 * interface.h of Armlet1 project
 *
 *  Created on: 15.09.2012
 *      Author: kreyl
 */

#ifndef MENU_H_
#define MENU_H_

#include "lcd1200.h"
#include "kl_lib.h"

#define BLINK_DELAY     306  // ms
#define STATE_DELAY     4005 // ms

enum State_t {
    stIdle, stMain, stSetTime
};

class Interface_t {
private:
    State_t State;
    // Blink needed
    uint32_t IBlinkTmr, IStateTmr;
    void EnterIdle(void);
    // Inc / Dec
    void IInc(bool AFast, int32_t *PValue);
    void IDec(bool AFast, int32_t *PValue);
public:
    void Init(void);
    void Task(void);
    void KeyDown(bool IsLongPressed);
    void KeyUp(bool IsLongPressed);
    void KeyEnter(void);
    // Events
    void SettingChanged(void);
    void SettingsLoad(void);
    void SettingsSave(void);
};
extern Interface_t Interface;

// Keypress events
inline void Evt_KeyDown(bool IsLongPressed)  { Interface.KeyDown(IsLongPressed); }
inline void Evt_KeyUp(bool IsLongPressed)    { Interface.KeyUp(IsLongPressed); }
inline void Evt_KeyEnter(bool IsLongPressed) { Interface.KeyEnter(); }

#endif /* MENU_H_ */
