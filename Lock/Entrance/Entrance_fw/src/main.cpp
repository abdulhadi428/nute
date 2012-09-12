/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_lib.h"
#include "cc2500.h"
#include "main.h"
#include "led.h"
#include "lcd1200.h"
#include "keys.h"
#include "interface.h"
#include "FlashStorage.h"

Signal_t Signal;
LedBlink_t Led;
LedSmooth_t Light;
FlashStorage_t Store;

void GeneralInit();

#define CC_ADDRESS 207

int main(void) {
    GeneralInit();

    /* Outer interface connector. Beware: #10 is actually #1 here
     * 1 gnd
     * 2 vcc    PB15
     * 3 xcs    PB14
     * 4 xres   PB13
     * 5 sda    PB12
     * 6 sclk   PB11
     * 7 k3     PB10
     * 8 k2     PB9
     * 9 k1     PB8
     */

    while (1) {
        Uart.Task();
        CC.Task();
        Led.Task();
        Signal.Task();
        Light.Task();
        Lcd.Task();
        Keys.Task();
        Interface.Task();
    } // while 1
}

void GeneralInit(void) {
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
    Led.Init(GPIOA, 1);
    Led.Blink(702);

    Light.Init(GPIOB, 6, TIM4, 250, 36, 1, false);

    CC.Init();
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(CC_ADDRESS);
    CC.TX_Pkt.From = CC_ADDRESS;

    Signal.Init();

    // Outer interface
    klGpioSetupByN(GPIOB, 15, GPIO_Mode_Out_PP);    // VCC
    klGpioSetByN(GPIOB, 15);
    Delay.ms(99);
    Lcd.Init();
    Keys.Init();

    Interface.Init();
    Interface.SettingsLoad();
    Interface.DisplayMaxLvl();
    Interface.DisplayMinLvl();
    Interface.SettingChanged();

    Uart.Printf("\rEntrance\r");
}

// ================================= CC task ===================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
#define RX_WAIT_TIME    2
enum SearchState_t {IsCalling, IsWaiting} SearchState;
uint8_t PktCounter=0;

void CC_t::Task(void) {
    // Proceed with state processing
    GetState();
    switch (State) {
        case CC_STB_RX_OVF:
            Uart.Printf("RX ovf\r");
            FlushRxFIFO();
            break;
        case CC_STB_TX_UNDF:
            Uart.Printf("TX undf\r");
            FlushTxFIFO();
            break;

        case CC_STB_IDLE:
            if (SearchState == IsCalling) { // Call alien
                WriteTX();
                //klPrintf("TX: %u\r", TX_Pkt.To);
                EnterTX();
            }
            else {  // Is waiting
                if (Delay.Elapsed(&Timer, RX_WAIT_TIME)) SearchState = IsCalling;
                else EnterRX();
            }
            break;

        case CC_STB_RX:
            if (Delay.Elapsed(&Timer, RX_WAIT_TIME)) {
                SearchState = IsCalling;
                EnterIdle();
                // Note that all was interrogated
                if((TX_Pkt.To == MAX_ADDRESS) and (PktCounter == TRY_COUNT-1)) Signal.AllThemCalled = true;
            }
            break;

        default: // Just get out in other cases
            //klPrintf("Other: %X\r", State);
            break;
    } //Switch
}

void CC_t::IRQHandler() {
    if (SearchState == IsCalling) { // Packet transmitted, enter RX
        if(++PktCounter == TRY_COUNT) {     // Several packets sent
            PktCounter = 0;
            // Increase packet address
            TX_Pkt.To++;
            if (TX_Pkt.To > MAX_ADDRESS) TX_Pkt.To = MIN_ADDRESS;
            // Switch to waiting state
            SearchState = IsWaiting;
            Delay.Reset(&Timer);
        }
    }
    else { // Will be here if packet received successfully or in case of wrong address
        if (ReadRX()) {
            // Check address
            if(RX_Pkt.To == CC_ADDRESS) {   // This packet is ours
                //klPrintf("From: %u; RSSI: %u\r", RX_Pkt.From, RX_Pkt.RSSI);
                Signal.Remember(RX_Pkt.From, RX_Pkt.RSSI);
            }
            FlushRxFIFO();
        } // if size>0
    } // if is waiting
}

