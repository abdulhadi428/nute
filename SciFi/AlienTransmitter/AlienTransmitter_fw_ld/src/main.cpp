/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "led.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {ActiveOn, ActiveOff, Passive} LampState_t;

uint32_t maxOnTime=50;
uint32_t maxOffTime=400;
uint32_t maxUptime=30000;

uint32_t onTime;
uint32_t offTime;

uint32_t BlinkTimer;
uint32_t SensorTimer;

LampState_t lampState=Passive;

void resetLampCycle(void);
void sensorInterrupt(void);

int main(void) {

    sensorInterrupt();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    srand( 4 );

    Delay.Init();

    while (1) {
        switch (lampState){
            case ActiveOn:
                if (Delay.Elapsed(&BlinkTimer, onTime)) {
                    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
                    lampState=ActiveOff;
                }
            break;
            case ActiveOff:
                if (Delay.Elapsed(&BlinkTimer, offTime)) {
                    resetLampCycle();
                }
            break;
            default: break;
        }

        if (Delay.Elapsed(&SensorTimer, maxUptime)) {
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);
            lampState=Passive;
        }
    }
}

void resetLampCycle(void){
    onTime=(rand()%maxOnTime);
    offTime=(rand()%maxOffTime);
    Delay.Reset(&BlinkTimer);
    lampState=ActiveOn;
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

void sensorInterrupt(void){
    if (lampState==Passive) {
        resetLampCycle();
    }
    Delay.Reset(&SensorTimer);
}
