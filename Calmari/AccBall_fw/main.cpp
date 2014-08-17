/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Acc Ball
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led.h"
#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "cmd_uart_f10x.h"
#include "acc_mma8452.h"
#include "evt_mask.h"

#define TIMEOUT_TO_FADE_MS  5400

i2c_t i2c;
Thread *PThd;
static VirtualTimer ITmr;
static void TmrCallback(void *p);

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv2, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    JtagDisable();
    Uart.Init(115200);
    i2c.Init(I2C1, 100000);
    Acc.Init();
    LedSmooth.Init();
    LedSmooth.SetSmoothly(0);
    PThd = chThdSelf();

    Uart.Printf("\rAccBall  AHB=%u; APB1=%u; APB2=%u\r\n", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);

    // ==== Main loop ====
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(EVTMSK_ACC_IRQ | EVTMSK_TIMEOUT);
        if(EvtMsk & EVTMSK_ACC_IRQ) {
            // Irq occured
            Uart.Printf("Evt\r");
            LedSmooth.SetSmoothly(LED_TOP_VALUE);
            // (Re)Start timeout timer
            chVTReset(&ITmr);
            chVTSet(&ITmr, MS2ST(TIMEOUT_TO_FADE_MS), TmrCallback, NULL);
        }
        else if(EvtMsk & EVTMSK_TIMEOUT) {
            // Timeout occured
            Uart.Printf("Timeout\r");
            LedSmooth.SetSmoothly(LED_BOTTOM_VALUE);
        }
    }
}

// Timer callback
static void TmrCallback(void *p) {
    chEvtSignalI(PThd, EVTMSK_TIMEOUT);
}
