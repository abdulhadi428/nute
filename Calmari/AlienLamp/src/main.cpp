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
<<<<<<< .mine
#include "led.h"
#include "cc2500.h"
//#include "sensor.h"
#include "acc_mma.h"
#include "i2c_mgr.h"
=======
#include "main.h"
#include "lcd110x.h"
//#include "IDStore.h"
#include "stm32f10x_flash.h"
>>>>>>> .r524

<<<<<<< .mine
=======
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
>>>>>>> .r524

#define SHOWSENSORSTATUS true


<<<<<<< .mine
=======
#define FLASH_PAGE_SIZE     1024
#define SAVED_DATA_SIZE     1024    // must be multiple of FlashPageSize
#define PAGE_COUNT          (SAVED_DATA_SIZE / FLASH_PAGE_SIZE)
>>>>>>> .r524

<<<<<<< .mine
=======
typedef enum {ActiveOn, ActiveOff, Passive} LampState_t;
const uint32_t SaveAddr[3] __attribute__ ((aligned(SAVED_DATA_SIZE))) = {0, 0, 0};

>>>>>>> .r524

<<<<<<< .mine
    uint32_t FTimer;
    while (1) {
        CC.Task();
        i2cMgr.Task();
        Acc.Task();
        //Sensor.Task();
        if (Delay.Elapsed(&FTimer, 504)) {
            klPrintf("X: %i; Y: %i; Z: %i\r", Acc.Accelerations.xMSB, Acc.Accelerations.yMSB, Acc.Accelerations.zMSB);
//            Acc.ReadAccelerations();
            //klPrintf("Evt: %X\r", Acc.EventReg[0]);
            //Acc.ReadEvent();
        }
=======

uint32_t maxOnTime=50;
uint32_t maxOffTime=400;
uint32_t maxUptime=30000;
uint32_t sensorActivityCheckout=100;
uint32_t keyboardLockPeriod=200;

uint32_t onTime;
uint32_t offTime;

uint32_t BlinkTimer;
uint32_t SensorTimer;
uint32_t SensorAvtivityTimer;
uint32_t keyboardLockTimer;

LampState_t lampState=Passive;

bool waitingForSensorConfirmation = false;

bool HUIDConnected = false;
bool keyMPressed = false;
bool keyIncPressed = false;
bool keyDecPressed = false;

bool keyboardLocked = false;

bool s0trigger = false;
bool s1trigger = false;



////////////////////////////
///HUID vars
char menuItemNumber=0;
uint32_t maxOnTimeStep=25;
uint32_t maxOffTimeStep=25;
uint32_t maxUptimeStep=1000;
////////////////////////////


void generalInitialization(void);
void resetLampCycle(void);
void sensorInterrupt(void);
void activateLamp(void);


void saveData(void) {

    klPrintf("save to: %X\r\r",&SaveAddr);
    klPrintf("data to write: %i %i %i\r\r", maxOnTime, maxOffTime, maxUptime);


    uint32_t FAddr = (uint32_t)&SaveAddr[0];// BANK1_WRITE_START_ADDR;
    FLASH_Unlock();

    klPrintf("FAddr: %X\r\r",FAddr);



    // Erase flash
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   // Clear All pending flags

    FLASH_ErasePage(FAddr);

    FLASH_ProgramWord(FAddr, maxOnTime);
    FAddr += 4;
    FLASH_ProgramWord(FAddr, maxOffTime);
    FAddr += 4;
    FLASH_ProgramWord(FAddr, maxUptime);

    FLASH_Lock();

    FAddr =(uint32_t)&SaveAddr[0];

    klPrintf("data in flash: %i\r\r", (*(__IO uint32_t*) FAddr));

}

void loadData(void){

    uint32_t FAddr = (uint32_t)&SaveAddr[0];

    maxOnTime = (*(__IO uint32_t*) FAddr);
    FAddr+=4;
    maxOffTime = (*(__IO uint32_t*) FAddr);
    FAddr+=4;
    maxUptime = (*(__IO uint32_t*) FAddr);

    klPrintf("read from: %X\r\r",FAddr);
    klPrintf("read data: %i %i %i\r\r", maxOnTime, maxOffTime, maxUptime);

    if ((maxOnTime==0)&&
       (maxOffTime==0)&&
       (maxUptime==0)){
        maxOnTime=50;
        maxOffTime=400;
        maxUptime=30000;
>>>>>>> .r524
    }

}


<<<<<<< .mine
    // Sensor
    Acc.EvtTrigger = Event_Trigger;
    Acc.EvtNoTrigger = Event_NoTrigger;
    Acc.Init();
=======
>>>>>>> .r524

<<<<<<< .mine
    EXTI_GenerateSWInterrupt(EXTI_Line5);

//    Sensor.EvtTrigger = Event_Trigger;
//    Sensor.EvtNoTrigger = Event_NoTrigger;

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(ID);
    CC.Shutdown();
=======
void refreshScreen(void){
    Lcd.Cls();
    Lcd.Printf(1,0,"MaxOn:  %i",maxOnTime);
    Lcd.Printf(1,1,"MaxOff: %i",maxOffTime);
    Lcd.Printf(1,2,"Uptime: %i",maxUptime);
    Lcd.Printf(0,menuItemNumber,">");
>>>>>>> .r524

    if (SHOWSENSORSTATUS){
        if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)){
            Lcd.Printf(1,4,"Sensor 0: +");
        }else{
            Lcd.Printf(1,4,"Sensor 0: -");
        }
        if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){
            Lcd.Printf(1,5,"Sensor 1: +");
        }else{
            Lcd.Printf(1,5,"Sensor 1: -");
        }
    }
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

    UART_Init();
    klPrintf(" yeap, I'm here\n");

    Delay.Init();

    loadData();

    GPIO_SetBits(GPIOB,GPIO_Pin_11);

    Lcd.Init();


    s0trigger=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
    s1trigger=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);





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
            klPrintf("HUID conncted\r\r");
        }
        if ((HUIDConnected)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))){
            HUIDConnected=false;
            saveData();
            klPrintf("HUID disconncted\r\r");
        }

        ///key pressure

        if (keyboardLocked){
            if (Delay.Elapsed(&keyboardLockTimer, keyboardLockPeriod)) {
                keyboardLocked=false;
            }
        }else{
            if ((!keyMPressed)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))){
                keyMPressed=true;
                Delay.Reset(&keyboardLockTimer);
                keyboardLocked=true;
                menuItemNumber=(menuItemNumber+1)%3;
                refreshScreen();
            }
            if ((keyMPressed)&&(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))){
                keyMPressed=false;
            }

            if ((!keyIncPressed)&&(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))){
                keyIncPressed=true;
                Delay.Reset(&keyboardLockTimer);
                keyboardLocked=true;
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
                Delay.Reset(&keyboardLockTimer);
                keyboardLocked=true;
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

        if (SHOWSENSORSTATUS){
            if (s0trigger!=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)){
                refreshScreen();
            }
            if (s1trigger!=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){
                refreshScreen();
            }

            s0trigger=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
            s1trigger=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
        }

        if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)){
            activateLamp();
        }
        if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){
            activateLamp();
        }


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

