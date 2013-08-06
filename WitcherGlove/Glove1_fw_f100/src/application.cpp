/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "acc.h"
#include "ManyLed.h"

#include "signs.h"

#include "VibroSequences.h"
#include "vibro.h"

#include <math.h>

App_t App;

#define TOLERANCE   30
#define T_FILTER    5

//#define SLEEP_ENABLED
#define SLEEP_TIMEOUT           100
#define SLEEP_TIMEOUT_INITIAL   4
#define STABLE_TOLERANCE        20

#define INIT_FORCE  80
#define CALLED_FORCE 150
#define INIT_CHANCE 300

int8_t signStatus=-1;
bool newSign=false;
uint32_t SignCodeNumb=999999;
int32_t countdown=0;
int32_t initialCountdown=20;
int16_t timeToSleep=SLEEP_TIMEOUT_INITIAL;


int16_t counter=0;  ///to remove

// Sleep related
IWDG_t Iwdg;
bool FirstTime = true;  // First time after reset
void GoSleep() {
    // Start LSI
    Clk.LsiEnable();
    // Start IWDG
    Iwdg.SetTimeout(4500);
    Iwdg.Enable();
    // Enter standby mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    PWR->CR = PWR_CR_PDDS;
    PWR->CR |= PWR_CR_CWUF;
    __WFI();
}

class Process_t {
private:
    int16_t AccVector[ACC_VECTOR_SZ];
    void AperiodicFilter(int16_t T, int16_t *PrevValue, int16_t NewValue) {
        if(FirstTime) *PrevValue = NewValue;
        else *PrevValue = *PrevValue + ((NewValue - *PrevValue) / T);
        if (Abs(NewValue - *PrevValue)>STABLE_TOLERANCE){
            timeToSleep=SLEEP_TIMEOUT;
        }
    }
    int16_t Abs(int16_t v) { return (v<0)? -v : v; }
public:
    uint8_t CurrentSubsign;
    void AddAcc(uint8_t ID, int16_t *PAcc);
    int16_t IsSimilar(const int16_t *PSign, const int16_t *PSignMask);
    void PrintVect() {
        for(uint8_t i=0; i<21; i++) Uart.Printf("%d,", AccVector[i]);
        Uart.Printf("\r\n");
    }
} Process;

void Process_t::AddAcc(uint8_t ID, int16_t *PAcc) {
    AperiodicFilter(T_FILTER, &AccVector[ID*3+0], PAcc[0]);
    AperiodicFilter(T_FILTER, &AccVector[ID*3+1], PAcc[1]);
    AperiodicFilter(T_FILTER, &AccVector[ID*3+2], PAcc[2]);
}

int16_t Process_t::IsSimilar(const int16_t *PSign, const int16_t *PSignMask) {
    double xS, xC, xX; //sign coord vs current coord vs cross.
    double yS, yC, yX; //cross is called X, because C is already in use
    double zS, zC, zX;

    int16_t summAngle=0;

    for (uint8_t i=0; i<(ACC_VECTOR_SZ/3); i++){
        xS=PSign[i*3];
        yS=PSign[i*3+1];
        zS=PSign[i*3+2];

        xC=AccVector[i*3];
        yC=AccVector[i*3+1];
        zC=AccVector[i*3+2];

        xX=yS*zC-zS*yC;
        yX=-xS*zC+zS*xC;
        zX=xS*yC-yS*xC;

        double dotL=double(xS*xC+yS*yC+zS*zC);
        double crossL=sqrt(double(xX*xX+yX*yX+zX*zX));
        double signL=sqrt(double(xS*xS+yS*yS+zS*zS));
        double curL=sqrt(double(xC*xC+yC*yC+zC*zC));


        crossL=crossL/(signL*curL); //sin(alpha)=y/d
        dotL=dotL/(signL*curL); //cos(alpha)=x/d

        //let d be 1

        double angle=(atan2(crossL,dotL)*180/M_PI);

        summAngle=summAngle+int(angle)*PSignMask[i];

        if (int(angle) > TOLERANCE) return -1;
    }
    return summAngle;
}

