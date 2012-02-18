/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "delay_util.h"
#include "kl_util.h"
#include "main.h"
#include "lcd110x.h"
#include "IDStore.h";

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {ActiveOn, ActiveOff, Passive} LampState_t;



uint32_t maxOnTime=50;
uint32_t maxOffTime=400;
uint32_t maxUptime=30000;
uint32_t sensorActivityCheckout=100;

uint32_t onTime;
uint32_t offTime;

uint32_t BlinkTimer;
uint32_t SensorTimer;
uint32_t SensorAvtivityTimer;

LampState_t lampState=Passive;

bool waitingForSensorConfirmation = false;

bool HUIDConnected = false;
bool keyMPressed = false;
bool keyIncPressed = false;
bool keyDecPressed = false;

////////////////////////////
///HUID vars
char menuItemNumber=0;
uint32_t maxOnTimeStep=50;
uint32_t maxOffTimeStep=50;
uint32_t maxUptimeStep=1000;
////////////////////////////


void generalInitialization(void);
void resetLampCycle(void);
void sensorInterrupt(void);
void activateLamp(void);

void saveData(void){
    IDStore.EraseAll();
    IDStore.Add(maxOnTime);
    IDStore.Add(maxOffTime);
    IDStore.Add(maxUptime);
    IDStore.Save();
}

void loadData(void){

    IDStore.IDArr.Count=3;
    IDStore.Load();
    maxOnTime=IDStore.IDArr.ID[0];
    maxOffTime=IDStore.IDArr.ID[1];
    maxUptime=IDStore.IDArr.ID[2];

    if ((maxOnTime==0)&&
       (maxOffTime==0)&&
       (maxUptime==0)){
        maxOnTime=50;
        maxOffTime=400;
        maxUptime=30000;
    }

}

void refreshScreen(void){
    Lcd.Cls();
    Lcd.Printf(1,0,"MaxOn:  %i",maxOnTime);
    Lcd.Printf(1,1,"MaxOff: %i",maxOffTime);
    Lcd.Printf(1,2,"Uptime: %i",maxUptime);
    Lcd.Printf(0,menuItemNumber,">");
}



void blink(void){
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
    Delay.ms(200);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
    Delay.ms(200);
}

void generalInitialization(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    srand( 4 );


    Delay.Init();

//    loadData();

    GPIO_SetBits(GPIOB,GPIO_Pin_11);

    Lcd.Init();




}

int main(void) {

    //sensorInterrupt();

    generalInitialization();

    while (1) {

        ///HUID reconnection

        if ((!HUIDConnected)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))){
            Lcd.Init();
            HUIDConnected=true;
            refreshScreen();
        }
        if ((HUIDConnected)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))){
            HUIDConnected=false;
            saveData();
        }

        ///key pressure

        if ((!keyMPressed)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))){
            keyMPressed=true;
            menuItemNumber=(menuItemNumber+1)%3;
            refreshScreen();
        }
        if ((keyMPressed)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))){
            keyMPressed=false;
        }

        if ((!keyIncPressed)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))){
            keyIncPressed=true;
            switch (menuItemNumber){
                case 0: maxOnTime+=maxOnTimeStep; break;
                case 1: maxOffTime+=maxOffTimeStep; break;
                case 2: maxUptime+=maxUptimeStep; break;
            }
            refreshScreen();
        }
        if ((keyIncPressed)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))){
            keyIncPressed=false;
        }

        if ((!keyDecPressed)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15))){
            keyDecPressed=true;
            switch (menuItemNumber){
                case 0:
                    if (maxOnTime>=maxOnTimeStep){maxOnTime-=maxOnTimeStep;}
                break;
                case 1:
                    if (maxOffTime>=maxOffTimeStep){maxOffTime-=maxOffTimeStep;}
                break;
                case 2:
                    if (maxUptime>=maxUptimeStep){maxUptime-=maxUptimeStep;}
                break;
            }
            refreshScreen();
        }
        if ((keyDecPressed)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15))){
            keyDecPressed=false;
        }


        Lcd.Task();

        //lamp blinking
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

        //lamp activiy cycle termination
        if (Delay.Elapsed(&SensorTimer, maxUptime)) {
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);
            lampState=Passive;
        }


        if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)){
            activateLamp();
        }
        if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){
            activateLamp();
        }



        //snesor activity verifiing

    }
}

void resetLampCycle(void){
    if (maxOnTime>0){
        onTime=(rand()%maxOnTime);
    }else{
        onTime=0;
    }
    if (maxOffTime>0){
        offTime=(rand()%maxOffTime);
    }else{
        offTime=0;
    }
    Delay.Reset(&BlinkTimer);
    lampState=ActiveOn;

    if (onTime>0){
        GPIO_SetBits(GPIOA,GPIO_Pin_1);
    }
}

void sensorInterrupt(void){
    if (!waitingForSensorConfirmation){
        waitingForSensorConfirmation=true;
        Delay.Reset(&SensorAvtivityTimer);
    }
}

void activateLamp(void){
    if (lampState==Passive) {
        resetLampCycle();


    }
    Delay.Reset(&SensorTimer);
}

