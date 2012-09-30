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
#include "kl_time.h"

#define BLINK_DELAY     306  // ms
#define STATE_DELAY     4005 // ms

enum State_t {
    stIdle, stMain, stSetTime
};

class Interface_t {
private:
    DateTime_t IDateTime;
    void DisplayTime();
    void DisplayYear()  { Lcd.Printf(7, 4, "%04u", Time.GetYear()); }
    void DisplayMonth() { Lcd.Printf(7, 5, "%02u", Time.GetMonth()); }
    void DisplayDay()   { Lcd.Printf(7, 6, "%02u", Time.GetDay());  }
    // Needed for blink
    uint32_t IBlinkTmr, IStateTmr;
    void EnterIdle(void);
    // Inc / Dec
    void IInc(bool AFast, int32_t *PValue);
    void IDec(bool AFast, int32_t *PValue);
    // ==== Menus ====
    void EnterSetTime(void);
public:
    State_t State;
    void Init(void);
    void Task(void);
    // Events
    void SettingChanged(void);
    void SettingsLoad(void);
    void SettingsSave(void);
};
extern Interface_t Interface;

#endif /* MENU_H_ */
