/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "cc2500.h"
#include "beep.h"
#include "lcd110x.h"

#include "bcklt.h"

void EVENT_NewPacket(void);
void GeneralInit(void);
void DisplaySignal(int32_t RSSI_dBm, uint8_t LQI);
void DisplayClear(void);

uint32_t ClearTmr;

int main(void) {
    GeneralInit();
    DisplayClear();

    while (1) {
        Beep.Task();
        Lcd.Task();
        CC.Task();
        // Clear screen if needed
        if(Delay.Elapsed(&ClearTmr, 999)) DisplayClear();
    } // while 1
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();
    Bcklt.Init();
    Bcklt.On(10);

    Lcd.Init();

    CC.Init();
    CC.EvtNewPkt = EVENT_NewPacket;

    Beep.Init();
    Beep.SetSound(&NothingBeep);

    klPrintf("\rDetector\r");
}

// Signal show
void DisplaySignal(int32_t RSSI_dBm, uint8_t LQI) {
    // Output to uart
    klPrintf("RSSI: %i; LQI:%u\r", RSSI_dBm, CC.RX_Pkt.LQI);
    // Output on screen
    Lcd.Printf(0, 0, "RSSI: %i  ", RSSI_dBm);
    Lcd.Printf(0, 1, "LQI:  %u  ", LQI);
    Beep.SetSound(&AlienBeep);
}
void DisplayClear(void) {
    Lcd.Printf(0, 0, "RSSI:     ");
    Lcd.Printf(0, 1, "LQI:      ");
    Beep.SetSound(&NothingBeep);
}

// Events
void EVENT_NewPacket(void) {
    int32_t RSSI_dBm = CC.RX_Pkt.RSSI;
    //if (RSSI_dBm == 0) return;
    if (RSSI_dBm >= 128) RSSI_dBm -= 256;
    RSSI_dBm  = (RSSI_dBm / 2) - 69;
    DisplaySignal(RSSI_dBm, CC.RX_Pkt.LQI);
    Delay.Reset(&ClearTmr); // Don't clear screen soon
}
