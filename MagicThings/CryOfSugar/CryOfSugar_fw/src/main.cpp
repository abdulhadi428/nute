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
#include "keys.h"
#include "kl_lib.h"
#include "led.h"
#include <stdlib.h>

#include "stm32f10x_rcc.h"

#define FNAME_LNG_MAX   13

struct Settings_t {
    char SndKeyBeep[FNAME_LNG_MAX], Snd[3][FNAME_LNG_MAX];
    uint32_t Duration[3];
    // Methods
    bool Read(void);
} Settings;

uint32_t EMode;
uint32_t ETmr, LedTmr;
bool WasPlayed;

// Prototypes
void GeneralInit(void);
void Event_Kbd(uint8_t AKey);
void ModeTask(void);
inline void SetLed(uint16_t ABrightness) {
    TIM1->CCR1 = ABrightness;
}

// ============================ Implementation ================================
int main(void) {
    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        Keys.Task();
        ESnd.Task();
        ModeTask();
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
    Keys.Init();
    Keys.EvtKbd = Event_Kbd;
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

    // GPIO
    klGpio::SetupByN(GPIOA, 8, GPIO_Mode_AF_PP);
    // ==== Timer1 as PWM ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM1->CR1 = 0x01;   // enable
    TIM1->CR2 = 0;
    TIM1->CCMR1 = 0x60;
    TIM1->CCER = 0x01;
    TIM1->PSC = 27;
    TIM1->ARR = 252;
    TIM1->CCR1 = 0;
    TIM1->BDTR = 0xC000;

    srand(7);
    // Play initial sound
    ESnd.Play("alive.wav");
}

bool Settings_t::Read(void) {
    klPrintf("Reading settings\r");
    // Sound names
    if(!ReadString("Sound", "KeyBeep", "settings.ini", Settings.SndKeyBeep, FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Sound1",  "settings.ini", Settings.Snd[0],     FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Sound2",  "settings.ini", Settings.Snd[1],     FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Sound3",  "settings.ini", Settings.Snd[2],     FNAME_LNG_MAX)) return false;

    // Timings
    if(!ReadUint32("Timings", "Duration1", "settings.ini", &Settings.Duration[0])) return false;
    if(!ReadUint32("Timings", "Duration2", "settings.ini", &Settings.Duration[1])) return false;

    Settings.Duration[0] = Settings.Duration[0] * 1000;
    Settings.Duration[1] = Settings.Duration[1] * 1000;

    return true;
}

// ================================== Keyboard =================================
void Event_Kbd(uint8_t AKey) {
    klPrintf("Key = %u\r", AKey);
    ESnd.Play(Settings.SndKeyBeep);     // Play sound
    if (AKey == EMode) {
        ESnd.Stop();
        EMode = 0;
    }
    else {
        EMode = AKey;
        WasPlayed = false;
        Delay.Reset(&ETmr);
        klPrintf("Set mode %u\r", EMode);
    }
}

// =================================== Mode ====================================
void ModeTask(void) {
    static uint16_t IBrightness=0;
    static uint32_t IDelay = 9;
    static bool IsOn = false;
    switch (EMode) {
        case 0: // switch led off when sound is over
            if (ESnd.State == sndStopped) SetLed(0);
            break;

        case 1:
            if (Delay.Elapsed(&ETmr, Settings.Duration[0])) EMode = 0;
            else {
                if (ESnd.State == sndStopped) {
                    ESnd.Play(Settings.Snd[0]);
                    Delay.Reset(&LedTmr);
                    IBrightness = 1;
                    IDelay = 36;
                    SetLed(0);
                }
                else if (Delay.Elapsed(&LedTmr, IDelay)) {
                    SetLed(IBrightness++);
                    if (IBrightness > 108) IBrightness = 0;
                    if (IBrightness < 11) IDelay = 54;
                    else if (IBrightness < 27) IDelay = 24;
                    else IDelay = 9;
                }
            }
            break;

        case 2:
            if (Delay.Elapsed(&ETmr, Settings.Duration[1])) EMode = 0;
            else {
                if (ESnd.State == sndStopped) {
                    ESnd.Play(Settings.Snd[1]);
                    Delay.Reset(&LedTmr);
                    IBrightness = 108;
                    IDelay = 9;
                    SetLed(0);
                }
                else if (Delay.Elapsed(&LedTmr, IDelay)) {
                    SetLed(IBrightness--);
                    if (IBrightness == 0) IBrightness = 108;
                    if (IBrightness < 11) IDelay = 11;
                    else if (IBrightness < 27) IDelay = 4;
                    else IDelay = 1;
                }
            }
            break;

        case 3:
            if (!WasPlayed) {
                if (ESnd.State == sndStopped) {
                    ESnd.Play(Settings.Snd[2]);
                    Delay.Reset(&LedTmr);
                    WasPlayed = true;
                    SetLed(250);
                    IsOn = true;
                    IDelay = 1+rand()%54;
                }
            }
            else {
                if (ESnd.State == sndStopped) EMode = 0;
                else {
                    if (Delay.Elapsed(&LedTmr, IDelay)) {
                        if(IsOn) SetLed(0);
                        else SetLed(250);
                        IsOn = !IsOn;
                        IDelay = 1+rand()%54;
                    }
                }
            }
            break;

        default: break;
    }
}


