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
    Bcklt.On(100);

    Lcd.Init();

    Battery.Init();
    Battery.State = bsFull;
    EVENT_NewBatteryState();
    Battery.EvtNewState = EVENT_NewBatteryState;

    CC.Init();
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(CC_ADDRESS);
    //CC.EvtNewPkt = EVENT_NewPacket;

    Beep.Init();
    Beep.SetSound(&IdleBeep);

    Signal.Init();

    klPrintf("\rDetector\r");
}

// =============================== Events ======================================
//void EVENT_NewPacket(void) {
//    int32_t RSSI_dBm = CC.RX_Pkt.RSSI;
//    if (RSSI_dBm >= 128) RSSI_dBm -= 256;
//    RSSI_dBm  = (RSSI_dBm / 2) - 69;
//    //klPrintf("Ch: %u; RSSI: %i\r", CC.ChannelN, RSSI_dBm);
//    // Display signal
//    int32_t RSSI = RSSI_dBm + 90;
//    if (RSSI < 1) RSSI = 1;
//    if (RSSI > 50) RSSI = 50;
//    Signal.Display(CC.RX_Pkt.From, RSSI);
//}

void EVENT_NewBatteryState(void) {
    switch (Battery.State) {
        case bsFull:  Lcd.DrawImage(80, 0, icon_BatteryFull);  break;
        case bsHalf:  Lcd.DrawImage(80, 0, icon_BatteryHalf);  break;
        case bsEmpty: Lcd.DrawImage(80, 0, icon_BatteryEmpty); break;
    }
}

// ============================= Signal_t ======================================
void Signal_t::Task() {
    bool IsIdle = true;
    // Check all channels if displaying needed
    for (uint8_t i=0; i<7; i++) {
        if (INew[i]) {
            INew[i] = false;
            Lcd.DrawPeak(i, IRSSI[i]);
            Beep.SetSound(&AlienBeep);
            Exists[i] = true;
        } // if new
        else if (Delay.Elapsed(&ITimer[i], 999)) {  // not new and timeout passed
            Lcd.DrawPeak(i, 0); // Clear peak
            Exists[i] = false;
        }
        // Check if set Idle sound
        if (Exists[i]) IsIdle = false;
    } // for

    // Set idle sound if needed
    if (IsIdle) Beep.SetSound(&IdleBeep);
}

void Signal_t::Remember(uint8_t AChannel, int32_t RawRSSI) {
    INew[AChannel] = true;
    Delay.Reset(&ITimer[AChannel]);
    if (RawRSSI >= 128) RawRSSI -= 256;
    RawRSSI = (RawRSSI / 2) - 69;    // now it in dBm
    // Scale to display
    RawRSSI += 90;
    if (RawRSSI < 0) RawRSSI = 1;
    if (RawRSSI > 50) RawRSSI = 50;
    IRSSI[AChannel] = RawRSSI;
}

void Signal_t::Init() {
    for(uint8_t i=0; i<8; i++) {
        Lcd.DrawPeak(i, 0);
        INew[i] = false;
    }
}
