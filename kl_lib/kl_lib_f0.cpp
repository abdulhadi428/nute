/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f0.h"

// ============================== Delay ========================================
Delay_t Delay;
uint32_t ITickCounter;
bool Delay_t::Elapsed(uint32_t *AVar, const uint32_t ADelay) {
    if ((uint32_t)(ITickCounter - (*AVar)) >= ADelay) {
        *AVar = ITickCounter; // Reset delay
        return true;
    }
    else return false;
}

void Delay_t::ms(uint32_t Ams) {
    uint32_t __ticks = (SystemCoreClock / 8000) * Ams;
    Loop(__ticks);
}

// IRQ
void SysTick_Handler(void) {
    ITickCounter++;
}


