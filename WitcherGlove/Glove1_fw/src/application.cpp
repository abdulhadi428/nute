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
#include "infrared.h"

App_t App;

int8_t signStatus=-1;
bool newSign=false;


#define TOLERANCE   200
#define T_FILTER    5

#define INIT_FORCE  80
#define CALLED_FORCE 150
#define INIT_CHANCE 300

class Process_t {
private:
    int16_t AccVector[ACC_VECTOR_SZ];
    void AperiodicFilter(int16_t T, int16_t *PrevValue, int16_t NewValue) {
        *PrevValue = *PrevValue + ((NewValue - *PrevValue) / T);
    }
    int16_t Abs(int16_t v) { return (v<0)? -v : v; }
public:
    uint8_t CurrentSubsign;
    void AddAcc(uint8_t ID, int16_t *PAcc);
    bool IsSimilar(const int16_t *PSign, const int16_t *PSignMask);
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

bool Process_t::IsSimilar(const int16_t *PSign, const int16_t *PSignMask) {
    for(uint8_t i=0; i<ACC_VECTOR_SZ; i++)
        if(Abs(PSign[i] - AccVector[i])*PSignMask[i] > TOLERANCE) return false;
    return true;
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
    signStatus=-1;
    newSign=true;
}

static void signExecForce(){
    if (newSign){
       Vibro.On(VIBRO_TOP);
       chThdSleepMilliseconds(500);
       Vibro.On(INIT_FORCE);
       newSign=false;
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

    // IR test
    IR.TransmitWord(0x0000, 100);
    while(1) {
        chThdSleepMilliseconds(45);

    }


    int8_t IsSimilar=-1;
    int16_t chance=INIT_CHANCE;



/*
 -1 - idle
 0,1,2 - aard inited/forced/executed        blue
 3,4,5 - geliotrop inited/forced/executed   violet
 6,7,8 - igni inited/forced/executed        red
 9,10,11 - irden inited/forced/executed     green
 12,13,14 - kven inited/forced/executed     yellow
 */

/*
    Vibro.On(100);
    chThdSleepMilliseconds(999);
    Vibro.Off();
*/
    TmrReset();

    while(1) {
        chThdSleepMilliseconds(9);
        if(!App.Enabled) continue;  // Do not do anything

       /* YellowLed.On(LED_TOP_BRT);
        RedLed.On(LED_TOP_BRT);
        GreenLed.On(LED_TOP_BRT);
        BlueLed.On(LED_TOP_BRT);
        UvLed.On(LED_TOP_BRT);*/
        for(uint8_t i=0; i<ACC_CNT; i++) {
            if(Acc[i].IsOperational) {
                Acc[i].ReadAccelerations();
                //Uart.Printf("%u %d %d %d; ", i, Acc[i].a[0], Acc[i].a[1], Acc[i].a[2]);
                // Copy accelerations to vector
                Process.AddAcc(i, &Acc[i].a[0]);
                IsSimilar=-1;
                for(uint8_t j=0; j<SUBSIGN_CNT; j++) {
                    if(Process.IsSimilar(Subsign[j],SubsignMask[j])) {
//                        Process.CurrentSubsign = j;
//                        Uart.Printf("Subs: %u\r", j);
                        //RedLed.On(54);
                        IsSimilar=j;
                    }


                }
                //if(!IsSimilar) RedLed.Off();
                switch (IsSimilar){
                    case 0:
                        resetSign();
                        Vibro.On(INIT_FORCE);
                        signStatus=0;
                        TmrReset();
                    break;
                    case 1:
                        if ((signStatus == 0)||(signStatus == 1)){
                            signStatus=1;
                            Vibro.On(CALLED_FORCE);
                        }else{
                            if ((signStatus == 3)||(signStatus == 4)){
                                signStatus=4;
                                Vibro.On(CALLED_FORCE);
                            }else{
                                if ((signStatus == 6)||(signStatus == 7)){
                                    signStatus=7;
                                    Vibro.On(CALLED_FORCE);
                                }else{
                                    if ((signStatus == 9)||(signStatus == 10)){
                                        signStatus=10;
                                        Vibro.On(CALLED_FORCE);
                                    }else{
                                        if ((signStatus == 12)||(signStatus == 13)){
                                            signStatus=13;
                                            Vibro.On(CALLED_FORCE);
                                        }else{
                                            resetSign();
                                        }
                                    }
                                }
                            }
                        }
                    break;
                    case 2:
                        if ((signStatus == 1)||(signStatus == 2)){
                            signStatus=2;
                            BlueLed.On(LED_TOP_BRT);
                            //chThdSleepMilliseconds(2000);
                            //resetSign();
                            chance=INIT_CHANCE;
                            signExecForce();
                        }else{
                            if ((signStatus == 7)||(signStatus == 8)){
                                signStatus=8;
                                RedLed.On(LED_TOP_BRT);
                                //chThdSleepMilliseconds(2000);
                                //resetSign();
                                chance=INIT_CHANCE;
                                signExecForce();
                            }else{
                                resetSign();
                            }
                        }
                    break;
                    case 3:
                        resetSign();
                        Vibro.On(INIT_FORCE);
                        signStatus=3;
                        TmrReset();
                    break;
                    case 4:
                        if ((signStatus == 4)||(signStatus == 5)){
                            signStatus=5;
                            UvLed.On(LED_TOP_BRT);
                            //chThdSleepMilliseconds(2000);
                            //resetSign();
                            chance=INIT_CHANCE;
                            signExecForce();
                        }else{
                            resetSign();
                        }
                    break;
                    case 5:
                        resetSign();
                        Vibro.On(INIT_FORCE);
                        signStatus=6;
                        TmrReset();
                    break;
                    case 6:
                        resetSign();
                        Vibro.On(INIT_FORCE);
                        signStatus=9;
                        TmrReset();
                    break;
                    case 7:
                        if ((signStatus == 10)||(signStatus == 11)){
                            signStatus=11;
                            GreenLed.On(LED_TOP_BRT);
                            //chThdSleepMilliseconds(2000);
                            //resetSign();
                            chance=INIT_CHANCE;
                            signExecForce();
                        }else{
                            if ((signStatus == 13)||(signStatus == 14)){
                                signStatus=14;
                                YellowLed.On(LED_TOP_BRT);
                                //chThdSleepMilliseconds(2000);
                                //resetSign();
                                chance=INIT_CHANCE;
                                signExecForce();
                            }else{
                                resetSign();
                            }
                        }

                    break;
                    case 8:
                        resetSign();
                        Vibro.On(INIT_FORCE);
                        signStatus=12;
                        TmrReset();
                    break;
                    default:
                        if (((signStatus-2)%3==0)){
                            chance=chance-1;
                            if (chance<0) resetSign();
                        }
                    break;
                }
                CheckTimer();


            }
            //else Acc[i].Init();
        }
        //Process.PrintVect();
        GreenLed.Off();
    } // if enabled
    return 0;
}


// =============================== App class ===================================
void App_t::Init() {
    Enabled = true;

    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
