/*
 * keys.h
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "hal.h"
#include "kl_lib_f100.h"

// Keys ports and pins
struct BtnGpioPin_t {
    GPIO_TypeDef *Gpio;
    uint16_t Pin;
};

#define BTN_CNT                 3
#define BTN_POLL_PERIOD_MS      72

// Keys indexes
#define KEY_UP      0
#define KEY_DOWN    1
#define KEY_TX      2

const BtnGpioPin_t BtnGpioPin[] = {
        {GPIOB, 6},  // Up
        {GPIOB, 7},  // Down
        {GPIOA, 1},  // Tx
};

// Button Event
enum BtnEvent_t {evtNone, evtPressed, evtReleased, evtTick};

struct Btn_t {
    bool Pressed;
    BtnEvent_t Evt;
    void ClearEvent() { Evt = evtNone; }
};

class Buttons_t {
private:

public:
    void Init();
    void RegisterEvt(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&EvtSrc, PEvtLstnr, EvtMask); }
    void Shutdown();
    inline bool EvtBoth(BtnEvent_t AEvt, Btn_t *PBtn1, Btn_t *PBtn2) { return ((PBtn1->Evt == AEvt) and (PBtn2->Evt == AEvt)); }
    inline bool EvtAny (BtnEvent_t AEvt, Btn_t *PBtn1, Btn_t *PBtn2) { return ((PBtn1->Evt == AEvt) or  (PBtn2->Evt == AEvt)); }
    // Inner use
    EventSource EvtSrc;
};

extern Buttons_t Buttons;
extern Btn_t BtnUp, BtnDown, BtnTx;


#endif /* KEYS_H_ */
