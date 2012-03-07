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
#include "leds_pca.h"
#include "i2c_mgr.h"
#include "adc.h"
#include "uart_cmd.h"
#include "kl_lib.h"

#define FNAME_LNG_MAX   13
#define CODE_LNG_MAX    6

klPin_t LaserPin;

// Prototypes
void GeneralInit(void);
void Event_Kbd(uint8_t AKbdSide, uint8_t AKey);
void LaserHallLaserOff(void) { LaserPin = 1; }

// Types
struct Settings_t {
    char SndKeyBeep[FNAME_LNG_MAX], SndKeyDrop[FNAME_LNG_MAX], SndPassError[FNAME_LNG_MAX], SndOpen[FNAME_LNG_MAX];
    char SndClose[FNAME_LNG_MAX], SndKeyCrash[FNAME_LNG_MAX];
    // Codes
    char CodeA[CODE_LNG_MAX+1], CodeB[CODE_LNG_MAX+1], ServiceCode[CODE_LNG_MAX+1]; // Because of trailing \0
    int8_t CodeALength, CodeBLength;     // Length of 0 means empty, negative length means crash
    char Complexity[4];
    // Colors
    Color_t ColorDoorOpen, ColorDoorOpening, ColorDoorClosed, ColorDoorClosing;
    uint32_t BlinkDelay;
    // Timings
    uint32_t DoorCloseDelay, KeyDropDelay;
    // Methods
    bool Read(void);
} Settings;

enum EntrResult_t {entNA, entError, entOpen};
struct Codecheck_t {
    uint32_t Timer;
    char EnteredCode[CODE_LNG_MAX+1];   // Because of trailing \0
    uint8_t EnteredLength;
    EntrResult_t EnterResult;
    void Task(void);
    void Reset(void) { EnteredLength=0; EnterResult=entNA; memset(EnteredCode, 0, CODE_LNG_MAX); }
} Codecheck;

enum DoorState_t {dsClosed, dsOpened, dsOpening, dsClosing};
class Door_t {
private:
    klPin_t IPin;
    static void EvtJustClosed(void);
    static void EvtJustOpened(void);
    void LasersOn (void) { IPin.Clear(); }
    void LasersOff(void) { IPin.Set(); }
public:
    void Init(void) {
        IPin.Init(GPIOA, 11, GPIO_Mode_Out_PP);
        EvtJustClosed();
    }
    uint32_t Timer;
    DoorState_t State;
    void Task(void);
    void Open(void);
    void Close(void);
} Door;

#define SENSOR_THRS     1100
enum LaserHallState_t {lsSleep, lsActive, lsInactive, lsGas, lsAlarm, lsAfterAlarm1, lsAfterAlarm2, lsTryToActivate, lsActivateFailure};
class LaserHall_t {
private:
    uint32_t ITimer, IMotionTimer;
    klPin_t GasLed, MotionPin;
    void LaserPinOn(void) { LaserPin = 1; }
    void LaserOn(void)   { ESnd.Play(SndLaserOn);  ESnd.EvtPlayEnd = 0; LaserPin = 0; }
    void LaserOff(void)  { ESnd.Play(SndLaserOff); ESnd.EvtPlayEnd = LaserHallLaserOff; }
    void GasLedOn(void)  { GasLed = 1; }
    void GasLedOff(void) { GasLed = 0; }
    bool BeamOk(void) { return (ADC_GetConversionValue(ADC2) > SENSOR_THRS); }
    bool MotionDetected(void) { return (MotionPin == 1); }
    void SensorInit(void);
public:
    // Variables
    LaserHallState_t State;
    // Settings
    char SndLaserOn[FNAME_LNG_MAX], SndLaserOff[FNAME_LNG_MAX], SndGas[FNAME_LNG_MAX], SndAlarm[FNAME_LNG_MAX];
    uint32_t ActiveDelay, AlarmDelay, AfterAlarmDelay, ActivateRetryDelay;
    // Methods
    void Init(void);
    void Task(void);
    bool ReadSettings(void);
    void Deactivate(void);
    void Activate(void);
} LaserHall;

//#define ADC_DISPLAY

