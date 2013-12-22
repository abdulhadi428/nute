/*
 * application.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: kreyl
 */

#include "application.h"
#include "cmd_uart.h"
#include "peripheral.h"
#include "sequences.h"
#include "eestore.h"
#include "lcd1200.h"
#include "evt_mask.h"

App_t App;

#define UART_RPL_BUF_SZ     36
//static uint8_t SBuf[UART_RPL_BUF_SZ];

#if 1 // ============================ Timers ===================================
static VirtualTimer ITmr;
void TmrOneSecondCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMASK_NEWSECOND);
    chVTSetI(&ITmr, MS2ST(1000), TmrOneSecondCallback, nullptr);
    chSysUnlockFromIsr();
}
#endif

#if 1 // ============================ Current ==================================
class Current_t {
private:
public:
    int32_t uA;
    uint8_t M_Set, M_Now, S_Now;
    uint32_t Get_mA_Whole() { return uA / 1000; }
    uint32_t Get_mA_Fract() { return (uA % 1000) / 100; }
    void Reset() {
        uA = CURRENT_INIT_uA;
        M_Set = 18;
        M_Now = 0;
        S_Now = 0;
    }

};

static Current_t Current;
#endif

#if 1 // ============================ Interface ================================
class Interface_t {
public:
    void DisplayCurrentSet() { Lcd.Printf(9, 7, "%u.%u mA", Current.Get_mA_Whole(), Current.Get_mA_Fract()); }
    void DisplayCurrentMeasured() {}
    void DisplayTimeSet() { Lcd.Printf(0, 7, "%02u:00", Current.M_Set); }
    void Reset() {
        Lcd.PrintfFont(Times_New_Roman18x16, 18, 0, "0.0 mA ");
        Lcd.PrintfFont(Times_New_Roman18x16, 27, 2, "--:--");
        Lcd.Symbols(0, 4,
                LineHorizDouble, 7,
                LineHorizDoubleDown, 1,
                LineHorizDouble, 8,
                0);
        Lcd.Printf (0, 5, " Time  %c Current", LineVertDouble);
        Lcd.Symbols(7, 6, LineVertDouble, 1, 0);
        Lcd.Symbols(7, 7, LineVertDouble, 1, 0);
        DisplayCurrentSet();
        DisplayTimeSet();
    }
};

static Interface_t Interface;
#endif

#if 1 // ============================ Keys =====================================
static void KeyStart() {

}

static void KeyTimeUp() {
    if(App.State != asIdle) return;
    if(Current.M_Set < MINUTES_MAX) {
        Beeper.Beep(BeepKeyOk);
        Current.M_Set++;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.DisplayTimeSet();
}
static void KeyTimeDown() {
    if(App.State != asIdle) return;
    if(Current.M_Set > MINUTES_MIN) {
        Beeper.Beep(BeepKeyOk);
        Current.M_Set--;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.DisplayTimeSet();
}

static void KeyCurrentUp() {
    if(App.State != asIdle) return;
    if((Current.uA + CURRENT_STEP_uA) <= CURRENT_MAX_uA) {
        Beeper.Beep(BeepKeyOk);
        Current.uA += CURRENT_STEP_uA;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.DisplayCurrentSet();
}
static void KeyCurrentDown() {
    if(App.State != asIdle) return;
    if((Current.uA - CURRENT_STEP_uA) >= CURRENT_MIN_uA) {
        Beeper.Beep(BeepKeyOk);
        Current.uA -= CURRENT_STEP_uA;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.DisplayCurrentSet();
}

static void KeyStartLong() {
    if(App.State == asStandBy) {
        Beeper.Beep(BeepWake);
        App.State = asIdle;
        Lcd.Backlight(50);
    }
    else {
        Beeper.Beep(BeepStandBy);
        App.State = asStandBy;
        Lcd.Backlight(0);
    }
}
#endif

#if 1 // ========================= Application =================================
static WORKING_AREA(waAppThread, 256);
__attribute__((noreturn))
static void AppThread(void *arg) {
    chRegSetThreadName("App");
    uint32_t EvtMsk;
    while(true) {
        EvtMsk = chEvtWaitAny(ALL_EVENTS);
        // Keys
        if(EvtMsk & EVTMSK_KEY_START)        KeyStart();
        if(EvtMsk & EVTMSK_KEY_TIME_UP)      KeyTimeUp();
        if(EvtMsk & EVTMSK_KEY_TIME_DOWN)    KeyTimeDown();
        if(EvtMsk & EVTMSK_KEY_CURRENT_UP)   KeyCurrentUp();
        if(EvtMsk & EVTMSK_KEY_CURRENT_DOWN) KeyCurrentDown();
        if(EvtMsk & EVTMSK_KEY_START_LONG)   KeyStartLong();

        // Time
        if(EvtMsk & EVTMASK_NEWSECOND) {
        }
    } // while 1
}

void App_t::Init() {
    Current.Reset();
    Interface.Reset();
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    // Timers init
    chSysLock();
    chVTSetI(&ITmr, MS2ST(1000), TmrOneSecondCallback, nullptr);
    chSysUnlock();
}
#endif

#if 1 // ======================= Command processing ============================
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        default: break;
    } // switch
}
#endif
