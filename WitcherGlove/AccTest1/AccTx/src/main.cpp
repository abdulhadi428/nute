/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_lib.h"
#include "cc2500.h"
#include "acc_mma.h"
#include "i2c_mgr.h"
#include "adc.h"
#include "led.h"

//#define _TX_

// Variables
#ifdef _TX_
Acc_t Acc;
#endif

LedBlink_t Led;

// Prototypes
static void Init();

// ============================== Implementation ===============================
int main(void) {
    Init();

    uint32_t Tmr;
    while(1) {
        Led.Task();
        CC.Task();
        Uart.Task();
        Battery.Task();
#ifdef _TX_
        Acc.Task();
        i2cMgr.Task();
#endif

        // Indication
        if(Battery.State == bsEmpty) {
            if(Delay.Elapsed(&Tmr, 450)) Led.Blink(99);
        }
        else if(Delay.Elapsed(&Tmr, 2700)) Led.Blink(99);
    } // while 1
}

static inline void Init() {
	InitClock(clk8MHzInternal);
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
#ifdef _TX_
    Uart.Printf("\rAccTestTx\r");
#else
    Uart.Printf("\rAccTestRx\r");
#endif

#ifdef _TX_
    i2cMgr.Init();
    Acc.Init();		// Accelerometer
#endif
    // Setup CC
    CC.Init();
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(7);
    CC.Wake();

    Battery.Init();

    Led.Init(GPIOA, 1);
}

// =============================== CC handling =================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
void CC_t::Task(void) {
    if (IsShutdown) return;
    // Do with CC what needed
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
#ifdef _TX_
        	if(Acc.NewData()) {
				//Uart.Printf("TX\r");
				// Prepare packet to send
        		TX_Pkt.Addr = 207;
				TX_Pkt.x = Acc.x;
				TX_Pkt.y = Acc.y;
				TX_Pkt.z = Acc.z;
				WriteTX();
				EnterTX();
        	}
#else
        	//Uart.Printf("RX\r");
        	EnterRX();
#endif
            break;

        default: // Just get out in other cases
            //klPrintf("Other: %X\r", State);
            break;
    } //Switch
}

void CC_t::IRQHandler() {
#ifndef _TX_
	// Will be here if packet received successfully or in case of wrong address
	//if (ReadRX())  // Proceed if read was successful
	ReadRX();
	if(RX_Pkt.Addr == 207) Uart.Printf("%d;%d;%d;\r", RX_Pkt.x, RX_Pkt.y, RX_Pkt.z);

	FlushRxFIFO();
#endif
}
