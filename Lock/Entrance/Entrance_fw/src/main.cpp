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

Signal_t Signal;
LedBlink_t Led;

LedSmooth_t Light;

void GeneralInit();

#define CC_ADDRESS 207

int main(void) {
    GeneralInit();

    while (1) {
        Uart.Task();
        CC.Task();
        Led.Task();
        Signal.Task();
        Light.Task();
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
            }
            break;

        default: // Just get out in other cases
            //klPrintf("Other: %X\r", State);
            break;
    } //Switch
}

void CC_t::IRQHandler() {
    if (SearchState == IsCalling) { // Packet transmitted, enter RX
        if(++PktCounter == 1) {     // Several packets sent
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

// ============================= Signal_t ======================================
void Signal_t::Task() {
    // Check if someone is near. Fear of the dark, he-he
    int32_t TopRssi = -999; // dummy
    static int32_t OldTopRssi = -999;
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
        if(Al->Exists)
            if(Al->RSSI > TopRssi) TopRssi = Al->RSSI;
    } // for

    // Setup brightness depending on RSSI
    if(TopRssi != OldTopRssi) {
        OldTopRssi = TopRssi;
        if(TopRssi >= MIN_RSSI_TO_USE) {
            int32_t FBrt = BRT_A * TopRssi + BRT_B;
            if(FBrt < 1) FBrt = 1;
            if(FBrt > MAX_BRT) FBrt = MAX_BRT;
            Light.SetSmoothly((uint16_t)FBrt);
            Uart.Printf("RSSI: %i; Brt: %u\r", TopRssi, FBrt);
        }
        else {
            Uart.Printf("Nobody here\r");
            Light.SetSmoothly(0);
        }
    }
}

void Signal_t::Remember(uint8_t AAddress, int32_t RawRSSI) {
    // Calculate RSSI
    if (RawRSSI >= 128) RawRSSI -= 256;
    RawRSSI = (RawRSSI / 2) - 69;    // now it is in dBm
    // Check if display
    Alien_t *Al = &Alien[AAddress - MIN_ADDRESS];
    if (RawRSSI > MIN_RSSI_TO_USE) {
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
