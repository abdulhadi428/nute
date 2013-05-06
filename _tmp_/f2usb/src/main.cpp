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
#include "kl_sd.h"

#include "application.h"

#include "shell.h"
#include "usb.h"
#include "usbcfg.h"
#include "chprintf.h"

#include "stdarg.h"

static inline void Init();

static SerialUSBDriver SDU1;

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WA_SIZE(2048)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {THD_STATE_NAMES};
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)7,
            states[tp->p_state], (uint32_t)tp->p_time);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
 // {"test", cmd_test},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

// Application entry point.
int main() {
    // ==== Setup clock ====
    uint8_t ClkResult = 1;
    Clk.SetupFlashLatency(16);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/6 = 64 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv6, 8);
    // 64/4 = 16 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    //shellCreateStatic(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    while(TRUE) {
//        if (!shelltp) {
//          if (SDU1.config->usbp->state == USB_ACTIVE) {
//            /* Spawns a new shell.*/
//            shelltp =
//          }
//        }
//        else {
//          /* If the previous shell exited.*/
//          if (chThdTerminated(shelltp)) {
//            /* Recovers memory of the previous shell.*/
//            //chThdRelease(shelltp);
//            shelltp = NULL;
//          }
//        }
        chThdSleepMilliseconds(500);
    }
}

void Init() {
    Uart.Init(115200);
    Uart.Printf("usb AHB=%u; APB1=%u; APB2=%u; UsbSdio=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz, Clk.UsbSdioFreqHz);
    // Application init
    AppInit();
}

extern "C" {
void dbgprn(const char* S, ...) {
    va_list args;
    va_start(args, S);
    Uart.Printf(S, args);
    va_end(args);
}

}
