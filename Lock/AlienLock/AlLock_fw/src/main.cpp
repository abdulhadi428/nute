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
#include "keys.h"

#define FNAME_LNG_MAX   13

struct Settings_t {
    char KeyBeep[FNAME_LNG_MAX];
    char KeyDrop[FNAME_LNG_MAX];
    char PassError[FNAME_LNG_MAX];
    char Open[FNAME_LNG_MAX];
    char Close[FNAME_LNG_MAX];
    int32_t Code, ServiceCode;
    bool Read(void);
} Settings;


// Prototypes
void GeneralInit(void);
void Event_Kbd(void);

// ============================ Implementation ================================
int main(void) {
    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        Keys.Task();
        ESnd.Task();
        //Sensor.Task();
    } // while(1)
    return 0;
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
    Keys.EvtKbd[0] = Event_Kbd;
    Keys.EvtKbd[1] = Event_Kbd;
    // Sensor
//    Sensor.Init();
//    Sensor.EvtDetected = EVENT_SomeoneDetected;
//    // Sound
    Vs.Init();
    ESnd.Init();

    klPrintf("Lock is on\r");

    // Register filesystem
    f_mount(0, &SD.FatFilesystem);
    if (!Settings.Read()) while(1);    // nothing to do if config not read

    // Play initial sound
    ESnd.Play("alive.wav");
}

bool Settings_t::Read(void) {
    // Read sound names
    if(!ReadString("Sound", "KeyBeep", "lock.ini", Settings.KeyBeep)) return false;
    if(!ReadString("Sound", "KeyDrop", "lock.ini", Settings.KeyDrop)) return false;
    if(!ReadString("Sound", "PassError", "lock.ini", Settings.PassError)) return false;
    if(!ReadString("Sound", "Open", "lock.ini", Settings.Open)) return false;
    if(!ReadString("Sound", "Close", "lock.ini", Settings.Close)) return false;

    // Read codes
    if(!ReadInt32("Code", "Code", "lock.ini", &Settings.Code)) return false;
    if(!ReadInt32("Code", "ServiceCode", "lock.ini", &Settings.ServiceCode)) return false;

    return true;
}

// ================================== Events ===================================
void Event_Kbd(void) {
    //if (ESnd.State == sndPlaying) return;
    klPrintf("Key = %u %u\r", Keys.Kbd[0], Keys.Kbd[1]);
    ESnd.Play(Settings.KeyBeep);
}
