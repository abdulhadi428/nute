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

//static uint8_t SBuf[UART_RPL_BUF_SZ];

#if 1 // ============================ Timers ===================================
static VirtualTimer ITmr;
void TmrOneSecondCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMSK_NEWSECOND);
    chVTSetI(&ITmr, MS2ST(1000), TmrOneSecondCallback, nullptr);
    chSysUnlockFromIsr();
}
#endif

#if 1 // ============================ Interface ================================
class Interface_t {
public:
    void DisplayCurrentSet() { Lcd.Printf(9, 7, "%u.%u mA", Current.Get_mA_Whole(), Current.Get_mA_Fract()); }
    void DisplayCurrentMeasured() {
        uint32_t adc = Measure.GetResult(CURRENT_CHNL);
        Uart.Printf("Curr=%u\r", adc);
   }
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

static void KeyStart() {
    Beeper.Beep(BeepKeyOk);
    Current.On();
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
            Measure.StartMeasurement();
        }

        // Measurement
        if(EvtMsk & EVTMSK_MEASUREMENT_DONE) {
            Interface.DisplayBattery();
//            Interface.DisplayCurrentMeasured();
        }
    } // while 1
}

void App_t::Init() {
    Current.InitHardware();
    Current.ResetValues();
    Measure.InitHardware();
    Interface.Reset();
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    Measure.PThreadToSignal = PThd;
    // Timers init
    chSysLock();
    chVTSetI(&ITmr, MS2ST(1000), TmrOneSecondCallback, nullptr);
    chSysUnlock();
}
#endif

#if 1 // ======================= Command processing ============================
#define UART_RPL_BUF_SZ     36

void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }

void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;

        default: break;
    } // switch
}
#endif
