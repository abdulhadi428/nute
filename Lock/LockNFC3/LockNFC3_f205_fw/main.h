/*
 * main.h
 *
 *  Created on: 30.03.2013
 *      Author: kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "kl_sd.h"
#include "IDStore.h"
#include "evt_mask.h"
#include "led_rgb.h"
#include "Sequences.h"

// ==== Sound files ====
#define SND_COUNT_MAX   100

struct Snd_t {
     char Filename[MAX_NAME_LEN];
     uint32_t ProbBottom, ProbTop;
};

struct SndList_t {
    Snd_t Phrases[SND_COUNT_MAX];
    int32_t Count;
    int32_t ProbSumm;
};

#define STATE_TIMEOUT       18000   // ms; switch to waiting state
#define DOOR_CLOSE_TIMEOUT  9999    // ms
// Colors in RGB
#define DOOR_CLOSED_COLOR   {4, 0, 0}
#define DOOR_OPEN_COLOR     {45, 0, 45}
#define DOOR_CLOSED_BLINK   {45, 0, 0}
#define DOOR_OPEN_BLINK     {135, 0, 135}

enum DoorState_t {dsClosed, dsOpen};
enum AppState_t  {asIdle, asAddingAcc, asRemovingAcc, asAddingMaster, asRemovingMaster};

void TmrGeneralCallback(void *p);

class App_t {
private:
    DoorState_t DoorState = dsClosed;
    VirtualTimer ITmr, IDoorTmr;
    void ProcessCardAppearance();
    void RestartStateTimer() {
        chSysLock();
        if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
        chVTSetI(&ITmr, MS2ST(STATE_TIMEOUT), TmrGeneralCallback, (void*)EVTMSK_STATE_TIMEOUT);
        chSysUnlock();
    }
public:
    Thread *PThd; // Main thread
    AppState_t State = asIdle;
    ID_t CurrentID;
    IDStore_t IDStore;
    void SendEvt(uint32_t EvtMsk) {
        chSysLock();
        chEvtSignalI(PThd, EvtMsk);
        chSysUnlock();
    }
    // Inner use
    void ITask();
    uint8_t ReadConfig();
};

extern App_t App;
extern LedRgbBlinker_t LedService;

#endif /* MAIN_H_ */
