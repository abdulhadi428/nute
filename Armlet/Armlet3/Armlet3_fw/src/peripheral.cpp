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

// ========================== Peripheral functions =============================
void Beep(const BeepChunk_t *PSequence) {
}

void Vibro(const VibroChunk_t *PSequence) {
}

