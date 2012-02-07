/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "sd.h"
#include "vs.h"
#include "media.h"
#include "kl_ini.h"
#include "sensors.h"
#include <string.h>
#include <stdlib.h>
#include "kl_util.h"

#define FNAME_LNG_MAX   13

// Prototypes
void GeneralInit(void);
bool ReadConfig(void);

// ============================ Implementation ================================
int main(void) {
    GeneralInit();
//    uint32_t tmr;
//    Delay.Reset(&tmr);
    // ==== Main cycle ====
    while(1) {
        //ESnd.Task();
        //Sensor.Task();
        //if (Delay.Elapsed(&tmr, 2000)) EVENT_SomeoneDetected();
    } // while(1)
    return 0;
}

void GeneralInit(void) {
    UART_Init();
    Delay.ms(100);
    klPrintf(" Lock is on\r");
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    // Init peripheral
    Delay.Init();
    // Sensor
//    Sensor.Init();
//    Sensor.EvtDetected = EVENT_SomeoneDetected;
//    // Sound
//    Vs.Init();
//    ESnd.Init();

    // Register filesystem
//    f_mount(0, &SD.FatFilesystem);
//    if (!ReadConfig()) while(1);    // nothing to do if config not read

    // Play initial sound
//    ESnd.Play("alive.wav");
}

bool ReadConfig(void) {
//    if(!ReadInt32("Sound", "Count", "portrait.ini", &tmp)) return false;
//    klPrintf("Count: %i\r", tmp);
//    if (tmp <= 0) return false;
//    char SndKey[18]="Sound";
//    char *c;
//    SndList.Count = (uint32_t)tmp;
//    SndList.ProbSumm = 0;
//    // Read sounds data
//    for (uint32_t i=0; i<SndList.Count; i++) {
//        // Build SndKey
//        c = UintToStr(i+1, &SndKey[5]);   // first symbol after "Sound"
//        strcpy(c, "Name");
//        //klPrintf("%s\r", SndKey);
//        // Read filename and probability
//        if(!ReadString("Sound", SndKey, "portrait.ini", SndList.Phrases[i].Filename)) return false;
//        strcpy(c, "Prob");
//        //klPrintf("%s\r", SndKey);
//        if(!ReadInt32 ("Sound", SndKey, "portrait.ini", &tmp)) return false;
//        Probability = (uint32_t)tmp;
//        // Calculate probability boundaries
//        SndList.Phrases[i].ProbBottom = SndList.ProbSumm;
//        SndList.ProbSumm += Probability;
//        SndList.Phrases[i].ProbTop = SndList.ProbSumm;
//    }
//    // for (uint32_t i=0; i<SndList.Count; i++) klPrintf("%u %S %u\r", i, SndList.Phrases[i].Filename, SndList.Phrases[i].Probability); // DEBUG
    return true;
}

// ================================== Events ===================================
void EVENT_SomeoneDetected(void) {
//    if (ESnd.State == sndPlaying) return;   // speaking now
//    klPrintf("Detected\r");
//    // Generate random
//    uint32_t r = rand() % SndList.ProbSumm + 1; // [1; Probsumm]
//    // Select phrase
//    uint32_t i;
//    for (i=0; i<SndList.Count-1; i++) // do not check last phrase
//        if((r >= SndList.Phrases[i].ProbBottom) && (r <= SndList.Phrases[i].ProbTop)) break;
//    // Play phrase
//    ESnd.Play(SndList.Phrases[i].Filename);
//    //DebugProb.count++;
    //DebugProb.c[i]++;
    //klPrintf("Total: %u   c1: %u  c2: %u  c3: %u  c4: %u\r", DebugProb.count, DebugProb.c[0], DebugProb.c[1], DebugProb.c[2], DebugProb.c[3]);
}
