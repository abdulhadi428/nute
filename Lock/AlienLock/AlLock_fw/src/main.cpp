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
#define CODE_LNG_MAX    6

#define WAITING_TIME    3006    // wait before drop

struct Settings_t {
    char KeyBeep[FNAME_LNG_MAX];
    char KeyDrop[FNAME_LNG_MAX];
    char PassError[FNAME_LNG_MAX];
    char Open[FNAME_LNG_MAX];
    char Close[FNAME_LNG_MAX];
    char Code[CODE_LNG_MAX+1], ServiceCode[CODE_LNG_MAX+1]; // Because of trailing \0
    uint8_t CodeLength, ServiceCodeLength;
    bool Read(void);
} Settings;

enum EntrResult_t {entNA, entError, entOpen};
struct Codecheck_t {
    uint32_t Timer;
    char EnteredCode[CODE_LNG_MAX+1];   // Because of trailing \0
    uint8_t EnteredLength;
    EntrResult_t EnterResult;
    void Task(void);
    void Reset(void) { EnteredLength=0; EnterResult=entNA; }
} Codecheck;


// Prototypes
void GeneralInit(void);
void Event_Kbd(void);
void Event_Open(void);
void Event_Close(void);

// ============================ Implementation ================================
int main(void) {
    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        Keys.Task();
        ESnd.Task();
        Codecheck.Task();
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

// ============================ Codecheck ======================================
void Codecheck_t::Task(void) {
    if (ESnd.State == sndPlaying) return;
    switch (Codecheck.EnterResult) {
        case entNA: // check if input timeout
            if((EnteredLength != 0) && Delay.Elapsed(&Timer, WAITING_TIME)) {
                Reset();
                ESnd.Play(Settings.KeyDrop);
            }
            break;
        case entError:
            Reset();
            ESnd.Play(Settings.PassError);
            break;
        case entOpen:
            Reset();
            Event_Open();
            break;
    } // switch
}



bool Settings_t::Read(void) {
    // Read sound names
    if(!ReadString("Sound", "KeyBeep", "lock.ini", Settings.KeyBeep, FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "KeyDrop", "lock.ini", Settings.KeyDrop, FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "PassError", "lock.ini", Settings.PassError, FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Open", "lock.ini", Settings.Open, FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Close", "lock.ini", Settings.Close, FNAME_LNG_MAX)) return false;

    // Read codes
    if(!ReadString("Code", "Code", "lock.ini", Settings.Code, CODE_LNG_MAX)) return false;
    Settings.CodeLength = strlen(Settings.Code);
    if(!ReadString("Code", "ServiceCode", "lock.ini", Settings.ServiceCode, CODE_LNG_MAX)) return false;
    Settings.ServiceCodeLength = strlen(Settings.ServiceCode);

    return true;
}

// ================================== Events ===================================
void Event_Kbd(void) {
    if (Codecheck.EnterResult != entNA) return; // disable keys in case of existing result
    uint8_t FKey = (Keys.Kbd[0] == KEY_NONE)? Keys.Kbd[1] : Keys.Kbd[0];
    //klPrintf("Key = %u %u\r", Keys.Kbd[0], Keys.Kbd[1]);
    // Digit entered
    if((FKey >= 0) && (FKey <=9) && (Settings.CodeLength != 0)) {   // Do not use digits in case of empty code
        ESnd.Play(Settings.KeyBeep);
        Codecheck.EnteredCode[Codecheck.EnteredLength] = '0' + FKey;
        Codecheck.EnteredLength++;
        if(Codecheck.EnteredLength == Settings.CodeLength) {
            //klPrintf("Entered: %s; real: %s\r", Codecheck.EnteredCode, Settings.Code);
            if(strcmp(Settings.Code, Codecheck.EnteredCode) == 0) Codecheck.EnterResult = entOpen;  // Equal
            else Codecheck.EnterResult = entError;
        }
        Delay.Reset(&Codecheck.Timer);
    }
    else {  // none-digit
        if(FKey == KEY_STAR) Delay.Bypass(&Codecheck.Timer, WAITING_TIME);  // Drop code
        if((FKey == KEY_HASH) && (Settings.CodeLength == 0)) Codecheck.EnterResult = entOpen;   // Open door in case of empty code
    }
}

void Event_Open(void) {
    ESnd.Play(Settings.Open);
}

void Event_Close(void) {
    ESnd.Play(Settings.Close);
}
