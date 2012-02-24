/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "kl_util.h"
#include "led.h"
#include "cc2500.h"
#include "sensor.h"

#include "kl_gpio.h"

// Variables
uint8_t ID = 1;
Led_t Led(GPIOA, 1);

// Prototypes
void GeneralInit(void);
void Event_Trigger(void);
void Event_NoTrigger(void);

//extern "C" {
//ftVoid_Void EXTI12_IRQHandler;
//}

//void __attribute__ ((interrupt)) f(void) ;

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
        Sensor.Task();
    }
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();

    // Sensor
    Sensor.EvtTrigger = Event_Trigger;
    Sensor.EvtNoTrigger = Event_NoTrigger;

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(ID);
    CC.Shutdown();

    klPrintf("\rTransmitter %u\r", ID);
}

// ======== Sensor ========
void Event_Trigger(void) {
    Led.On();
    CC.Wake();
}
void Event_NoTrigger(void) {
    Led.Off();
    CC.Shutdown();
}

// =============================== CC handling =================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
typedef enum {IsWaiting, IsReplying} WaitState_t;
WaitState_t SearchState = IsWaiting;
uint8_t PktCounter=0;

void CC_t::Task(void) {
    if (IsShutdown) return;
    // Do with CC what needed
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
            if (SearchState == IsWaiting) {
                EnterRX();
            }
            else {
                //klPrintf("TX\r");
                // Prepare packet to send
                TX_Pkt.To = 207;
                WriteTX();
                EnterTX();
                //klPrintf("TX\r");
            }
            break;

        default: // Just get out in other cases
            //klPrintf("Other: %X\r", State);
            break;
    } //Switch
}

void CC_t::IRQHandler() {
    if (SearchState == IsWaiting) {
        // Will be here if packet received successfully or in case of wrong address
        if (ReadRX()) { // Proceed if read was successful
            // Check address
            if(RX_Pkt.To == ID) {   // This packet is ours
                //klPrintf("From: %u; RSSI: %u\r", RX_Pkt.From, RX_Pkt.RSSI);
                SearchState = IsReplying;
                PktCounter=0;
            }
        } // if read
        FlushRxFIFO();
    }
    else {  // Packet transmitted
        if(++PktCounter == 2) SearchState = IsWaiting;
    }
}
