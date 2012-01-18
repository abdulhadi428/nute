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
#include <string.h>
#include <stdlib.h>
#include "kl_util.h"

// Prototypes
void GeneralInit(void);

// ============================ Implementation ================================
int main(void) {
    UART_Init();
    Delay.ms(100);
    klPrintf("Player is here\r");

    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        ESnd.Task();
        if(ESnd.State == sndStopped) ESnd.Play("sound.wav");
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
    // Sound
    Vs.Init();
    ESnd.Init();

    // Register filesystem
    f_mount(0, &SD.FatFilesystem);

    // Play initial sound
    ESnd.Play("alive.wav");
}
