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
//#include "lcd2630.h"

static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    PinSet(GPIOB, 9);
    chThdSleepMilliseconds(100);
    PinClear(GPIOB, 9);
    chThdSleepMilliseconds(1800);
  }
  return 0;
}

static inline void Init();

/*
 * Application entry point.
 */
int main(void) {
    /*
    * System initializations.
    * - HAL initialization, this also initializes the configured device drivers
    *   and performs the board-specific initializations.
    * - Kernel initialization, the main() function becomes a thread and the
    *   RTOS is active.
    */
    halInit();
    chSysInit();

    Init();

    while(TRUE) {
        //    x = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTX);
        //    y = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTY);
        //    z = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTZ);
        //chprintf((BaseChannel *)&SD2, "%d, %d, %d\r\n", x, y, z);
        chThdSleepMilliseconds(500);
    }
}

void Init() {
    PinSetupOut(GPIOB, 9, omPushPull);
    PinSet(GPIOB, 9);
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    Uart.Init(115200);
    Uart.Printf("Armlet2\r");
    Uart.Printf("2 3 4\r");
}
