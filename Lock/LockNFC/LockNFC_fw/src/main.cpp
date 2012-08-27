/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "pn.h"
#include "IDStore.h"
#include "led.h"
#include "keys.h"
#include "adc.h"

#define STATE_TIMEOUT       27000   // ms; switch to waiting state
#define DOOR_CLOSE_TIMEOUT  9999    // ms
// Colors in RGB
#define DOOR_CLOSED_COLOR   {4, 0, 0}
#define DOOR_OPEN_COLOR     {45, 0, 45}
#define DOOR_CLOSED_BLINK   {45, 0, 0}
#define DOOR_OPEN_BLINK     {135, 0, 135}



// Types
typedef enum {sWaiting, sAdding, sRemoving} State_t;

struct Door_t {
    uint32_t Timer;
    bool IsOpen;
    void Task(void);
};

Door_t Door;
State_t State;

uint32_t StateTimer;

// Prototypes
void GeneralInit(void);
// Events
void Event_CardAppeared(void);
void Event_KeyAdd(void);
void Event_KeyRemove(void);
void Event_KeyBoth(void);

// ============================ Implementation ================================
int main(void) {
    UART_Init();
    klPrintf(" === Lock is here ===\r");
    GeneralInit();

    uint32_t LedTmr;
    // ==== Main cycle ====
    while(1) {
        PN.Task();
        LedGreen.Task();
        LedRed.Task();
        Crystal.Task();
        Keys.Task();
        Battery.Task();
        Door.Task();

        // Handle battery discharge indication
        if (State == sWaiting) {
            if(Battery.IsDischarged()) {
                if (Delay.Elapsed(&LedTmr, 108)) LedRed.Toggle();
            }
            else LedRed.Disable();
        }

        // Handle state
        if (State != sWaiting) {
            if (Delay.Elapsed(&StateTimer, STATE_TIMEOUT)) {
                if (IDStore.IsChanged) IDStore.Save();
                State = sWaiting;
                LedRed.Disable();
                LedGreen.Disable();
            }
        } // if not waiting
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
    // Leds
    LedGreen.Init(GPIOB, GPIO_Pin_11);
    LedRed.Init(GPIOB, GPIO_Pin_10);
    Crystal.Init();
    Crystal.On();
    Crystal.SetColorSmoothly(DOOR_CLOSED_COLOR);

    Door.IsOpen = false;

    Keys.Init();
    Keys.EvtKey1Press = Event_KeyAdd;
    Keys.EvtKey2Press = Event_KeyRemove;
    Keys.EvtKeyPressBoth = Event_KeyBoth;

    PN.Init();
    PN.Evt_CardAppeared = Event_CardAppeared;
    // ID store
    IDStore.Load();

    Battery.Init();

    State = sWaiting;
}

// ================================== Door =====================================
void Door_t::Task() {
    if (IsOpen) if(Delay.Elapsed(&Timer, DOOR_CLOSE_TIMEOUT)) {
        IsOpen = false;
        klPrintf("Door is closed\r");
        Crystal.SetColorSmoothly(DOOR_CLOSED_COLOR);
    }
}


// ================================== Events ===================================
void Event_CardAppeared(void) {
    //klPrintf("NewCard\r");
    if (!Card.ReadID()) return;
    switch (State) {
        case sWaiting:
            if (!Door.IsOpen) {
                if (IDStore.IsPresent(Card.ID)) {
                    Door.IsOpen = true;
                    Crystal.SetColorSmoothly(DOOR_OPEN_COLOR);
                    Delay.Reset(&Door.Timer);
                    klPrintf("Door is open\r");
                }
                else {  // Blink to inform detection
                    Crystal.SetColor(DOOR_CLOSED_BLINK);
                }
            }
            break;
        case sAdding:
            LedGreen.Blink();
            IDStore.Add(Card.ID);
            Delay.Reset(&StateTimer);   // Reset state timeout
            break;
        case sRemoving:
            LedRed.Blink();
            IDStore.Remove(Card.ID);
            Delay.Reset(&StateTimer);   // Reset state timeout
            break;
    } // switch
}

// ==== Keypress events ====
void Event_KeyAdd(void) {
    if (State == sAdding) {
        State = sWaiting;
        if (IDStore.IsChanged) IDStore.Save();
        LedGreen.Disable();
    }
    else {
        State = sAdding;
        LedGreen.On();
        LedRed.Disable();
        Delay.Reset(&StateTimer);   // Reset state timeout
    }
}
void Event_KeyRemove(void) {
    if (State == sRemoving) {
        State = sWaiting;
        if (IDStore.IsChanged) IDStore.Save();
        LedRed.Disable();
    }
    else {
        State = sRemoving;
        LedRed.On();
        LedGreen.Disable();
        Delay.Reset(&StateTimer);   // Reset state timeout
    }
}
void Event_KeyBoth(void) {
    // Inform action
    LedGreen.Disable();
    LedRed.Disable();
    State = sWaiting;
    for(uint32_t i=0; i<4; i++) {
        LedGreen.On();
        Delay.ms(99);
        LedGreen.Off();
        Delay.ms(99);
    }
    // Clear memory
    IDStore.EraseAll();
}
