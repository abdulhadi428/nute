/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "kl_util.h"
#include "cc2500.h"
#include "kl_lib.h"
#include "led.h"

enum CCState_t {csWaiting, csTransmitting} CCState;

LedBlink_t Led;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        Led.Task();
        CC.Task();
    } // while 1
}

void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div8);
    SystemCoreClockUpdate();

    Delay.Init();
    Delay.ms(63);
    UART_Init();
    Led.Init(GPIOA, 0);

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = 7;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(7);
    CC.Wake();
    CCState = csWaiting;

    klPrintf("\rOlwen transmitter\r");
}

// =============================== CC handling =================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
void CC_t::Task(void) {
    if (IsShutdown) return;
    static uint32_t Tmr;

    switch (CCState) {
        case csWaiting:
            GetState();
            switch (InnerState) {
                case CC_STB_RX_OVF:
                    klPrintf("RX ovf\r");
                    FlushRxFIFO();
                    break;
                case CC_STB_TX_UNDF:
                    klPrintf("TX undf\r");
                    FlushTxFIFO();
                    break;
                case CC_STB_IDLE:
                    if (Delay.Elapsed(&Tmr, 999)) Led.Blink(45);
                    // Prepare packet to send
                    TX_Pkt.To = 207;
                    WriteTX();
                    EnterTX();
                    CCState = csTransmitting;
                    //klPrintf("TX\r");
                    break;

                default: // Just get out in other cases
                    //klPrintf("Other: %X\r", State);
                    break;
            } //Switch InnerState
            break; // case waiting

        default: break;
    } // switch ccstate
}

void CC_t::IRQHandler() {
    // Packet transmitted
    CCState = csWaiting;
}
