/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "ir.h"

// IR pkt: 0xxx xxxx  0000 01
//          ^^^^^^^^ ID
// Pkt is left-adjusted.

ir_t ir;
static inline void Init();
static uint8_t ID;

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();

    while(TRUE) {
        chThdSleepMilliseconds(180);
        uint16_t w = ID;
        w <<= 8;
        w |= 0x04;
        //Uart.Printf("%X\r", w);
        ir.TransmitWord(w, 100);
    }
}

static inline uint8_t GetID() {
    uint8_t id = 0;
    for(uint8_t i=3; i<=9; i++) {
        id <<= 1;
        PinSetupIn(GPIOB, i, pudPullUp);
        Delay_ms(1);
        if(!PinIsSet(GPIOB, i)) id |= 1;    // Fuse detected
        PinSetupAnalog(GPIOB, i);
    }
    return id;
}

void Init() {
    JtagDisable();
    Uart.Init(57600);
    ir.Init();
    ID = GetID();
    ir.Init();
    Uart.Printf("\rGirandole #%u  AHB=%u; APB1=%u; APB2=%u\r", ID, Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
}
