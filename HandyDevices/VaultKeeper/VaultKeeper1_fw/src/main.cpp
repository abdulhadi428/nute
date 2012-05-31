/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "kl_util.h"
#include "kl_lib.h"
#include "led.h"
#include "sim900.h"

LedBlink_t Led;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();
    uint32_t Tmr;

    while (1) {
        Led.Task();

        if(Delay.Elapsed(&Tmr, 207)) Led.Blink(45);

    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();

    Delay.Init();
    Delay.ms(63);
    UART_Init();

    klJtagDisable();

    Led.Init(GPIOD, 2);
    Led.On();

    Mdm.Init();

    klPrintf("\rVault Keeper1\r");
}
