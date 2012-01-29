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
    bool IdleFlag = true;
    // Check all channels if clear needed
    for (uint8_t i=0; i<7; i++) {
        if (IExist[i]) {
            if (Delay.Elapsed(&ITimer[i], NOSIGNAL_DELAY)) {    // Check if time to clear
                IExist[i] = false;
                Lcd.DrawPeak(i, 0);
            }
            else {
                IdleFlag = false;
                if (!IDisplayed[i]) {
                    IDisplayed[i] = true;
                    int32_t r = IRSSI[i];
                    if (r>= 128) r -= 256;
                    r = (r / 2) - 69;    // now it in dBm
                    // Scale to display
                    r += 90;
                    if (r < 1) r = 1;
                    if (r > 50) r = 50;
                    Lcd.DrawPeak(i, r);
                } // if not displayed
            } // if delay
        } // if exist
    } // for
    // Set correct sound
    if (IdleFlag) Beep.SetSound(&IdleBeep);
    else Beep.SetSound(&AlienBeep);
}

void Signal_t::Remember(uint8_t AChannel, int32_t RawRSSI) {
    Delay.Reset(&ITimer[AChannel]); // Reset clear timer
    IExist[AChannel] = true;
    IDisplayed[AChannel] = false;
    IRSSI[AChannel] = RawRSSI;
}

void Signal_t::Init() {
    for(uint8_t i=0; i<8; i++) {
        Lcd.DrawPeak(i, 0);
        IExist[i] = false;
    }
}
