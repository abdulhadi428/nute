/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "mdm.h"
#include "sensors.h"
#include "uart.h"

void GeneralInit(void);
void LedOn(void);
void LedOff(void);
void LedToggle(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Vault Keeper\r");

    GeneralInit();

    uint32_t FTimr;
    // ==== Main cycle ====
    while(1) {
        if(Delay.Elapsed(&FTimr, 400)) {
            LedToggle();
        }
        WaterSensor.Task();
    } // while(1)
    return 0;
}

void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    Delay.Init();
    WaterSensor.Init();
    // Led
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LedOn(void) {
    GPIOC->BSRR = GPIO_Pin_9;
}
void LedOff(void) {
    GPIOC->BRR = GPIO_Pin_9;
}
void LedToggle(void) {
    GPIOC->ODR ^= GPIO_Pin_9;
}

// ================================== Events ===================================

