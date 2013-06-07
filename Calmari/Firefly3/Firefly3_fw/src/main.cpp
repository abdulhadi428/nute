/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led.h"
#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "adc_f100.h"

Adc_t Adc;
IWDG_t Iwdg;

#define ADC_VALUE_TO_OFF    540
#define ADC_VALUE_TO_ON     198

#define LED_COLOR           clBlue

static inline void Init();
static inline void GoSleep();

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv8, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    while(TRUE) {
        chThdSleepMilliseconds(450);
        uint32_t rslt = 0;
        Adc.Enable();
        Adc.Calibrate();
        for(uint8_t i=0; i<8; i++) {
            Adc.StartConversion();
            while(!Adc.ConversionCompleted()) chThdSleepMilliseconds(20);
            rslt += Adc.Result();
        }
        Adc.Disable();
        rslt >>= 3;
        Uart.Printf("Adc: %u\r", rslt);
        if(rslt > ADC_VALUE_TO_OFF) Led.SetColorSmoothly(clBlack);
        if(rslt < ADC_VALUE_TO_ON)  Led.SetColorSmoothly(LED_COLOR);
        // Check if sleep
        if(Led.IsOff()) GoSleep();
    }
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
    Led.Init();
    // Set white and print info only when switch on, not after watcdog reset.
    if(!Iwdg.ResetOccured()) {
        Uart.Printf("\rFirefly3  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
        Led.SetColor(clWhite);
    }
    Adc.Init();
}

void GoSleep() {
    // Start LSI
    Clk.LsiEnable();
    // Start IWDG
    Iwdg.SetTimeout(4500);
    Iwdg.Enable();
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
}

