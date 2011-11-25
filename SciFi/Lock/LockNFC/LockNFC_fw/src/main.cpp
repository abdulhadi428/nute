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

// Types
typedef enum {sWaiting, sAdding, sRemoving} State_t;

State_t State;

// Prototypes
void GeneralInit(void);
void Event_CardAppeared(void);
void DoorToggle(void);

// ============================ Implementation ================================
int main(void) {
    UART_Init();
    klPrintf("==== Lock is here ===\r");

    GeneralInit();
    // ==== Main cycle ====
    while(1) {
        PN.Task();
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
    PN.Init();
    PN.Evt_CardAppeared = Event_CardAppeared;
    // ID store
    IDStore.Load();
}

// ================================== Door =====================================
void DoorToggle(void) {

}


// ================================== Events ===================================
void Event_CardAppeared(void) {
    //klPrintf("NewCard\r");
    if (!Card.ReadID()) return;
    switch (State) {
        case sWaiting:  if (IDStore.IsPresent(Card.ID)) DoorToggle(); break;
        case sAdding:   IDStore.Add(Card.ID); break;
        case sRemoving: IDStore.Remove(Card.ID); break;
    } // switch
}