// ========================= Timer subsystem ===================================
#define TMR_DELAY   1999
systime_t Time;

static void resetSign(){
    GreenLed.Off();
    RedLed.Off();
    YellowLed.Off();
    BlueLed.Off();
    UvLed.Off();
    Vibro.Off();
}

static void signExecForce(){
       Vibro.On(VIBRO_TOP);
       chThdSleepMilliseconds(500);
       Vibro.On(INIT_FORCE);
}

static int8_t analize(uint32_t code){
    uint32_t sub=0;
    int8_t newStatus=IDLE_STATUS;

    sub=code%100;

    switch(sub){
        case AARD_SOURCE_DONE:
            newStatus=AARD_SOURCE_STATUS;
        break;
        case IGNI_SOURCE_DONE:
            newStatus=IGNI_SOURCE_STATUS;
        break;
        case KVEN_SOURCE_DONE:
            newStatus=KVEN_SOURCE_STATUS;
        break;
        case IRDEN_SOURCE_DONE:
            newStatus=IRDEN_SOURCE_STATUS;
        break;
        case GELIO_SOURCE_DONE:
            newStatus=GELIO_SOURCE_STATUS;
        break;
        case RESET_DONE:
            newStatus=RESET_STATUS;
        break;
    }

    sub=code%10000;

    switch(sub){
        case AARD_FORCE_DONE:
            newStatus=AARD_FORCE_STATUS;
        break;
        case IGNI_FORCE_DONE:
            newStatus=IGNI_FORCE_STATUS;
        break;
        case KVEN_FORCE_DONE:
            newStatus=KVEN_FORCE_STATUS;
        break;
        case IRDEN_FORCE_DONE:
            newStatus=IRDEN_FORCE_STATUS;
        break;
        case GELIO_FORCE_DONE:
            newStatus=GELIO_FORCE_STATUS;
        break;
    }

    sub=code;

    switch(sub){
        case AARD_EXEC_DONE:
            newStatus=AARD_EXEC_STATUS;
        break;
        case IGNI_EXEC_DONE:
            newStatus=IGNI_EXEC_STATUS;
        break;
        case KVEN_EXEC_DONE:
            newStatus=KVEN_EXEC_STATUS;
        break;
        case IRDEN_EXEC_DONE:
            newStatus=IRDEN_EXEC_STATUS;
        break;
        case GELIO_EXEC_DONE:
            newStatus=GELIO_EXEC_STATUS;
        break;
    }

    return newStatus;

}

static void execStatus(){
    switch (signStatus){

        case IDLE_STATUS:
        case RESET_STATUS:
            resetSign();
        break;

        case AARD_SOURCE_STATUS:
        case IGNI_SOURCE_STATUS:
        case KVEN_SOURCE_STATUS:
        case IRDEN_SOURCE_STATUS:
        case GELIO_SOURCE_STATUS:
            Vibro.On(INIT_FORCE);
        break;

        case AARD_FORCE_STATUS:
        case IGNI_FORCE_STATUS:
        case KVEN_FORCE_STATUS:
        case IRDEN_FORCE_STATUS:
        case GELIO_FORCE_STATUS:
            resetSign();
            Vibro.On(CALLED_FORCE);
        break;

        case AARD_EXEC_STATUS:
            BlueLed.On(LED_VALUE);
            signExecForce();
        break;

        case IGNI_EXEC_STATUS:
            RedLed.On(LED_VALUE);
            signExecForce();
        break;

        case KVEN_EXEC_STATUS:
            YellowLed.On(LED_VALUE);
            signExecForce();
        break;

        case IRDEN_EXEC_STATUS:
            GreenLed.On(LED_VALUE);
            signExecForce();
        break;

        case GELIO_EXEC_STATUS:
            UvLed.On(LED_VALUE);
            signExecForce();
        break;
    }
}

