/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"
#include "nute.h"
#include "gps.h"
#include "collar.h"
#include "areas.h"
#include "beep.h"
#include "adc.h"

LedBlinkInverted_t Led;

class KeyCon_t {
private:
    bool IKeyWasPressed;
    bool IKeyIsPressed(void) { return klGpioIsClearByN(GPIOB, 15); }
    bool IConIsOpen(void) { return klGpioIsSetByN(GPIOB, 2); }
public:
    bool CollarIsBroken;
    void Init(void);
    void Task(void);
} KeyCon;

// Prototypes
void GeneralInit(void);
void CollarStateHandler(void);
uint32_t GetID(void);
// State changes
void EnterState(uint8_t ANewState);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    uint32_t Tmr;
    while (1) {
        CC.Task();
        Nute.Task();
        Gps.Task();
        CmdUnit.Task();     // Uart
        CollarStateHandler();
        Led.Task();
        KeyCon.Task();
        // Store battery value
        if(Delay.Elapsed(&Tmr, 999)) {
            Situation->Battery = 1.61 * Adc.GetValue();
            //CmdUnit.Printf("Battery: %u\r", Situation->Battery);
            //CmdUnit.Printf("State: %X\r", Situation->State);
        }

    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);
    CmdUnit.Init();

    Gps.Init();

    Led.Init(GPIOB, 1);
    Led.Blink(999);

    Nute.Init(GetID());
    Situation = &Nute.TX_Pkt.Situation;
    AreaList.Init();
    Beep.Init();
    Beep.Freq = 1600;

    EnterState(COLSTATE_OK);
    //EnterState(COLSTATE_CMD_DELAY);

    Adc.Init();
    KeyCon.Init();

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(Nute.TX_Pkt.AddrFrom); // Set in Nute.init

    CmdUnit.Printf("\rCollar %u\r", Nute.TX_Pkt.AddrFrom);
}

uint32_t GetID(void) {
    klGpioSetupByMsk(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, GPIO_Mode_IPU);
    Delay.ms(18);
    uint32_t ID = 0;
    if (klGpioIsClearByN(GPIOB, 3))  ID += 0x01;
    if (klGpioIsClearByN(GPIOB, 4))  ID += 0x02;
    if (klGpioIsClearByN(GPIOB, 10)) ID += 0x04;
    if (klGpioIsClearByN(GPIOB, 11)) ID += 0x08;
    if (klGpioIsClearByN(GPIOB, 12)) ID += 0x10;
    if (klGpioIsClearByN(GPIOB, 13)) ID += 0x20;
    if (klGpioIsClearByN(GPIOB, 14)) ID += 0x40;
    klGpioSetupByMsk(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, GPIO_Mode_AIN);  // Disable inputs
    return (ID + TIXE_ADDR_OFFSET);
}

// ============================= Key & connector ===============================
void KeyCon_t::Init() {
    klGpioSetupByMsk(GPIOB, GPIO_Pin_2 | GPIO_Pin_15, GPIO_Mode_IPU);
    IKeyWasPressed = false;
    CollarIsBroken = false;
    Situation->State &= ~0x30;  // Reset state
}


void KeyCon_t::Task(void) {
    // Connector
    if (IConIsOpen() and !CollarIsBroken) {
        CollarIsBroken = true;
        Situation->State |= 0x20;
    }

    // Key
    if (CollarState == COLSTATE_OK) {
        static uint32_t FTmr, ICounter = 0;
        if (IKeyIsPressed() and !IKeyWasPressed) {
            IKeyWasPressed = true;
            ICounter = 0;
            Delay.Reset(&FTmr);
        }
        else if (!IKeyIsPressed() and IKeyWasPressed) {
            IKeyWasPressed = false;
            Beep.Off();
        }
        else if (IKeyIsPressed() and IKeyWasPressed) {
            if (Delay.Elapsed(&FTmr, 999)) {
                if (ICounter < 5) {
                    Beep.Volume = 4;
                    Beep.On();
                }
                else {
                    Beep.Off();
                    Led.On();
                    CollarState = COLSTATE_BOOM;
                    Situation->State |= 0x10;
                }
                ICounter++;
            }
        }
    } // if ok
}

