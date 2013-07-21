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
#include "buttons.h"
#include "color_table.h"
#include "cc2500.h"

#define CC_CHANNEL  11

static inline void Init();
static inline void ProcessBtnEvt();
static uint8_t ColorIndx;
static rPkt_t Pkt;
//static EventListener EvtLstnrRadioRx, EvtListenerKeys;
static enum State_t {sIdle, sColorEdit, sFade} State;

#define WAIT_DURATION_MS    45
#define RX_DURATION_MS      999

#define FADE_CMD            0xFE

#define EVTMASK_RADIO_TX    EVENT_MASK(0)
#define EVTMASK_RADIO_RX    EVENT_MASK(1)
#define EVTMASK_KEYS        EVENT_MASK(2)

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
//    uint32_t EvtMsk;
//    Btn.RegisterEvt(&EvtListenerKeys, EVTMASK_KEYS);
    // Application cycle
    while(TRUE) {
        // Transmit if Tx pressed
        if(Btn.Tx.Pressed) {
            Pkt.Indx = (State == sFade)? FADE_CMD : ColorIndx;
            Uart.Printf("Tx %u\r", Pkt.Indx);
            CC.TransmitSync(&Pkt);
            chThdSleepMilliseconds(WAIT_DURATION_MS);
        }
        else {
            // If Up or Down pressed, do nothing
            if(Btn.Up.Pressed or Btn.Down.Pressed) chThdSleepMilliseconds(45);
            // Receive if none of the buttons pressed
            else {
                Uart.Printf("Rx\r");
                uint8_t Result = CC.ReceiveSync(RX_DURATION_MS, &Pkt);
                if(Result == OK) {
                    Uart.Printf("RSSI=%d; %u\r", Pkt.RSSI, Pkt.Indx);
                    if(Pkt.Indx == FADE_CMD) {  // Enter fade
                        State = sFade;
                        Led.SetColorSmoothly(clBlack);
                    }
                    else {
                        State = sIdle;
                        ColorIndx = Pkt.Indx;
                        Led.SetColorSmoothly(ColorTable[ColorIndx]);
                    } // if fade
                } // if rx ok
                //chThdSleepMilliseconds(WAIT_DURATION_MS);
            } // if not pressed
        } // if Tx is pressed
    } // while
}

void Init() {
    JtagDisable();
    State = sIdle;
    Uart.Init(57600);
    ColorIndx = COLOR_COUNT-1;
    Led.Init();
    Led.SetColorSmoothly(ColorTable[ColorIndx]);
    Btn.Init();
    Btn.EvtCallback = ProcessBtnEvt;
    // Radio
    CC.Init();
    CC.SetChannel(CC_CHANNEL);
    CC.SetTxPower(PwrMinus8dBm);

    Uart.Printf("\rCandle AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
}

void ProcessBtnEvt() {
    switch(State) {
        case sIdle:
            if((Btn.Up.Evt == evtPressed) and (Btn.Down.Evt == evtPressed)) {     // Both pressed, switch to sFade
                State = sFade;
                Led.SetColorSmoothly(Btn.Tx.Pressed? COLOR_FADE : clBlack);
            }
            else if(Btn.Up.Pressed or Btn.Down.Pressed) State = sColorEdit;   // Any pressed
            break;

        case sColorEdit:
            if(Btn.Up.Pressed and Btn.Down.Pressed) {     // Both pressed, switch to sFade
                State = sFade;
                Led.SetColorSmoothly(Btn.Tx.Pressed? COLOR_FADE : clBlack);
            }
            else if((Btn.Up.Evt == evtReleased) or (Btn.Down.Evt == evtReleased)) State = sIdle;  // Any released
            // Process long hold of button
            else if(Btn.Up.Evt == evtTick) {
                if(ColorIndx < (COLOR_COUNT-1)) ColorIndx++;
                else ColorIndx = 0;
                Led.SetColorSmoothly(ColorTable[ColorIndx]);
            }
            else if(Btn.Down.Evt == evtTick) {
                if(ColorIndx != 0) ColorIndx--;
                else ColorIndx = COLOR_COUNT-1;
                Led.SetColorSmoothly(ColorTable[ColorIndx]);
            }
            break;

        case sFade:
            if((Btn.Up.Evt == evtPressed) or (Btn.Down.Evt == evtPressed)) {
                State = sColorEdit;
                Led.SetColorSmoothly(ColorTable[ColorIndx]);
            }
            else if(Btn.Tx.Evt == evtPressed)  Led.SetColorSmoothly(COLOR_FADE);
            else if(Btn.Tx.Evt == evtReleased) Led.SetColorSmoothly(clBlack);
            break;
    } // switch state
    Btn.ClearEvents();
}
