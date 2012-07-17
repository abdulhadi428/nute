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
#include "kl_lib.h"

#define SND_COUNT_MAX   100
#define FNAME_LNG_MAX   13

struct Snd_t {
     char Filename[FNAME_LNG_MAX];
     uint32_t ProbBottom, ProbTop;
};

struct SndList_t {
    Snd_t Phrases[SND_COUNT_MAX];
    uint32_t Count;
    uint32_t ProbSumm;
} SndList;

enum DetectorState_t {dsWaitingTouch, dsDelayBeforeEvt, dsEvtProcessing, dsDelayAfterEvt};
class Detector_t {
private:
    DetectorState_t IState;
    uint32_t ITimer;
    klPin_t IPin;
    bool WasTouched;
public:
    uint32_t DelayBeforeEvt, DelayAfterEvt;
    void Event(void);
    void Init(void) {
        IPin.Init(GPIOC, 1, GPIO_Mode_IN_FLOATING);
        IState = dsWaitingTouch;
        WasTouched = false;
    }
    void Task(void) {
        switch (IState) {
            case dsWaitingTouch:
                if (Delay.Elapsed(&ITimer, 200)) {
                    if ((IPin == 1) and (!WasTouched)) {
                        IState = dsDelayBeforeEvt;
                        WasTouched = true;
                        klPrintf("Waiting before\r");
                    }
                    else if ((IPin == 0) and (WasTouched)){
                        WasTouched = false;
                    }
                }
                break;

            case dsDelayBeforeEvt:
                if (Delay.Elapsed(&ITimer, DelayBeforeEvt)) {
                    IState = dsEvtProcessing;
                    klPrintf("Processing\r");
                    Event();
                }
                break;

            case dsEvtProcessing:
                if (Delay.Elapsed(&ITimer, 450)) {
                    if (ESnd.State != sndPlaying) { // Song ended
                        IState = dsDelayAfterEvt;
                        klPrintf("Waiting after\r");
                        Delay.Reset(&ITimer);
                    }
                }
                break;

            case dsDelayAfterEvt:
                if (Delay.Elapsed(&ITimer, DelayAfterEvt)) {
                    IState = dsWaitingTouch;
                    klPrintf("Waiting touch\r");
                }
                break;
        } // switch
    }
} Detector;

// Prototypes
void GeneralInit(void);
bool ReadConfig(void);

// ============================ Implementation ================================
int main(void) {
    UART_Init();
    Delay.ms(100);
    klPrintf("Helm is up to sing\r");

    GeneralInit();
    uint32_t tmr;
    Delay.Reset(&tmr);
    // ==== Main cycle ====
    while(1) {
        ESnd.Task();
        Detector.Task();
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
    Detector.Init();
    // Sound
    Vs.Init();
    ESnd.Init();

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
    uint32_t FVolume=0;
    if(!ReadUint32("Sound", "Volume", "settings.ini", &FVolume)) return false;
    klPrintf("Volume: %i\r", FVolume);
    if(FVolume > 100) return false;
    Vs.SetVolume(FVolume);
    // Sounds
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

    // ==== Detector ====
    if(!ReadUint32("Detector", "DelayBefore", "settings.ini", &Detector.DelayBeforeEvt)) return false;
    if(!ReadUint32("Detector", "DelayAfter",  "settings.ini", &Detector.DelayAfterEvt))  return false;

    return true;
}

// ================================== Events ===================================
void Detector_t::Event(void) {
    if (ESnd.State == sndPlaying) return;   // speaking now
    // Select sound, do not play previous again
    uint32_t i;
    static uint32_t IPreviousIndx = 0;
    do {
        // Generate random
        uint32_t r = rand() % SndList.ProbSumm + 1; // [1; Probsumm]
        // Select sound by prob
        for (i=0; i<SndList.Count-1; i++) { // do not check last phrase
            if((r >= SndList.Phrases[i].ProbBottom) && (r <= SndList.Phrases[i].ProbTop)) break;
        }
    } while(i == IPreviousIndx);
    IPreviousIndx = i;
    // Play phrase
    klPrintf("Snd %u\r", i);
    ESnd.Play(SndList.Phrases[i].Filename);
    //DebugProb.count++;
    //DebugProb.c[i]++;
    //klPrintf("Total: %u   c1: %u  c2: %u  c3: %u  c4: %u\r", DebugProb.count, DebugProb.c[0], DebugProb.c[1], DebugProb.c[2], DebugProb.c[3]);
    //ESnd.Play("alive.wav");
}
