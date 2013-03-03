/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "acc.h"
#include "ManyLed.h"

#include "VibroSequences.h"

App_t App;

// =============================== App Thread ==================================
static WORKING_AREA(waAppThread, 1024);
static msg_t AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    while(1) {
        chThdSleepMilliseconds(99);
        if(!App.Enabled) continue;  // Do not do anything

        GreenLed.On(LED_TOP_BRT);
        for(uint8_t i=0; i<ACC_CNT; i++) {
            if(Acc[i].IsOperational) {
                Acc[i].ReadAccelerations();
                Uart.Printf("%0u %d %d %d; ", i, Acc[i].a[0], Acc[i].a[1], Acc[i].a[2]);
            }
            //else Acc[i].Init();
        }
        Uart.Printf("\r\n");
        GreenLed.Off();
    } // if enabled
    return 0;
}


// =============================== App class ===================================
void App_t::Init() {
    Enabled = true;
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, AppThread, NULL);
}
