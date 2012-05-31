/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_util.h"
#include "kl_lib.h"
#include "pn.h"
#include "led.h"

// Prototypes
void GeneralInit(void);
// Events
void Event_CardAppeared(void);
void Event_CardDisappeared(void);

// ============================ Implementation ================================
int main(void) {
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
    Delay.ms(1800);
    UART_Init();
    klPrintf("\rID Reader\r\n");

    PN.Init();
    PN.Evt_CardAppeared = Event_CardAppeared;
    PN.Evt_CardDisappeared = Event_CardDisappeared;
}

// ================================== Events ===================================
void Event_CardAppeared(void) {
    if (Card.ReadID()) {
        klPrintf("ID: %X%X\r\n", (uint32_t)((Card.ID >> 32)& 0xFFFFFFFF), (uint32_t)(Card.ID & 0xFFFFFFFF));
    }
}
void Event_CardDisappeared(void) {
    klPrintf("No card\r\n");
}
