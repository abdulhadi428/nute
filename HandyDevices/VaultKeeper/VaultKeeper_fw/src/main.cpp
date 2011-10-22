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
#include "led.h"
#include "uart.h"

void GeneralInit(void);
void LedOn(void);
void LedOff(void);
void LedToggle(void);

void EVENT_WaterHere(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Vault Keeper\r");

    GeneralInit();

    uint32_t FTimr;
    Delay.Reset(&FTimr);
    bool MsgSent = false;
    // ==== Main cycle ====
    while(1) {
        if(Delay.Elapsed(&FTimr, 3006)) {
            //LedToggle();
            if (!MsgSent) {
                MsgSent = true;
                EVENT_WaterHere();
            }
        }
        WaterSensor.Task();
        Led.Task();
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
    Led.Init();
    WaterSensor.EVT_WaterHere = &EVENT_WaterHere;
    //Mdm.Init();

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
void EVENT_WaterHere(void) {
    LedOn();
    UART_PrintString(" Water is here\r");
    Mdm.Init();
    if (Mdm.State == erOk) {
        //Mdm.SendSMS("+79169895800", "(!)Utechka4: podval 45, blok 27");
        //Mdm.SendSMS("111", "11");
        //if (Mdm.SmsSent) UART_PrintString("# SMS sent\r");
        Mdm.ReceiveAllSMS();
        Mdm.PowerDown();
    }
}