// ============================ Implementation ================================
int main(void) {
    GeneralInit();

#ifdef ADC_DISPLAY
    uint32_t tmr;
#endif

    // ==== Main cycle ====
    while(1) {
#ifdef ADC_DISPLAY
        if(Delay.Elapsed(&tmr, 198)) {
            //if (ADC_GetFlagStatus(ADC2, ADC_FLAG_AWD)) klPrintf("aga\r");
            klPrintf("ADC: %u\r", ADC_GetConversionValue(ADC2));
        }
#endif
        Keys.Task();
        ESnd.Task();
        Codecheck.Task();
        Door.Task();
        i2cMgr.Task();
        Leds.Task();
        Battery.Task();
        CmdUnit.Task();
        LaserHall.Task();
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
    Keys.EvtKbd = Event_Kbd;
    CmdUnit.Init();

    // Sound
    Vs.Init();
    ESnd.Init();

    // Leds
    i2cMgr.Init();
    Leds.Init();

    // ADC
    Battery.Init();
    Battery.State = bsFull;

    // Register filesystem
    klPrintf("Mount fs\r");
    f_mount(0, &SD.FatFilesystem);
    if (!Settings.Read()) {
        klPrintf("Settings read error\r");
        while(1);    // nothing to do if config not read
    }

    // LaserHall init
    LaserHall.Init();
    // Read settings
    if (!LaserHall.ReadSettings()) {
        klPrintf("Laserhall settings read error\r");
        while(1);    // nothing to do if config not read
    }

    Door.Init();    // Door is closed during Init

    //klPrintf("Lock is on\r");
    CmdUnit.Print2Buf("Lock is on\r");
    // Play initial sound
    ESnd.Play("alive.wav");
}

bool Settings_t::Read(void) {
    klPrintf("Reading settings\r");
    // Sound names
    if(!ReadString("Sound", "KeyBeep",   "lock.ini", Settings.SndKeyBeep,   FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "KeyDrop",   "lock.ini", Settings.SndKeyDrop,   FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "KeyCrash",  "lock.ini", Settings.SndKeyCrash,  FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "PassError", "lock.ini", Settings.SndPassError, FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Open",      "lock.ini", Settings.SndOpen,      FNAME_LNG_MAX)) return false;
    if(!ReadString("Sound", "Close",     "lock.ini", Settings.SndClose,     FNAME_LNG_MAX)) return false;

    // ======= Codes =======
    // If length == 0 then code is empty. If code is negative (-1 for examle) then side is crashed
    // === Code A ===
    if(!ReadString("Code", "CodeA", "lock.ini", Settings.CodeA, CODE_LNG_MAX)) return false;
    Settings.CodeALength = strlen(Settings.CodeA);
    if (Settings.CodeALength != 0) {
        if (Settings.CodeA[0] == 'N') Settings.CodeALength = 0;     // Check if None
        if (Settings.CodeA[0] == '-') Settings.CodeALength = -1;    // Check if crashed
    }

    // === Code B ===
    if(!ReadString("Code", "CodeB", "lock.ini", Settings.CodeB, CODE_LNG_MAX)) return false;
    Settings.CodeBLength = strlen(Settings.CodeB);
    if (Settings.CodeBLength !=0) {
        if (Settings.CodeB[0] == 'N') Settings.CodeBLength = 0;     // Check if None
        if (Settings.CodeB[0] == '-') Settings.CodeBLength = -1;    // Check if crashed
    }

    // === Service code ===
    if(!ReadString("Code", "ServiceCode", "lock.ini", Settings.ServiceCode, CODE_LNG_MAX)) return false;

    // Complexity
    if(!ReadString("Code", "Complexity", "lock.ini", Settings.Complexity, 3)) return false;

    // Colors
    if(!ReadColor("Colors", "DoorOpen", "lock.ini", &Settings.ColorDoorOpen)) return false;
    if(!ReadColor("Colors", "DoorOpening", "lock.ini", &Settings.ColorDoorOpening)) return false;
    if(!ReadColor("Colors", "DoorClosed", "lock.ini", &Settings.ColorDoorClosed)) return false;
    if(!ReadColor("Colors", "DoorClosing", "lock.ini", &Settings.ColorDoorClosing)) return false;
    if(!ReadUint32("Colors", "BlinkDelay", "lock.ini", &Settings.BlinkDelay)) return false;

    // Timings
    if(!ReadUint32("Timings", "DoorCloseDelay", "lock.ini", &Settings.DoorCloseDelay)) return false;
    if(!ReadUint32("Timings", "KeyDropDelay", "lock.ini", &Settings.KeyDropDelay)) return false;

    klPrintf("Settings read ok\r");
    return true;
}


// ========================== Commands handling ================================
void CmdUnit_t::Task() {
    static bool FieldIsOn = false;
    if (CmdState == csReady) {
        // Switch field generator on
        CoilA.Enable();
        CoilB.Enable();
        Delay.Reset(&ITimer);
        FieldIsOn = true;

        // ==== Handle cmd ====
        // Get state
        if (RXBuf[0] == 'S') {    // Get state: "service code","CodeA","CodeB", "Complexity"
            Print2Buf("S:%S,%S,%S,%S\r", Settings.ServiceCode, Settings.CodeA, Settings.CodeB, Settings.Complexity);
        }

        // Kick
        if (RXBuf[0] == 'K') Print2Buf("K\r");

        // Open door
        else if (RXBuf[0] == 'O') {
            if (Door.State == dsClosed) Door.Open();
            Print2Buf("O\r");
        }
        // Close door
        else if (RXBuf[0] == 'C') {
            if (Door.State == dsOpened) Door.Close();
            Print2Buf("C\r");
        }

        // Replace codeA
        else if (RXBuf[0] == 'A') {
            if (RXBuf[1] == ':') {
                if ((RXBuf[2] == 0) || (RXBuf[2] == 'N')) { // Check if empty or None
                    Settings.CodeA[0] = 0;
                    Settings.CodeALength = 0;
                }
                else {
                    strncpy(Settings.CodeA, (char*)&RXBuf[2], 4);
                    if (RXBuf[2] == '-') Settings.CodeALength = -1;
                    else Settings.CodeALength = strlen(Settings.CodeA);
                }
                Print2Buf("A\r");
            }
        }

        // Replace codeB
        else if (RXBuf[0] == 'B') {
            if (RXBuf[1] == ':') {
                if ((RXBuf[2] == 0) || (RXBuf[2] == 'N')) { // Check if empty or None
                    Settings.CodeB[0] = 0;
                    Settings.CodeBLength = 0;
                }
                else {
                    strncpy(Settings.CodeB, (char*)&RXBuf[2], 4);
                    if (RXBuf[2] == '-') Settings.CodeBLength = -1;
                    else Settings.CodeBLength = strlen(Settings.CodeB);
                }
                Print2Buf("B\r");
            }
        }

        // Replace service code
        else if (RXBuf[0] == 'V') {
            if (RXBuf[1] == ':') {
                strncpy(Settings.ServiceCode, (char*)&RXBuf[2], 6);
                Print2Buf("V\r");
            }
        }
        CmdReset();
    } // if ready

    // Check if shutdown field generator
    if (FieldIsOn) if (Delay.Elapsed(&ITimer, FIELD_GENERATOR_TIMEOUT)) {
        CoilA.Disable();
        CoilB.Disable();
        FieldIsOn = false;
    }
}

// ============================ Codecheck ======================================
void Codecheck_t::Task(void) {
    if (ESnd.State == sndPlaying) return;
    switch (Codecheck.EnterResult) {
        case entNA: // check if input timeout
            if((EnteredLength != 0) && Delay.Elapsed(&Timer, Settings.KeyDropDelay)) {
                Reset();
                ESnd.Play(Settings.SndKeyDrop);
            }
            break;
        case entError:
            Reset();
            ESnd.Play(Settings.SndPassError);
            break;
        case entOpen:
            Reset();
            Door.Open();
            break;
    } // switch
}

// =============================== LaserHall ===================================
void LaserHall_t::Task() {
    if (MotionDetected()) Delay.Reset(&IMotionTimer);

    switch (State) {
        case lsSleep:   // Check if motion detected
            if (Delay.Elapsed(&ITimer, 252)) {
                if (MotionDetected()) {
                    LaserOn();
                    State = lsTryToActivate;
                }
            }
            break;

        case lsActive:
            // Check if timeout
            if (Delay.Elapsed(&ITimer, ActiveDelay)) {
                LaserOff();
                State = lsSleep;
            }
            // Check if beam ok
            if (!BeamOk()) {
                LaserOff();
                State = lsGas;
            }
            // Check if something is still moving
            if (MotionDetected()) Delay.Reset(&ITimer);
            break;

        case lsInactive: break; // Nothing to do in this mode

        case lsGas: // Play Gas when allowed
            if (ESnd.State != sndPlaying) {
                ESnd.Play(SndGas);
                Delay.Reset(&ITimer);
                State = lsAlarm;
            }
            break;

        case lsAlarm:
            if (ESnd.State != sndPlaying) {
                // Check if time to off
                if (Delay.Elapsed(&ITimer, AlarmDelay)) State = lsAfterAlarm1;
                else {
                    ESnd.Play(SndAlarm);
                    GasLedOn();
                }
            }
            break;

        case lsAfterAlarm1: // Wait for sound to complete
            if (ESnd.State != sndPlaying) {
                GasLedOff();
                Delay.Reset(&ITimer);
                State = lsAfterAlarm2;
            }
            break;

        case lsAfterAlarm2: // Check if time to activate
            if (Delay.Elapsed(&ITimer, AfterAlarmDelay)) {
                LaserOn();
                State = lsTryToActivate;
            }
            break;

        case lsTryToActivate: // Wait for sound to complete
            if (ESnd.State != sndPlaying) {
                Delay.Reset(&ITimer);
                if (BeamOk()) State = lsActive;
                else {
                    LaserOff();
                    // Shutdown system in case of no motion
                    if (Delay.Elapsed(&IMotionTimer, ActiveDelay)) State = lsSleep;
                    else State = lsActivateFailure;
                }
            }
            break;

        case lsActivateFailure: // Wait for beam path to restore
            if (Delay.Elapsed(&ITimer, ActivateRetryDelay)) {
                LaserOn();
                State = lsTryToActivate;
            }
            break;
    } // switch
}

void LaserHall_t::Init() {
    klPrintf("LaserHall init\r");
    State = lsSleep;
    // Pins
    GasLed.Init  (GPIOA, 8,  GPIO_Mode_Out_PP);
    LaserPin.Init(GPIOA, 10, GPIO_Mode_Out_OD);
    MotionPin.Init(GPIOC, 3, GPIO_Mode_IPD);
    GasLed = 0;
    LaserPin = 0;
    SensorInit();
}

bool LaserHall_t::ReadSettings() {
    klPrintf("Reading LaserHall settings\r");
    // Sound names
    if(!ReadString("LaserHall", "LaserOn",  "lock.ini", SndLaserOn,  FNAME_LNG_MAX)) return false;
    if(!ReadString("LaserHall", "LaserOff", "lock.ini", SndLaserOff, FNAME_LNG_MAX)) return false;
    if(!ReadString("LaserHall", "Gas",      "lock.ini", SndGas,      FNAME_LNG_MAX)) return false;
    if(!ReadString("LaserHall", "Alarm",    "lock.ini", SndAlarm,    FNAME_LNG_MAX)) return false;

    // Timings
    if(!ReadUint32("LaserHall", "ActiveDelay",        "lock.ini", &ActiveDelay))        return false;
    if(!ReadUint32("LaserHall", "AlarmDelay",         "lock.ini", &AlarmDelay))         return false;
    if(!ReadUint32("LaserHall", "AfterAlarmDelay",    "lock.ini", &AfterAlarmDelay))    return false;
    if(!ReadUint32("LaserHall", "ActivateRetryDelay", "lock.ini", &ActivateRetryDelay)) return false;

    klPrintf("LaserHall settings read ok\r");
    return true;
}

void LaserHall_t::Activate() {
    LaserOn();
    State = lsTryToActivate;
    Delay.Reset(&ITimer);
}

void LaserHall_t::Deactivate() {
    LaserOff();
    State = lsInactive;
}

// ================================== Door =====================================
void Door_t::Open(void) {
    Door.State = dsOpening;
    ESnd.Play(Settings.SndOpen);
    ESnd.EvtPlayEnd = EvtJustOpened;
    Delay.Reset(&Timer);
    Leds.Led[0].Blink(Settings.ColorDoorOpening, Settings.BlinkDelay);
    Leds.Led[2].Blink(Settings.ColorDoorOpening, Settings.BlinkDelay);
}

void Door_t::Close(void) {
    Door.State = dsClosing;
    ESnd.Play(Settings.SndClose);
    ESnd.EvtPlayEnd = EvtJustClosed;
    Leds.Led[0].Blink(Settings.ColorDoorClosing, Settings.BlinkDelay);
    Leds.Led[2].Blink(Settings.ColorDoorClosing, Settings.BlinkDelay);
}

void Door_t::Task() {
    if (State == dsOpened) {
        if(Delay.Elapsed(&Timer, Settings.DoorCloseDelay))
            Close();
    }
}

void Door_t::EvtJustClosed(void) {
    ESnd.EvtPlayEnd = 0;
    Door.State = dsClosed;
    Leds.Led[0].Solid(Settings.ColorDoorClosed);
    Leds.Led[2].Solid(Settings.ColorDoorClosed);
    Door.LasersOn();
    LaserHall.Activate();
    klPrintf("Door is closed\r");
}
void Door_t::EvtJustOpened(void) {
    ESnd.EvtPlayEnd = 0;
    Door.State = dsOpened;
    Leds.Led[0].Solid(Settings.ColorDoorOpen);
    Leds.Led[2].Solid(Settings.ColorDoorOpen);
    Door.LasersOff();
    LaserHall.Deactivate();
    klPrintf("Door is opened\r");
}

// ==================================== Adc ====================================
void LaserHall_t::SensorInit() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    // ==== GPIO ====
    //klGpio::SetupByN(GPIOC, 1, GPIO_Mode_AIN);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // ==== ADC ====
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // Independent, not dual
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC2, &ADC_InitStructure);
    // ADC1 regular channels configuration
    ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_71Cycles5);

    ADC_Cmd(ADC2, ENABLE);                      // Enable ADC
    // Calibrate ADC
    ADC_ResetCalibration(ADC2);                 // Enable ADC reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC2)); // Check the end of ADC1 reset calibration register
    ADC_StartCalibration(ADC2);                 // Start ADC calibration
    while(ADC_GetCalibrationStatus(ADC2));      // Check the end of ADC1 calibration
    // Start ADC Software Conversion
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}

