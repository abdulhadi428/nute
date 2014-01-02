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
#include "power.h"
#include "battery_consts.h"
#include "lcd_images.h"

App_t App;

#if 1 // ============================ Time =====================================
class Time_t {
public:
    uint8_t M_Set, M_Now, S_Now;
    void Init() {
        M_Set = 18;
        M_Now = 0;
        S_Now = 0;
    }
    bool IsZero() { return (M_Now == 0) and (S_Now == 0); }
    void DecreaseSecond() {
        if(IsZero()) return;
        if(S_Now == 0) {
            S_Now = 59;
            M_Now--;
        }
        else S_Now--;
    }
} Time;

#endif

#if 1 // ============================ Timers ===================================
static VirtualTimer ITimeTmr, IMeasureTmr;
void TmrOneSecondCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMSK_NEWSECOND);
    chVTSetI(&ITimeTmr, MS2ST(1000), TmrOneSecondCallback, nullptr);
    chSysUnlockFromIsr();
}
void MeasureTmrCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMSK_MEASURE_TIME);
    chVTSetI(&IMeasureTmr, MS2ST(MEASURE_PERIOD_MS), MeasureTmrCallback, nullptr);
    chSysUnlockFromIsr();
}
#endif

#if 1 // ============================ Interface ================================
class Interface_t {
public:
    // ==== Time ====
    void ShowTimeSet() { Lcd.Printf(0, 7, "%02u:00", Time.M_Set); }
    void ShowTimeStopped() { Lcd.PrintfFont(Times_New_Roman18x16, 27, 2, "--:--  "); }
    void ShowTimeNow() { Lcd.PrintfFont(Times_New_Roman18x16, 27, 2, "%02u:%02u ", Time.M_Now, Time.S_Now); }

    // ==== Current ====
    void ShowCurrentOn()  { Lcd.DrawImage(88, 2, iconOn); }
    void ShowCurrentOff() { Lcd.DrawImage(88, 2, iconOff); }
    void DisplayCurrentSet() { Lcd.Printf(9, 7, "%u.%u mA", Current.uA2mA_Whole(Current.uA), Current.uA2mA_Fract(Current.uA)/10); }
    void DisplayCurrentMeasured() {
        uint32_t tmp = Measure.GetResult(CURRENT_CHNL);
//        Uart.Printf("adc=%u; ", tmp);
        tmp = Current.Adc2uA(tmp);
//        Uart.Printf("curr=%u\r", tmp);
        uint32_t Whole = Current.uA2mA_Whole(tmp);
        uint32_t Fract = Current.uA2mA_Fract(tmp);
        Lcd.PrintfFont(Times_New_Roman18x16, 11, 0, "%u.%02u mA ", Whole, Fract);
    }

    // ==== Common ====
    void DisplayBattery() {
        uint32_t tmp = Measure.GetResult(BATTERY_CHNL);
        tmp = (tmp * 3320) / 4096;  // Calculate voltage, mV, from ADC value
        //Uart.Printf("%u ", tmp);
        tmp = mV2PercentAlkaline(tmp);
        //Uart.Printf("%u\r", tmp);
        if     (tmp > 80) Lcd.DrawImage(90, 0, iconBattery80_100);
        else if(tmp > 60) Lcd.DrawImage(90, 0, iconBattery60_80);
        else if(tmp > 40) Lcd.DrawImage(90, 0, iconBattery40_60);
        else if(tmp > 20) Lcd.DrawImage(90, 0, iconBattery20_40);
        else              Lcd.DrawImage(90, 0, iconBattery0_20);
    }

    void Reset() {
        Lcd.PrintfFont(Times_New_Roman18x16, 11, 0, "0.0 mA ");
        ShowTimeStopped();
        Lcd.Symbols(0, 4,
                LineHorizDouble, 7,
                LineHorizDoubleDown, 1,
                LineHorizDouble, 8,
                0);
        Lcd.Printf (0, 5, " Time  %c Current", LineVertDouble);
        Lcd.Symbols(7, 6, LineVertDouble, 1, 0);
        Lcd.Symbols(7, 7, LineVertDouble, 1, 0);
        DisplayCurrentSet();
        ShowTimeSet();
    }
};

static Interface_t Interface;
#endif

#if 1 // ============================ Keys =====================================
static void KeyTimeUp() {
    if(App.State != asIdle) return;
    if(Time.M_Set < MINUTES_MAX) {
        Beeper.Beep(BeepKeyOk);
        Time.M_Set++;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.ShowTimeSet();
}
static void KeyTimeDown() {
    if(App.State != asIdle) return;
    if(Time.M_Set > MINUTES_MIN) {
        Beeper.Beep(BeepKeyOk);
        Time.M_Set--;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.ShowTimeSet();
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

static void KeyStart() {
    //Beeper.Beep(BeepKeyOk);
    if(App.State == asIdle) {   // Start current
        Beeper.Beep(BeepStart);
        // Time
        Time.M_Now = Time.M_Set;
        Time.S_Now = 0;
        Interface.ShowTimeNow();
        chVTSet(&ITimeTmr, MS2ST(1000), TmrOneSecondCallback, nullptr);
        // Current
        Current.HighVEnable();
        chThdSleepMilliseconds(360);
        Current.On();
        Interface.ShowCurrentOn();
        App.State = asCurrent;
    }
    else if(App.State == asCurrent) App.StopEverything();
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
        if(EvtMsk & EVTMSK_NEWSECOND) {
            Time.DecreaseSecond();
            if(Time.IsZero()) App.StopEverything();
            else Interface.ShowTimeNow();
        }

        // Measurement
        if(EvtMsk & EVTMSK_MEASURE_TIME) { Measure.StartMeasurement(); }
        if(EvtMsk & EVTMSK_MEASUREMENT_DONE) {
            Interface.DisplayBattery();
            Interface.DisplayCurrentMeasured();
        }
    } // while 1
}

void App_t::Init() {
    Current.Init();
    Time.Init();
    Measure.InitHardware();
    Interface.Reset();
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    Measure.PThreadToSignal = PThd;
    // Timers init
    chVTSet(&IMeasureTmr, MS2ST(MEASURE_PERIOD_MS), MeasureTmrCallback, nullptr);
}

void App_t::StopEverything() {
    Beeper.Beep(BeepStop);
    // Time
    Interface.ShowTimeStopped();
    chVTReset(&ITimeTmr);
    // Current
    Current.HighVDisable();
    Current.Off();
    Interface.ShowCurrentOff();
    App.State = asIdle;
}
#endif

#if UART_RX_ENABLED // ================ Command processing =====================
#define UART_RPL_BUF_SZ     36

void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        default: break;
    } // switch
}
#endif
