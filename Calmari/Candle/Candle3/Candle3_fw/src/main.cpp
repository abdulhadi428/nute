/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "led.h"
#include "lvl1_candle.h"
#include "buttons.h"
#include "color_table.h"

static inline void Init();
static inline void ProcessBtnEvt();
//static inline void GoSleep();
//static IWDG_t Iwdg;
static uint32_t ColorIndx;
static EventListener EvtLstnrRadioRx, EvtListenerKeys;
static enum State_t {sIdle, sColorEdit, sFade} State;

#define EVTMASK_RADIO_TX    EVENT_MASK(0)
#define EVTMASK_RADIO_RX    EVENT_MASK(1)
#define EVTMASK_KEYS        EVENT_MASK(2)

// ==== Color Change Thread ====
//static WORKING_AREA(waColorKeysThread, 128);
//static msg_t ColorKeysThread(void *arg) {
//    (void)arg;
//    chRegSetThreadName("Color");
//    //bool BothPressed = false;
//    while(1) {
//        chThdSleepMilliseconds(BTNS_POLL_PERIOD_MS);
//        if(KeyUp.Pressed and KeyDown.Pressed) State = sFade;    // Color depend on Tx
//        else if(KeyUp.Pressed) {
//            State = sColorEdit;
//
//        }
//
//    }
//    return 0;
//}

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    // Events
    uint32_t EvtMsk;
    Buttons.RegisterEvt(&EvtListenerKeys, EVTMASK_KEYS);
    // Application cycle
    while(TRUE) {
        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_KEYS);
        // Check keys
        if(EvtMsk & EVTMASK_KEYS) ProcessBtnEvt();
//        if((EvtMsk & EVTMASK_RADIO_TX) and (BtnTx.State == stPressed)) {
//            // Transmit color
//        }
//        else if(

    }
}

void Init() {
    JtagDisable();
    State = sIdle;
    Uart.Init(57600);
    ColorIndx = COLOR_COUNT-1;
    Led.Init();
    Led.SetColor(ColorTable[ColorIndx]);
    Buttons.Init();
    Uart.Printf("\rCandle AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
}

void ProcessBtnEvt() {
    switch(State) {
        case sIdle:
            if((BtnUp.Evt == evtPressed) and (BtnDown.Evt == evtPressed)) {     // Both pressed, switch to sFade
                State = sFade;
                Led.SetColorSmoothly(BtnTx.Pressed? COLOR_FADE : clBlack);
            }
            else if(BtnUp.Pressed() or BtnDown.Pressed()) State = sColorEdit;   // Any pressed
            break;

        case sColorEdit:
            if((BtnUp.Evt == evtPressed) and (BtnDown.Evt == evtPressed)) {     // Both pressed, switch to sFade
                State = sFade;
                Led.SetColorSmoothly(BtnTx.Pressed? COLOR_FADE : clBlack);
            }
            else if((BtnUp.Evt == evtReleased) or (BtnDown.Evt == evtReleased)) State = sIdle;  // Any released
            // Process long hold of button
            else if(BtnUp.Evt == evtTick) {
                if(ColorIndx < (COLOR_COUNT-1)) {    // Top value not reached
                    ColorIndx++;
                    Led.SetColorSmoothly(ColorTable[ColorIndx]);
                }
            }
            else if(BtnDown.Evt == evtTick) {
                if(ColorIndx > 0) {    // Top value not reached
                    ColorIndx--;
                    Led.SetColorSmoothly(ColorTable[ColorIndx]);
                }
            }
            break;

        case sFade:
            if((BtnUp.Evt == evtPressed) or (BtnDown.Evt == evtPressed)) {
                State = sColorEdit;
                Led.SetColorSmoothly(ColorTable[ColorIndx]);
            }
            else if(BtnTx.Evt == evtPressed)  Led.SetColorSmoothly(COLOR_FADE);
            else if(BtnTx.Evt == evtReleased) Led.SetColorSmoothly(clBlack);
            break;
    } // switch state
    // Clear events
    BtnUp.ClearEvent();
    BtnDown.ClearEvent();
    BtnTx.ClearEvent();
}
