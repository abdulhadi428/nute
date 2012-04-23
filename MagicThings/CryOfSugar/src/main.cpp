/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include <stdint.h>
#include "stm32f10x.h"
#include "delay_util.h"
#include "sd.h"
#include "vs.h"
#include "media.h"
#include "kl_ini.h"
#include "kl_util.h"
#include "kl_lib.h"
#include <stdlib.h>

#define FNAME_LNG_MAX   13
#define SND_COUNT_MAX   100

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


struct Settings_t {
    uint32_t SndCount;
    char Snd[3][FNAME_LNG_MAX];
    // Methods
    bool Read(void);
} Settings;

// Prototypes
void GeneralInit(void);

// ============================ Implementation ================================
int main(void) {
    GeneralInit();
    // ==== Main cycle ====
    while(1) {
        ESnd.Task();
    } // while(1)
}

void GeneralInit(void) {
    UART_Init();
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    // Init peripheral
    Delay.Init();
    // Sound
    Vs.Init();
    ESnd.Init();

    // Register filesystem
    klPrintf("Mount fs\r");
    f_mount(0, &SD.FatFilesystem);
    if (!Settings.Read()) {
        klPrintf("Settings read error\r");
        while(1);    // nothing to do if config not read
    }
    srand(7);
    // Play initial sound
    ESnd.Play("alive.wav");
}

bool Settings_t::Read(void) {
    klPrintf("Reading settings\r");
    // Sound names
    if(!ReadString("Sound", "Sound1",  "settings.ini", Settings.Snd[0],     FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Sound2",  "settings.ini", Settings.Snd[1],     FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Sound3",  "settings.ini", Settings.Snd[2],     FNAME_LNG_MAX)) return false;

    return true;
}
