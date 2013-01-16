/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f2xx.h"
#include "lcd2630.h"
#include "application.h"

//static WORKING_AREA(waThread1, 128);
//static msg_t Thread1(void *arg) {
//
//  (void)arg;
//  chRegSetThreadName("blinker");
//  while (TRUE) {
////    PinSet(GPIOB, 9);
////    chThdSleepMilliseconds(100);
////    PinClear(GPIOB, 9);
////    chThdSleepMilliseconds(1800);
//  }
//  return 0;
//}

static inline void Init();

// Application entry point.
int main(void) {
    halInit();
    chSysInit();
    Init();

    //uint32_t i=0;
    while(TRUE) {
        chThdSleepMilliseconds(999);
        //Uart.Printf("%u %u %u\r", i, i, i);
        //i++;
    }
}

void Init() {
//    PinSetupOut(GPIOB, 9, omPushPull);
//    PinSet(GPIOB, 9);
//    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

    Uart.Init(115200);
    Uart.Printf("Armlet2\r");
    Lcd.Init();
    // Application init
    App.Init();
}