// =================================== States ==================================
uint8_t CollarState;
uint8_t CollarStateCmd;
void CollarStateHandler(void) {
    // Check areas
    if (Gps.NewMeasurement) {
        Gps.NewMeasurement  = false;
        uint8_t CurrentAreaIndx;
        bool IsInRestrictedArea = true;
        if (AreaList.CoordToAreaIndx(Situation->Latitude, Situation->Longitude, &CurrentAreaIndx)) {  // if we somewhere in areas
            IsInRestrictedArea = AreaList.IsAreaRestricted(CurrentAreaIndx);
        }
        // Enter Delay if in restricted area
        if (IsInRestrictedArea and (CollarState == COLSTATE_OK)) EnterState(COLSTATE_DELAY_BY_AREA);
        // Enter Ok if moved out of restricted area, but do not if delay was by Cmd
        else if (!IsInRestrictedArea and (CollarState == COLSTATE_DELAY_BY_AREA)) EnterState(COLSTATE_OK);
    }

    // Check collar
    if (KeyCon.CollarIsBroken and ((CollarState == COLSTATE_OK) or (CollarState == COLSTATE_DELAY_BY_AREA) or (CollarState == COLSTATE_DELAY_BY_CMD))) EnterState(COLSTATE_BOOM);

    // Check radio cmd
    if(CollarStateCmd == COLSTATE_CMD_BOOM) {
        if(CollarState != COLSTATE_BOOM) EnterState(COLSTATE_BOOM);
    }
    else if(CollarStateCmd == COLSTATE_CMD_DELAY) {
        if((CollarState == COLSTATE_OK) or (CollarState == COLSTATE_DELAY_BY_AREA)) EnterState(COLSTATE_DELAY_BY_CMD);
    }
    else if(CollarStateCmd == COLSTATE_CMD_RESET) EnterState(COLSTATE_OK);

    // Maintain state
    if (CollarState == COLSTATE_BOOM) {
        if (Beep.IsOn) {
            if(Delay.Elapsed(&Beep.Timer, BEEP_BOOM_LEN)) {
                Led.Off();
                Beep.Off();
            }
        }
    }
    else if ((CollarState == COLSTATE_DELAY_BY_AREA) or (CollarState == COLSTATE_DELAY_BY_CMD)) {
        if (Beep.IsOn) {
            if(Delay.Elapsed(&Beep.Timer, BEEP_SND_LEN)) {
                Led.Off();
                Beep.Off();
                Beep.SilenceLen = BeepLenArr[Beep.N];
                Beep.N++;
            }
        }
        else {
            if(Beep.N == BEEP_ARR_SZ) { // Delay before boom
                if(Delay.Elapsed(&Beep.Timer, 450)) EnterState(COLSTATE_BOOM);
            }
            else if(Delay.Elapsed(&Beep.Timer, Beep.SilenceLen)) {
                Led.On();
                Beep.On();
            }
        } // if on
    } // if state
}

// Called only by state change. Switches state, does not maintain it.
void EnterState(uint8_t ANewState) {
    CmdUnit.Printf("New state: %u\r", ANewState);
    KeyCon.Init();
    CollarState = ANewState;
    switch (CollarState) {
        case COLSTATE_OK:
            Led.Disable();
            Beep.Off();
            Led.Off();
            Situation->State &= ~0x0F;
            if (CollarStateCmd == COLSTATE_CMD_RESET) Situation->State |= 0x03;
            break;

        case COLSTATE_BOOM:
            Delay.Reset(&Beep.Timer);
            Beep.Volume = 100;
            Beep.N = 0;
            Beep.On();
            Led.On();
            Situation->State &= ~0x0F;
            Situation->State |= 0x01;
            break;

        case COLSTATE_DELAY_BY_AREA:
        case COLSTATE_DELAY_BY_CMD:
            Delay.Reset(&Beep.Timer);
            Beep.Volume = 4;
            Beep.N = 0;
            Beep.On();
            Led.On();
            Situation->State &= ~0x0F;
            Situation->State |= 0x02;
            break;

        default: break;
    }
}