// ============================ Settings =======================================
void Interface_t::SettingsLoad() {
    int32_t Arr[2];
    Store.Load((uint32_t*)Arr, 2);
    if((Arr[0] < -111) or (Arr[0] > 0)) Interface.MinLvl = -45;
    else Interface.MinLvl = Arr[0];
    if((Arr[1] < -111) or (Arr[1] > 0)) Interface.MaxLvl = -63;
    else Interface.MaxLvl = Arr[1];
}

void Interface_t::SettingsSave() {
    uint32_t Arr[2];
    Arr[0] = (uint32_t)Interface.MinLvl;
    Arr[1] = (uint32_t)Interface.MaxLvl;
    Store.Save(Arr, 2);
}

// Recalculate coefficients
void Interface_t::SettingChanged() {
    // Coeffs of brightness conversion
    Signal.BrtA = ((int32_t)((MAX_BRT - MIN_BRT) / (MaxLvl - MinLvl)));
    Signal.BrtB =   ((int32_t)(MIN_BRT - Signal.BrtA * MinLvl));
}

// ============================= Signal_t ======================================
void Signal_t::Task() {
    if(!AllThemCalled) return;  // Nothing to do if we did not call everybody
    AllThemCalled = false;
    // Iterate all to check if someone is near. Fear of the dark, he-he.
    int32_t TopRssi = Interface.MinLvl - 1; // Even less than nothing ;)
    Alien_t *Al;
    for (uint8_t i=0; i<ALIEN_COUNT; i++) {
        Al = &Alien[i];
        if (Al->New) {
            Al->New = false;
            Al->Exists = true;
        } // if new
        else if (Delay.Elapsed(&Al->Timer, 999)) {  // not new and timeout passed
            Al->Exists = false;
        }
        // Calculate top RSSI
        if(Al->Exists) {
            if(Al->RSSI > TopRssi) TopRssi = Al->RSSI;
            //Uart.Printf("ID: %u; Rssi: %i\r", i+MIN_ADDRESS, Al->RSSI);
            Interface.DisplayRxLvl(i+MIN_ADDRESS, Al->RSSI);
        }
    } // for
    if(TopRssi < Interface.MinLvl) Interface.DisplayRxLvl(0, 0);    // Clear screen

    // Now we have top RSSI value of all the aliens.
    // ==== Setup brightness depending on RSSI ====
    int32_t FBrt;
    if(TopRssi >= Interface.MinLvl) {
        FBrt = BrtA * TopRssi + BrtB;
        if(FBrt < 1) FBrt = 1;
        if(FBrt > MAX_BRT) FBrt = MAX_BRT;
    }
    else FBrt = 0;

    // ==== Average calculation ====
    AvgBuf[AvgIndx++] = FBrt;   // Add new value
    if(AvgIndx == AVG_SZ) AvgIndx = 0;
    // Calculate sum
    int32_t Average=0;
    for(uint32_t i=0; i<AVG_SZ; i++) Average += AvgBuf[i];
    // Calculate average
    Average /= AVG_SZ;
    //Uart.Printf("Rssi: %i; Brt: %i; Avg: %i\r", TopRssi, FBrt, Average);

    Light.SetSmoothly((uint16_t)Average);
}

void Signal_t::Remember(uint8_t AAddress, int32_t RawRSSI) {
    // Calculate RSSI
    if (RawRSSI >= 128) RawRSSI -= 256;
    RawRSSI = (RawRSSI / 2) - 69;    // now it is in dBm
    // Check if display
    Alien_t *Al = &Alien[AAddress - MIN_ADDRESS];
    if (RawRSSI > Interface.MinLvl) {
        Al->New = true;
        Delay.Reset(&Al->Timer);
        Al->RSSI = RawRSSI;
    }
}

void Signal_t::Init() {
    for(uint8_t i=0; i<8; i++) {
        Alien[i].New = false;
    }
}
