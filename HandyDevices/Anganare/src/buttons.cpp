/*
 * buttons.cpp
 *
 *  Created on: 24 окт. 2013 г.
 *      Author: r.leonov
 */


#include "buttons.h"
#include "load.h"
#include "lcd.h"
#include "encoder.h"

ButtonControl_t Button;
Load_t  Load;
void TimeOut(void *p);
// =============================== Thread ======================================

static WORKING_AREA(waBtnThread, 256);
__attribute__ ((__noreturn__))
static void BtnThread(void *arg) {
    chRegSetThreadName("ButtonTask");
    while(1) Button.Task();
}

void ButtonControl_t::MainBtnPressed() {
    if(Load.TimeToWork == 0) return;
    if(Load.InLoad) return;
    else {
        chVTSetI(&ITmr, MS2ST(Load.TimeToWork), TimeOut, NULL);
        Load.On();
        Lcd.DrawImage(35,0, icon_Lighting);
//    Uart.Printf("%u\r", Load.TimeToWork);
    }
}
void TimeOut(void *p) {
    Load.Off();
    Lcd.Printf(6,0, " ");
    Uart.Printf("wasworked=%ums\r", Load.TimeToWork);
}

void ButtonControl_t::ModeBtnPressed() {
    Load.ChangeMode();
    Uart.Printf("Mode:%u\r", Load.Mode);
    Load.PrintMode(Load.Mode);
}
void ButtonControl_t::TimeBtnPressed() {

}

void ButtonControl_t::Init() {
    MainBtn.PinNumber = BTN_MAIN;
    ModeBtn.PinNumber = BTN_MODE;
    TimeBtn.PinNumber = BTN_TIME;
    MainBtn.KWP = ModeBtn.KWP = TimeBtn.KWP = false;
    // Init GPIOS
    PinSetupIn(BTN_PORT, BTN_MAIN, pudPullUp);
    PinSetupIn(BTN_PORT, BTN_MODE, pudPullUp);
    PinSetupIn(BTN_PORT, BTN_TIME, pudPullUp);

    // Create thread
    chThdCreateStatic(waBtnThread, sizeof(waBtnThread), NORMALPRIO, (tfunc_t)BtnThread, NULL);
}
void ButtonControl_t::Task() {
    switch (Load.Mode) {
        case lmTime:
            if (MainBtn.KeyIsPressed() && !MainBtn.KWP) {
                MainBtnPressed();
//                Uart.Printf("db\r");
                MainBtn.KWP = true;
            }
            else if (!MainBtn.KeyIsPressed() && MainBtn.KWP) {
                MainBtn.KWP = false;
            }
            break;

        case lmManual:
            if (MainBtn.KeyIsPressed() && !MainBtn.KWP) {
                PressedTime = chTimeNow();
//                Uart.Printf("rr: %u\r", chTimeNow());
                Lcd.DrawImage(35,0, icon_Lighting);
                Load.On();
                MainBtn.KWP = true;
            }
            else if (!MainBtn.KeyIsPressed() && MainBtn.KWP) {
                PressedTime = chTimeNow() - PressedTime;
                Uart.Printf("waspressed=%ums\r", PressedTime);
                Load.TimeToWork = PressedTime;
                Lcd.Printf(0,0, "%u.%us   ", (Load.TimeToWork/1000), (Load.TimeToWork%1000)/100);
                Load.Off();
                MainBtn.KWP = false;
            }

            break;
    }
    if (ModeBtn.KeyIsPressed() && !ModeBtn.KWP) {
        if(!MainBtn.KWP) ModeBtnPressed();
//        Uart.Printf("mb\r");
        ModeBtn.KWP = true;
    }
    else if (!ModeBtn.KeyIsPressed() && ModeBtn.KWP) {
        ModeBtn.KWP = false;
    }
    if (TimeBtn.KeyIsPressed() && !TimeBtn.KWP) {
        if(!MainBtn.KWP) TimeBtnPressed();
//        Uart.Printf("tb\r");
        TimeBtn.KWP = true;
    }
    else if (!TimeBtn.KeyIsPressed() && TimeBtn.KWP) {
        TimeBtn.KWP = false;
    }

    chThdSleepMilliseconds(99);
}
