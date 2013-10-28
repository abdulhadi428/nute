/*
 * buttons.h
 *
 *  Created on: 24 окт. 2013 г.
 *      Author: r.leonov
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "kl_lib_f100.h"

#define BTN_PORT     GPIOA
#define BTN_MAIN     11
#define BTN_MODE     12
#define BTN_TIME     15


class buttons_t {
public:
    uint8_t PinNumber;
    bool KWP;
    bool KeyIsPressed() {return (!PinIsSet(BTN_PORT, PinNumber)); }
};

class ButtonControl_t {
private:
    buttons_t MainBtn, ModeBtn, TimeBtn;
public:
    VirtualTimer ITmr;
    uint32_t PressedTime;

    void Init();
    void Task();
    inline void MainBtnPressed();
    inline void ModeBtnPressed();
    inline void TimeBtnPressed();
};

extern ButtonControl_t Button;
#endif /* BUTTONS_H_ */
