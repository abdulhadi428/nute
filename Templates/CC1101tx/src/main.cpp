/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_lib.h"
#include "kl_printf.h"
#include "led.h"
#include "cc1101.h"

void GeneralInit();

int main(void) {
    GeneralInit();
    Led_t Led(GPIOB, 9);

    uint32_t Tmr;

    while (1) {
        CC.Task();
        Led.Task();
        if (Delay.Elapsed(&Tmr, 999)) Led.Blink();
    } // while 1
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    klPrintfInit();

    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(1);

    klPrintf("\rTransmitter\r");
}

// ================================= CC task ===================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
/*
#define RX_WAIT_TIME    2
enum SearchState_t {IsCalling, IsWaiting} SearchState;
uint8_t PktCounter=0;

void CC_t::Task(void) {
    // Proceed with state processing
    GetState();
    switch (State) {
        case CC_STB_RX_OVF:
            klPrintf("RX ovf\r");
            FlushRxFIFO();
            break;
        case CC_STB_TX_UNDF:
            klPrintf("TX undf\r");
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
*/
