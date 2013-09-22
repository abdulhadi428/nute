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

#define ADC_VALUE_TO_OFF    144
#define ADC_VALUE_TO_ON     54

#define LED_COLOR           ((Color_t){0, 0, 99})

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
        uint32_t rslt = 0;
        for(uint8_t i=0; i<8; i++) {
            Adc.StartConversion();
            while(!Adc.ConversionCompleted());
            rslt += Adc.Result();
        }
        rslt >>= 3;
        Uart.Printf("Adc: %u\r", rslt);
        if(rslt > ADC_VALUE_TO_OFF) Led.SetColorSmoothly(clBlack);
        if(rslt < ADC_VALUE_TO_ON)  Led.SetColorSmoothly(LED_COLOR);
    }
}

void Init() {
    Uart.Init(115200);
    Uart.Printf("\rFirefly2\r");
    Led.Init();
    Led.SetColorSmoothly((Color_t){255, 255, 255});
    Adc.Init();
}

// =============================== Phototransistor =============================
