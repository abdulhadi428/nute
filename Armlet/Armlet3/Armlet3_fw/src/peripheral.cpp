/*
 * peripheral.cpp
 *
 *  Created on: 18.01.2013
 *      Author: kreyl
 */

#include "ch.h"
#include "hal.h"
#include "clocking.h"
#include "string.h"     // For memcpy
#include "peripheral.h"

// Variables
PwrStatus_t PwrStatus;
IR_t IR;
Pill_t Pill[PILL_COUNT_MAX];

// ================================== Beep =====================================
#define BEEP_TOP_VALUE   270 // 100% volume means on/off ratio 1/1
Beeper_t Beeper;
// Timer callback
void BeeperTmrCallback(void *p) {
    Beeper.Beep((const BeepChunk_t*)p);
}

void Beeper_t::Init() {
    Pin.Init(GPIOD, 12, 4, 1, BEEP_TOP_VALUE);
}

void Beeper_t::Beep(const BeepChunk_t *PSequence) {
    chVTReset(&Tmr);
    // Process chunk
    int8_t Volume = PSequence->VolumePercent;
    if((Volume < 0) or (PSequence->Time_ms == 0)) {    // Nothing to play
        Pin.Off();
        return;
    }
    if(Volume > 100) Volume = 100;  // Normalize volume
    Pin.SetFreqHz(PSequence->Freq_Hz);
    Pin.On(Volume);
    // Start timer
    chVTSet(&Tmr, MS2ST(PSequence->Time_ms), BeeperTmrCallback, (void*)(PSequence+1));
}

// ================================== Vibro ====================================
#define VIBRO_TOP_VALUE   207
Vibrator_t Vibro;
// Timer callback
void VibroTmrCallback(void *p) {
    Vibro.Vibrate((const VibroChunk_t*)p);
}

void Vibrator_t::Init() {
    Pin.Init(GPIOE, 14, 1, 4, VIBRO_TOP_VALUE);
    Pin.SetFreqHz(171);
}

void Vibrator_t::Vibrate(const VibroChunk_t *PSequence) {
    chVTReset(&Tmr);
    // Process chunk
    int8_t Intencity = PSequence->Intencity;
    if((Intencity < 0) or (PSequence->Time_ms == 0)) {    // Nothing to play
        Pin.Off();
        return;
    }
    if(Intencity > 100) Intencity = 100;  // Normalize volume
    Pin.On(Intencity);
    // Start timer
    chVTSet(&Tmr, MS2ST(PSequence->Time_ms), VibroTmrCallback, (void*)(PSequence+1));
}
