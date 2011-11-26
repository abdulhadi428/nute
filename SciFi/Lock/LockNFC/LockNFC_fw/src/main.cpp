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

#define STATE_TIMEOUT   7000    // ms

// Types
typedef enum {sWaiting, sAdding, sRemoving} State_t;

State_t State;
bool DoorIsOpen = false;
uint32_t StateTimer;

// Prototypes
void GeneralInit(void);
void DoorToggle(void);
// Events
void Event_CardAppeared(void);
void Event_KeyAdd(void);
void Event_KeyRemove(void);

// ============================ Implementation ================================
int main(void) {
    UART_Init();
    klPrintf(" === Lock is here ===\r");
    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        PN.Task();
        LedGreen.Task();
        LedRed.Task();
        Keys.Task();
        // Handle state
        if (State != sWaiting) {
            if (Delay.Elapsed(&StateTimer, STATE_TIMEOUT)) {
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

    LedGreen.Init(GPIOB, GPIO_Pin_11);
    LedRed.Init(GPIOB, GPIO_Pin_10);

    Keys.Init();
    Keys.EvtKeyPress[0] = Event_KeyAdd;
    Keys.EvtKeyPress[1] = Event_KeyRemove;

    PN.Init();
    PN.Evt_CardAppeared = Event_CardAppeared;
    // ID store
    IDStore.Load();

    State = sWaiting;
}

// ================================== Door =====================================
void DoorToggle(void) {
    DoorIsOpen = !DoorIsOpen;
    if (DoorIsOpen) {
        klPrintf("Door is open\r");
    }
    else {
        klPrintf("Door is closed\r");
    }
}


// ================================== Events ===================================
void Event_CardAppeared(void) {
    //klPrintf("NewCard\r");
    if (!Card.ReadID()) return;
    switch (State) {
        case sWaiting:
            if (IDStore.IsPresent(Card.ID)) DoorToggle();
            else klPrintf("Key does not fit\r");
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

void Event_KeyAdd(void) {
    if (State == sAdding) {
        State = sWaiting;
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
        LedRed.Disable();
    }
    else {
        State = sRemoving;
        LedRed.On();
        LedGreen.Disable();
        Delay.Reset(&StateTimer);   // Reset state timeout
    }
}