void CheckTimer() {
    systime_t Now = chTimeNow();
    if((Now - Time) >= TMR_DELAY) {
        Time = Now;
        if (signStatus%3==0){
            resetSign();
        }
    }
}

static void TmrReset() {
    Time = chTimeNow();
}

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);

static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");
    int16_t bestSummAngle=180*6;
    int16_t summAngle=0;
    uint32_t tmpCode = 0;
    int8_t tmpStatus = -1;
    uint32_t newCode = 0;
    int8_t newStatus = -1;
//    int16_t chance=INIT_CHANCE;
/*
 -1 - idle
 0,1,2 - aard inited/forced/executed        blue
 3,4,5 - geliotrop inited/forced/executed   violet
 6,7,8 - igni inited/forced/executed        red
 9,10,11 - irden inited/forced/executed     green
 12,13,14 - kven inited/forced/executed     yellow
 */
    TmrReset();

    while(1) {
        chThdSleepMilliseconds(9);
        if(!App.Enabled) continue;  // Do not do anything

        timeToSleep=timeToSleep-1;
//        Uart.Printf("%d\r\n",timeToSleep);
        if (timeToSleep<0){
            timeToSleep=0;
//            Uart.Printf("Zzzz");
#ifdef SLEEP_ENABLED
            GoSleep();
#endif
        }

        countdown=countdown-1;
        if (countdown<0){
            countdown=0;
            signStatus=IDLE_STATUS;
            resetSign();
            SignCodeNumb=(SignCodeNumb*100)%1000000+99;
        }

        for(uint8_t i=0; i<ACC_CNT; i++) {
            if(Acc[i].IsOperational) {
                Acc[i].ReadAccelerations();
//                Uart.Printf("%u %d %d %d; ", i, Acc[i].a[0], Acc[i].a[1], Acc[i].a[2]);
                // Copy accelerations to vector
                Process.AddAcc(i, &Acc[i].a[0]);
                bestSummAngle=180*6;
                newCode=SignCodeNumb;
                newStatus=signStatus;
//                for(uint8_t j=0; j<SUBSIGN_CNT; j++) {
//                    summAngle=Process.IsSimilar(Subsign[j],SubsignMask[j]);
//
//                    if (summAngle>-1){
//                        tmpCode=(SignCodeNumb*100)%1000000+j;
//                        tmpStatus=analize(tmpCode);
//                        if ((tmpStatus>-1)&&(summAngle<bestSummAngle)) {
//                             newCode=tmpCode;
//                             newStatus=tmpStatus;
//                             bestSummAngle=summAngle;
//                             Process.CurrentSubsign = j;
//                             countdown=initialCountdown;
//                             Uart.Printf("%d\r\n",j);
//                        }
//                    }
//
//                }
//                SignCodeNumb=newCode;
//                if (signStatus!=newStatus){
//                    signStatus=newStatus;
//                    execStatus();
//                }

            }
            else {
                Uart.Printf("Acc %u fail\r", i);
                RedLed.On(LED_TOP);
                YellowLed.On(LED_TOP);
                Acc[i].Init();
            }
        } // for i
        FirstTime = false;

        counter=counter+1;

        if (counter%10==0){
          counter=0;
          Process.PrintVect();
        }
    } // while 1