void Battery_t::Task() {
    if(!Delay.Elapsed(&Timer, 999)) return;
    Measure();
    // Discharging slope
    if ((State == bsFull) && (IValue < BATTERY_HALF)) {
        State = bsHalf;
        return;
    }
    if ((State == bsHalf) && (IValue < BATTERY_EMPTY)) {
        State = bsEmpty;
        return;
    }
    // Charging slope
    if ((State == bsHalf) && (IValue > (BATTERY_HALF+60))) {
        State = bsFull;
        return;
    }
    if ((State == bsEmpty) && (IValue > (BATTERY_EMPTY+60))) {
        State = bsHalf;
        return;
    }
    // Indicate discharged battery
    if ((State == bsEmpty) && (Door.State == dsClosed) && (Leds.Led[0].Mode != lmBlink)) {
        Leds.Led[0].Blink(Settings.ColorDoorClosed, Settings.BlinkDelay);
        Leds.Led[2].Blink(Settings.ColorDoorClosed, Settings.BlinkDelay);
    }
}


// ================================== Keyboard =================================
void KeyHandler(uint8_t RKey, int8_t RCodeLength, char *RCode) {
    // Do not react if LaserHall does not allow this
    if((LaserHall.State == lsGas) || (LaserHall.State == lsAlarm)) return;
    // Do not react if door is opening or closing
    if ((Door.State == dsOpening) || (Door.State == dsClosing)) return;
    // Check if crash
    if (RCodeLength < 0) {
        if (ESnd.State == sndStopped) ESnd.Play(Settings.SndKeyCrash);
        return;
    }
    // Digit entered
    if((RKey >= 0) && (RKey <=9)) {
        ESnd.Play(Settings.SndKeyBeep);     // Play sound always
        if (RCodeLength > 0) {
            if(Codecheck.EnteredLength < RCodeLength) {        // Digit entered is number 0...3
                Codecheck.EnteredCode[Codecheck.EnteredLength++] = '0' + RKey;
                Delay.Reset(&Codecheck.Timer);
            }
        }// if (RCodeLength > 0)
    }
    else {  // none-digit
        if (RKey == KEY_STAR) {     // Drop code
            Codecheck.Reset();
            ESnd.Play(Settings.SndKeyDrop);
        }
        else if(RKey == KEY_HASH) {
            // Close door if open
            if (Door.State == dsOpened) { Door.Close(); }
            else if (Door.State == dsClosed) {
                if (RCodeLength == 0) Codecheck.EnterResult = entOpen;   // Open door in case of empty code
                else {  // Check code
                    if((Codecheck.EnteredLength == RCodeLength) && (strcmp(RCode, Codecheck.EnteredCode) == 0)) Codecheck.EnterResult = entOpen;  // Equal
                    else Codecheck.EnterResult = entError;
                }
            } // if open
        } // if hash
    } // if digit
}

void Event_Kbd(uint8_t AKbdSide, uint8_t AKey) {
    //klPrintf("Key = %u %u\r", Keys.Kbd[0], Keys.Kbd[1]);
    if (AKbdSide == KBD_SIDE_A) KeyHandler(AKey, Settings.CodeALength, Settings.CodeA);
    else KeyHandler(AKey, Settings.CodeBLength, Settings.CodeB);
}
