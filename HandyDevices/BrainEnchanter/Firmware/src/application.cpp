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
private:
//    Timer_t ITmr;
public:
    Timer_t Tmr;
    uint8_t M_Set, M_Now, S_Now;
    void Init() {
        M_Set = 18;
        M_Now = 0;
        S_Now = 0;
        // Setup TIM11 to be fed by LSE
        Tmr.Init(TIM11);
        Tmr.SetTopValue(32768);
        Tmr.EnableExternalClk();
        Tmr.ClearIrqBits();
        nvicEnableVector(TIM11_IRQn, CORTEX_PRIORITY_MASK(IRQ_PRIO_LOW));
        Tmr.EnableIrqOnUpdate();
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
    void StartTimer() { Tmr.Enable();  }
    void StopTimer()  { Tmr.Disable(); }
} Time;

extern "C" {
CH_IRQ_HANDLER(TIM11_IRQHandler) {
    CH_IRQ_PROLOGUE();
    Time.Tmr.ClearIrqBits();
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMSK_NEWSECOND);
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}
} // extern C
#endif

#if 1 // ============================ Timers ===================================
static VirtualTimer IMeasureTmr;
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
    void ShowCurrentOff() {
        Lcd.DrawImage(88, 2, iconOff);
        Lcd.PrintfFont(Times_New_Roman18x16, 11, 0, "         ");
    }
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
    void ShowFailure() { Lcd.PrintfFont(Times_New_Roman18x16, 11, 0, "FAILURE  "); }

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
    if(Time.M_Set < MINUTES_MAX) {
        Beeper.Beep(BeepKeyOk);
        Time.M_Set++;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.ShowTimeSet();
}
static void KeyTimeDown() {
    if(Time.M_Set > MINUTES_MIN) {
        Beeper.Beep(BeepKeyOk);
        Time.M_Set--;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.ShowTimeSet();
}

static void KeyCurrentUp() {
    if((Current.uA + CURRENT_STEP_uA) <= CURRENT_MAX_uA) {
        Beeper.Beep(BeepKeyOk);
        Current.uA += CURRENT_STEP_uA;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.DisplayCurrentSet();
}
static void KeyCurrentDown() {
    if((Current.uA - CURRENT_STEP_uA) >= CURRENT_MIN_uA) {
        Beeper.Beep(BeepKeyOk);
        Current.uA -= CURRENT_STEP_uA;
    }
    else Beeper.Beep(BeepKeyErr);
    Interface.DisplayCurrentSet();
}

static void KeyStart() {
    if(App.State == asIdle) {   // Start current
        Beeper.Beep(BeepStart);
        // Time
        Time.M_Now = Time.M_Set;
        Time.S_Now = 0;
        Time.StartTimer();
        Interface.ShowTimeNow();
        // Current
        Current.HighVEnable();
        chThdSleepMilliseconds(810);
        Current.On();
        Interface.ShowCurrentOn();
        App.State = asCurrent;
    }
    else if(App.State == asCurrent) App.StopEverything();
}

static void KeyStartLong() {
    switch(App.State) {
        case asStandBy:
            App.Wake();
            break;
        case asIdle:
            App.Sleep();
            break;
        case asCurrent:
            App.StopEverything();
            App.Sleep();
            break;
    } // switch
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
        if(EvtMsk & EVTMSK_KEY_START_LONG)   KeyStartLong();
        if(App.State == asIdle) {
            if(EvtMsk & EVTMSK_KEY_TIME_UP)      KeyTimeUp();
            if(EvtMsk & EVTMSK_KEY_TIME_DOWN)    KeyTimeDown();
            if(EvtMsk & EVTMSK_KEY_CURRENT_UP)   KeyCurrentUp();
            if(EvtMsk & EVTMSK_KEY_CURRENT_DOWN) KeyCurrentDown();
        }

        // Time
        if(EvtMsk & EVTMSK_NEWSECOND) {
            Time.DecreaseSecond();
            if(Time.IsZero()) App.StopEverything();
            else Interface.ShowTimeNow();
        }

        // Measurement
        if(EvtMsk & EVTMSK_MEASURE_TIME) {
            if(App.State != asStandBy) Measure.StartMeasurement();
        }

        if(EvtMsk & EVTMSK_MEASUREMENT_DONE) {
            if(App.State != asStandBy) Interface.DisplayBattery();
            if(App.State == asCurrent) {
                Interface.DisplayCurrentMeasured();
                // Check if overcurrent (current regulation loop failure)
                int32_t tmp = Measure.GetResult(CURRENT_CHNL);
                tmp = Current.Adc2uA(tmp);
                if((tmp - Current.uA) > 504) {
                    App.StopEverything();
                    Interface.ShowFailure();
                }
            }
        }
    } // while 1
}

void App_t::Init() {
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    Current.Init();
    Time.Init();
    Measure.InitHardware();
    Measure.PThreadToSignal = PThd;
    Interface.Reset();
    // Timers init
    chVTSet(&IMeasureTmr, MS2ST(MEASURE_PERIOD_MS), MeasureTmrCallback, nullptr);
}

void App_t::StopEverything() {
    Beeper.Beep(BeepStop);
    // Time
    Interface.ShowTimeStopped();
    Time.StopTimer();
    // Current
    Current.HighVDisable();
    Current.Off();
    Interface.ShowCurrentOff();
    App.State = asIdle;
}

void App_t::Sleep() {
    App.State = asStandBy;
    chVTReset(&IMeasureTmr);
    Beeper.Beep(BeepStandBy);
    Lcd.Backlight(0);
    Lcd.Cls();
    Lcd.PrintfFont(Times_New_Roman18x16, 27, 2, "Sleep");
    chThdSleepMilliseconds(630);
    chSysLock();
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    Clk.UpdateSysTick();
    chSysUnlock();
}

void App_t::Wake() {
    chSysLock();
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    Clk.UpdateSysTick();
    chSysUnlock();
    chThdSleepMilliseconds(99);
    Beeper.Beep(BeepWake);
    Lcd.Cls();
    Interface.Reset();
    Lcd.Backlight(50);
    chVTSet(&IMeasureTmr, MS2ST(MEASURE_PERIOD_MS), MeasureTmrCallback, nullptr);
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
