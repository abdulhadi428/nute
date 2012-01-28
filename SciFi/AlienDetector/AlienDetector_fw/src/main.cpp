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
#include "images.h"
#include "main.h"
#include "bcklt.h"
#include "adc.h"

Signal_t Signal;

int main(void) {
    GeneralInit();

    while (1) {
        Lcd.Task();
        CC.Task();
        Beep.Task();
        Signal.Task();
        Battery.Task();
    } // while 1
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();
    Bcklt.Init();
    Bcklt.On(10);

    Lcd.Init();

    Battery.Init();
    Battery.State = bsFull;
    EVENT_NewBatteryState();
    Battery.EvtNewState = EVENT_NewBatteryState;

    CC.Init();
    CC.EvtNewPkt = EVENT_NewPacket;

    Beep.Init();
    Beep.SetSound(&IdleBeep);

    Signal.Init();

    klPrintf("\rDetector\r");
}

// =============================== Events ======================================
void EVENT_NewPacket(void) {
    int32_t RSSI_dBm = CC.RX_Pkt.RSSI;
    if (RSSI_dBm >= 128) RSSI_dBm -= 256;
    RSSI_dBm  = (RSSI_dBm / 2) - 69;
    //klPrintf("RSSI: %i\r", RSSI_dBm);
    // Display signal
    int32_t RSSI = RSSI_dBm + 90;
    if (RSSI < 1) RSSI = 1;
    if (RSSI > 50) RSSI = 50;
    Signal.Display(1, RSSI);
}

void EVENT_NewBatteryState(void) {
    switch (Battery.State) {
        case bsFull:  Lcd.DrawImage(80, 0, icon_BatteryFull);  break;
        case bsHalf:  Lcd.DrawImage(80, 0, icon_BatteryHalf);  break;
        case bsEmpty: Lcd.DrawImage(80, 0, icon_BatteryEmpty); break;
    }
}

// ============================= Signal_t ======================================
void Signal_t::Task() {
    bool IdleFlag = true;
    // Check all channels if clear needed
    for (uint8_t i=1; i<7; i++) {
        if (IFlag[i]) {
            if (Delay.Elapsed(&ITimer[i], NOSIGNAL_DELAY)) {
                IFlag[i] = false;
                Lcd.DrawPeak(i, 0);
            }
            else IdleFlag = false;
        }
    }
    // Set idle sound if needed
    if (IdleFlag) Beep.SetSound(&IdleBeep);
}

void Signal_t::Display(uint8_t AChannel, uint8_t RSSI) {
    Lcd.DrawPeak(AChannel, RSSI);
    // Reset timer
    Delay.Reset(&ITimer[AChannel]);
    IFlag[AChannel] = true;
    // Set correct sound
    Beep.SetSound(&AlienBeep);
}

void Signal_t::Init() {
    for(uint8_t i=0; i<8; i++) {
        Lcd.DrawPeak(i, 0);
        IFlag[i] = 0;
    }
}
