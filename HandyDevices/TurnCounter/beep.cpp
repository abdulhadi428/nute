/*
 * beep.cpp
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#include "beep.h"

#define BEEP_TOP_VALUE   22
Beeper_t Beeper;
// Timer callback
void BeeperTmrCallback(void *p) {
    chSysLockFromIsr();
    Beeper.BeepI((const BeepChunk_t*)p);
    chSysUnlockFromIsr();
}

void Beeper_t::Init() {
    IPin.Init(GPIOA, 8, TIM1, 1, BEEP_TOP_VALUE);
}

void Beeper_t::BeepI(const BeepChunk_t *PSequence) {
    // Reset timer
    if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
    if(PSequence == nullptr) {
        IPin.Off();
        return;
    }
    // Set sound
    IPin.SetFreqHz(PSequence->Freq_Hz);
    IPin.Set(PSequence->Volume);
    // Proceed sequence, stop it or restart
    const BeepChunk_t *PCh = nullptr;
    switch(PSequence->ChunkKind) {
        case ckNormal: PCh = PSequence + 1; break;
        case ckStop:                        break;
        case ckRepeat: PCh = IPFirstChunk;  break;
    }
    // Start timer
    chVTSetI(&ITmr, MS2ST(PSequence->Time_ms), BeeperTmrCallback, (void*)PCh);
}

