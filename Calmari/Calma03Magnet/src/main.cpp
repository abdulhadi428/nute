/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "kl_util.h"
#include "kl_lib.h"
#include "led.h"

int main(void) {
    // ==== Init ====
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div8);
    SystemCoreClockUpdate();
    Delay.Init();

    // Led
    LedSmooth_t Led;
    Led.Init(GPIOB, 6, TIM4, 250, 0, 1, false);
    Led.On();
    Delay.ms(144);
    Led.Off();

    // Sensor
    klPin_t Sns(GPIOB, 15, GPIO_Mode_IPD);

    // ==== Main cycle ====
    uint32_t SnsTmr, SleepTmr;
    bool SnsIsOn = false;
    while (1) {
        Led.Task();

        // Sensor task
        if(Delay.Elapsed(&SnsTmr, 360)) {
            if ((Sns == 1) and (!SnsIsOn)) {
                SnsIsOn = true;
                if ((Led.State == lsOff) or (Led.State == lsDown)) Led.RampUp();
                else Led.RampDown();
            }
            else if ((Sns == 0) and SnsIsOn) SnsIsOn = false;
        }

        // Sleep task
        if (Led.State == lsOff) {
            if (Delay.Elapsed(&SleepTmr, 1800)) {
                // Prepare to sleep: configure GPIO line to be event source
                // Connect EXTI Line to front switch GPIO
                GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
                // Configure front switch EXTI line
                EXTI_InitTypeDef EXTI_InitStructure;
                EXTI_InitStructure.EXTI_Line = EXTI_Line15;
                EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
                EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
                EXTI_InitStructure.EXTI_LineCmd = ENABLE;
                EXTI_Init(&EXTI_InitStructure);
                // Enter sleep mode
                PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
                // At this stage the system has resumed from STOP mode
                EXTI_InitStructure.EXTI_LineCmd = DISABLE;
                EXTI_Init(&EXTI_InitStructure);
            }
        }
    }
}

