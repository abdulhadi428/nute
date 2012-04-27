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
#include <string.h>
#include <stdlib.h>
#include "kl_util.h"
#include "acc_mma.h"

#define SND_COUNT_MAX   100
#define FNAME_LNG_MAX   13

typedef struct {
     char Filename[FNAME_LNG_MAX];
     uint32_t ProbBottom, ProbTop;
} Snd_t;

typedef struct {
    Snd_t Phrases[SND_COUNT_MAX];
    uint32_t Count;
    uint32_t ProbSumm;
} SndList_t;

SndList_t SndList;

Acc_t Acc;

// Prototypes
void GeneralInit(void);
bool ReadConfig(void);

void EVENT_Jolt(void);

// ============================ Implementation ================================
int main(void) {
    UART_Init();
    Delay.ms(100);
    klPrintf("Sugar is up to cry\r");

    GeneralInit();
    uint32_t tmr;
    Delay.Reset(&tmr);
    // ==== Main cycle ====
    while(1) {
        ESnd.Task();
        Acc.Task();
        i2cMgr.Task();
        //if (Delay.Elapsed(&tmr, 2000)) EVENT_SomeoneDetected();
    } // while(1)
    return 0;
}

void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    // Init peripheral
    Delay.Init();
    // Sound
    Vs.Init();
    ESnd.Init();

    // Accelerometer
    i2cMgr.Init();
    Acc.Init();
    Acc.EvtTrigger = EVENT_Jolt;
    Acc.Enable();

    // Register filesystem
    f_mount(0, &SD.FatFilesystem);
    if (!ReadConfig()) {
        klPrintf("Config read error\r");
        while(1);    // nothing to do if config not read
    }
    // Play initial sound
    ESnd.Play("alive.wav");
}

bool ReadConfig(void) {
    // ==== Sound ====
    if(!ReadUint32("Sound", "Count", "settings.ini", &SndList.Count)) return false;
    klPrintf("Count: %i\r", SndList.Count);
    if (SndList.Count == 0) return false;
    char SndKey[18]="Sound";
    char *c;
    SndList.ProbSumm = 0;
    // Read sounds data
    uint32_t Probability = 0;
    for (uint32_t i=0; i<SndList.Count; i++) {
        // Build SndKey
        c = UintToStr(i+1, &SndKey[5]);   // first symbol after "Sound"
        strcpy(c, "Name");
        //klPrintf("%s\r", SndKey);
        // Read filename and probability
        if(!ReadString("Sound", SndKey, "settings.ini", SndList.Phrases[i].Filename, FNAME_LNG_MAX)) return false;
        strcpy(c, "Prob");
        //klPrintf("%s\r", SndKey);
        if(!ReadUint32 ("Sound", SndKey, "settings.ini", &Probability)) return false;
        // Calculate probability boundaries
        SndList.Phrases[i].ProbBottom = SndList.ProbSumm;
        SndList.ProbSumm += Probability;
        SndList.Phrases[i].ProbTop = SndList.ProbSumm;
    }
    // for (uint32_t i=0; i<SndList.Count; i++) klPrintf("%u %S %u\r", i, SndList.Phrases[i].Filename, SndList.Phrases[i].Probability); // DEBUG

    // ==== Accelerometer ====
    if(!ReadUint32("Accelerometer", "ThresholdTop",    "settings.ini", &Acc.ThresholdTop)) return false;
    if(!ReadUint32("Accelerometer", "ThresholdBottom", "settings.ini", &Acc.ThresholdBottom)) return false;
    if(!ReadUint32("Accelerometer", "Delay",           "settings.ini", &Acc.Delay_ms)) return false;
    Acc.Delay_ms *= 1000;   // Convert seconds to milliseconds

    return true;
}

// ================================== Events ===================================
void EVENT_Jolt(void) {
    if (ESnd.State == sndPlaying) return;   // speaking now
    klPrintf("Jolting\r");
    // Generate random
    uint32_t r = rand() % SndList.ProbSumm + 1; // [1; Probsumm]
    // Select phrase
    uint32_t i;
    for (i=0; i<SndList.Count-1; i++) { // do not check last phrase
        if((r >= SndList.Phrases[i].ProbBottom) && (r <= SndList.Phrases[i].ProbTop)) break;
    }
    // Play phrase
    ESnd.Play(SndList.Phrases[i].Filename);
    //DebugProb.count++;
    //DebugProb.c[i]++;
    //klPrintf("Total: %u   c1: %u  c2: %u  c3: %u  c4: %u\r", DebugProb.count, DebugProb.c[0], DebugProb.c[1], DebugProb.c[2], DebugProb.c[3]);
}
