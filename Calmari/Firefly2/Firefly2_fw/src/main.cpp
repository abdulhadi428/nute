/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led.h"
#include "kl_lib_f0.h"
#include "ch.h"
#include "hal.h"

#include "adc_f0.h"

Adc_t Adc;

static inline void Init();

int main(void) {
    // ==== Init clock system ====
    Clk.UpdateFreqValues();
    Clk.SetupFlashLatency();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    while(TRUE) {
        chThdSleepMilliseconds(360);
        Adc.StartConversion();
        while(!(ADC1->ISR & ADC_ISR_EOC));
        uint16_t rslt = ADC1->DR;
        Uart.Printf("Adc: %u\r", rslt);

    }
}

void Init() {
    Uart.Init(115200);
    Uart.Printf("\rFirefly2\r");
    Led.Init();

    Adc.Init();
    Adc.StartConversion();
}

// =============================== Phototransistor =============================
