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
#include "cmd_uart_f10x.h"
#include "cmd.h"
#include "Sequences.h"
#include "led_rgb.h"

#define KEYPRESS_DELAY  18

LedRGB_t Led({GPIOB, 0, TIM3, 3}, {GPIOB, 3, TIM2, 2}, {GPIOB, 1, TIM3, 4});

void GoSleep();

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    JtagDisable();
    Uart.Init(115200);
    Uart.Printf("\rBadge  AHB=%u; APB1=%u; APB2=%u\r\n", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    PinSetupIn(GPIOA, 0, pudNone);  // Button
    Led.Init();
    // Remap TIM2 CH2 to PB3
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    uint32_t tmp = AFIO->MAPR;
    tmp &= ~AFIO_MAPR_TIM2_REMAP;
    tmp |= AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1;  // 01: Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3)
    AFIO->MAPR = tmp;
    RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;

    // Start blink
    Led.StartSequence(lsqChange);

    // Main cycle: btn polling
    uint32_t N=0;
    while(true) {
        chThdSleepMilliseconds(72);
        if(PinIsSet(GPIOA, 0)) {
            N++;
            if(N >= KEYPRESS_DELAY) GoSleep();
        }
        else N=0;
    }
}

void GoSleep() {
    chSysLock();
    // Enable WKUP1 pin
    PWR->CSR |= PWR_CSR_EWUP;
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
    chSysUnlock();
}