//
//                IsSimilar=-1;
//                for(uint8_t j=0; j<SUBSIGN_CNT; j++) {
//                    if(Process.IsSimilar(Subsign[j],SubsignMask[j])) {
////                        Process.CurrentSubsign = j;
////                        Uart.Printf("Subs: %u\r", j);
//                        //RedLed.On(54);
//                        IsSimilar=j;
//                    }
//                }
//
//                if(IsSimilar != OldSim) {
//                    Uart.Printf("S: %d\r", IsSimilar);
//                    OldSim = IsSimilar;
//                }
//
//                //if(!IsSimilar) RedLed.Off();
//                switch (IsSimilar){
//                    case 0:
//                        resetSign();
//                        Vibro.On(INIT_FORCE);
//                        signStatus=0;
//                        TmrReset();
//                    break;
//                    case 1:
//                        if ((signStatus == 0)||(signStatus == 1)){
//                            signStatus=1;
//                            Vibro.On(CALLED_FORCE);
//                        }else{
//                            if ((signStatus == 3)||(signStatus == 4)){
//                                signStatus=4;
//                                Vibro.On(CALLED_FORCE);
//                            }else{
//                                if ((signStatus == 6)||(signStatus == 7)){
//                                    signStatus=7;
//                                    Vibro.On(CALLED_FORCE);
//                                }else{
//                                    if ((signStatus == 9)||(signStatus == 10)){
//                                        signStatus=10;
//                                        Vibro.On(CALLED_FORCE);
//                                    }else{
//                                        if ((signStatus == 12)||(signStatus == 13)){
//                                            signStatus=13;
//                                            Vibro.On(CALLED_FORCE);
//                                        }else{
//                                            resetSign();
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    break;
//                    case 2:
//                        if ((signStatus == 1)||(signStatus == 2)){
//                            signStatus=2;
//                            BlueLed.On(LED_TOP_BRT);
//                            //chThdSleepMilliseconds(2000);
//                            //resetSign();
//                            chance=INIT_CHANCE;
//                            signExecForce();
//                        }else{
//                            if ((signStatus == 7)||(signStatus == 8)){
//                                signStatus=8;
//                                RedLed.On(LED_TOP_BRT);
//                                //chThdSleepMilliseconds(2000);
//                                //resetSign();
//                                chance=INIT_CHANCE;
//                                signExecForce();
//                            }else{
//                                resetSign();
//                            }
//                        }
//                    break;
//                    case 3:
//                        resetSign();
//                        Vibro.On(INIT_FORCE);
//                        signStatus=3;
//                        TmrReset();
//                    break;
//                    case 4:
//                        if ((signStatus == 4)||(signStatus == 5)){
//                            signStatus=5;
//                            UvLed.On(LED_TOP_BRT);
//                            //chThdSleepMilliseconds(2000);
//                            //resetSign();
//                            chance=INIT_CHANCE;
//                            signExecForce();
//                        }else{
//                            resetSign();
//                        }
//                    break;
//                    case 5:
//                        resetSign();
//                        Vibro.On(INIT_FORCE);
//                        signStatus=6;
//                        TmrReset();
//                    break;
//                    case 6:
//                        resetSign();
//                        Vibro.On(INIT_FORCE);
//                        signStatus=9;
//                        TmrReset();
//                    break;
//                    case 7:
//                        if ((signStatus == 10)||(signStatus == 11)){
//                            signStatus=11;
//                            GreenLed.On(LED_TOP_BRT);
//                            //chThdSleepMilliseconds(2000);
//                            //resetSign();
//                            chance=INIT_CHANCE;
//                            signExecForce();
//                        }else{
//                            if ((signStatus == 13)||(signStatus == 14)){
//                                signStatus=14;
//                                YellowLed.On(LED_TOP_BRT);
//                                //chThdSleepMilliseconds(2000);
//                                //resetSign();
//                                chance=INIT_CHANCE;
//                                signExecForce();
//                            }else{
//                                resetSign();
//                            }
//                        }
//
//                    break;
//                    case 8:
//                        resetSign();
//                        Vibro.On(INIT_FORCE);
//                        signStatus=12;
//                        TmrReset();
//                    break;
//                    default:
//                        if (((signStatus-2)%3==0)){
//                            chance=chance-1;
//                            if (chance<0) resetSign();
//                        }
//                    break;
//                }
//                CheckTimer();
//
//
//            }
//            //else Acc[i].Init();
//        }
//        //Process.PrintVect();
//        GreenLed.Off();
//    } // if enabled
    return 0;
}


// =============================== App class ===================================
void App_t::Init() {
    Enabled = true;

    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
