/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "led.h"
#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "acc_mma8452.h"

// Event masks
#define EVTMASK_ACC_IRQ     EVENT_MASK(0)
#define EVTMASK_TIMEOUT     EVENT_MASK(1)

#define TIMEOUT_TO_FADE_MS  5400

i2c_t i2c;
static EventListener EvtLstnrAccIrq, EvtLstnrTimeout;
static EventSource IEvtSrcTimeout;
static VirtualTimer ITmr;

static inline void Init();
static void TmrCallback(void *p);

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);    // For I2C, minimum ClockFreq=2MHz
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    PinSetupOut(GPIOA, 1, omPushPull);

    // Events
    uint32_t EvtMsk;
    chEvtInit(&IEvtSrcTimeout);
    Acc.RegisterEvtIrq(&EvtLstnrAccIrq, EVTMASK_ACC_IRQ);
    chEvtRegisterMask(&IEvtSrcTimeout, &EvtLstnrTimeout, EVTMASK_TIMEOUT);

    while(TRUE) {
        EvtMsk = chEvtWaitAny(EVTMASK_ACC_IRQ | EVTMASK_TIMEOUT);
        if(EvtMsk & EVTMASK_ACC_IRQ) {
            // Irq occured
            Uart.Printf("Evt\r");
            PinSet(GPIOA, 1);
            LedSmooth.SetSmoothly(LED_TOP_VALUE);
            // (Re)Start timeout timer
            chVTReset(&ITmr);
            chVTSet(&ITmr, MS2ST(TIMEOUT_TO_FADE_MS), TmrCallback, NULL);
        }
        else if(EvtMsk & EVTMASK_TIMEOUT) {
            // Timeout occured
            Uart.Printf("Timeout\r");
            PinClear(GPIOA, 1);
            LedSmooth.SetSmoothly(LED_BOTTOM_VALUE);
        }
     }
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
    LedSmooth.Init();
    Uart.Printf("\rRinca  AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    LedSmooth.SetSmoothly(LED_BOTTOM_VALUE);
    i2c.Init(I2C1, 100000);
    Acc.Init();
}

// Timer callback
static void TmrCallback(void *p) {
    //Uart.Printf("TO\r");
    chEvtBroadcastI(&IEvtSrcTimeout);
}
