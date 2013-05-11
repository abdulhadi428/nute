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

#include "kl_usb.h"

static inline void Init();

//static SerialUSBDriver SDU1;


// Application entry point.
int main() {
    // ==== Setup clock ====
    uint8_t ClkResult = 1;
    Clk.SetupFlashLatency(24);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/2 = 24 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv2, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r\n");

//    sduObjectInit(&SDU1);
//    sduStart(&SDU1, &serusbcfg);
//
//    usbDisconnectBus(serusbcfg.usbp);
//    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
//    usbConnectBus(serusbcfg.usbp);

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
    Uart.Printf("usb AHB=%u; APB1=%u; APB2=%u; UsbSdio=%u\r\n", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz, Clk.UsbSdioFreqHz);
    Usb.Init();
    Usb.Disconnect();
    chThdSleepMilliseconds(999);
    Usb.Connect();
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

void dbgprn2(const char* S, uint32_t N) {
    Uart.Printf(S, N);
}

}
