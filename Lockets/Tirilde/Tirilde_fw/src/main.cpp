/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "kl_lib.h"
#include "led.h"


enum Clk_t {clk1MHzInternal, clk2MHzInternal, clk4MHzInternal, clk8MHzInternal, clk8MHzExternal, clk36MHzExternal, clk72MHzExternal};
void InitClock(Clk_t AClk);

void InitPwm(void);

int main(void) {
    // ==== Init ====
    InitClock(clk2MHzInternal);
    Delay.Init();
    InitPwm();
    Uart.Init(115200);
    Uart.Printf("Tirilde\r");

    // ==== Main cycle ====
    //uint32_t SnsTmr, SleepTmr;
    //bool SnsIsOn = false;
    while (1) {
        //Led.Task();

        // Sensor task
//        if(Delay.Elapsed(&SnsTmr, 360)) {
//            if ((Sns == 1) and (!SnsIsOn)) {
//                SnsIsOn = true;
//                if ((Led.State == lsOff) or (Led.State == lsDown)) Led.RampUp();
//                else Led.RampDown();
//            }
//            else if ((Sns == 0) and SnsIsOn) SnsIsOn = false;
//        }

     }
}

inline void InitClock(Clk_t AClk) {
    RCC_DeInit();                                           // RCC system reset(for debug purpose)
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   // Enable Prefetch Buffer
    RCC_PCLK2Config(RCC_HCLK_Div1);                         // PCLK2 = HCLK   (to APB2, 72MHz max)
    // External clock
    if ((AClk == clk8MHzExternal) or (AClk == clk36MHzExternal) or (AClk == clk72MHzExternal)) {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                            // HCLK = SYSCLK
        RCC_HSEConfig(RCC_HSE_ON);                                  // Enable HSE
        ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();     // Wait till HSE is ready
        if (HSEStartUpStatus == SUCCESS) {
            if (AClk == clk8MHzExternal) {
                FLASH_SetLatency(FLASH_Latency_0);                      // Flash 1 wait state
                RCC_PCLK1Config(RCC_HCLK_Div1);                         // PCLK1 = HCLK (to APB1, 36MHz max)
                RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
                while (RCC_GetSYSCLKSource() != 0x04);
                SystemCoreClock = 8000000;
            }
            else {
                switch (AClk) {
                    case clk36MHzExternal:
                        FLASH_SetLatency(FLASH_Latency_1);                      // Flash 1 wait state
                        RCC_PCLK1Config(RCC_HCLK_Div1);                         // PCLK1 = HCLK (to APB1, 36MHz max)
                        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);    // PLLCLK = 8MHz / 2 * 9 = 36 MHz
                        SystemCoreClock = 36000000;
                        break;

                    case clk72MHzExternal:
                        FLASH_SetLatency(FLASH_Latency_2);                      // Flash 2 wait state
                        RCC_PCLK1Config(RCC_HCLK_Div2);                         // PCLK1 = HCLK/2 (to APB1, 36MHz max)
                        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);    // PLLCLK = 8MHz * 9 = 72 MHz
                        SystemCoreClock = 72000000;
                        break;

                    default: break;
                } // switch
                RCC_PLLCmd(ENABLE);                                     // Enable PLL
                while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);    // Wait till PLL is ready
                RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              // Select PLL as system clock source
                while(RCC_GetSYSCLKSource() != 0x08);                   // Wait till PLL is used as system clock source
            }
        }
        else {    // If HSE fails to start-up, the application will have wrong clock configuration.
            while (1);
        }
    } // if external
    else {  // Internal clock: 8 and lower
        switch (AClk) {
            case clk8MHzInternal:
                RCC_HCLKConfig(RCC_SYSCLK_Div1);        // HCLK = SYSCLK
                SystemCoreClock = 8000000;
                break;
            case clk4MHzInternal:
                RCC_HCLKConfig(RCC_SYSCLK_Div2);        // HCLK = SYSCLK / 2
                SystemCoreClock = 4000000;
                break;
            case clk2MHzInternal:
                RCC_HCLKConfig(RCC_SYSCLK_Div4);        // HCLK = SYSCLK / 4
                SystemCoreClock = 2000000;
                break;
            case clk1MHzInternal:
                RCC_HCLKConfig(RCC_SYSCLK_Div8);        // HCLK = SYSCLK / 8
                SystemCoreClock = 1000000;
                break;

            default: break;
        }
        FLASH_SetLatency(FLASH_Latency_0);      // Flash 0 wait state
        RCC_PCLK1Config(RCC_HCLK_Div1);         // PCLK1 = HCLK (to APB1, 36MHz max)
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
        while (RCC_GetSYSCLKSource() != 0x00);
    }
}

inline void InitPwm(void) {
    // ==== Gpio ====
    klGpioSetupByMsk(GPIOA, GPIO_Pin_7 | GPIO_Pin_8, GPIO_Mode_AF_PP);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);    // Remap TIM1 CH1N to PA7
    // ==== Timer ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    // Timebase
    TIM1->CR1 = 0x01;       // Enable timer, set clk division to 0, AutoReload not buffered
    TIM1->CR2 = 0;          // Output Idle State?
    TIM1->PSC = 0;          // No clock division
    TIM1->ARR = 250;        // Autoreload register
    TIM1->BDTR = 0x8000 | 0 ;    // Enable output
    TIM1->CCMR1 = 0x60;     // PWM
    //TIM1->CCER = 0b0101;    // CC1P & CC1N active 1
    TIM1->CCER = 0b1100;    // CC1N active 0``
    //TIM1->CCER = 0b1011;

    TIM1->CCR1 = 81;

}
