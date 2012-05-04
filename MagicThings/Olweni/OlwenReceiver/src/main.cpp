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

//#define ON_THRESHOLD    (-72)   // dBm

LedSmooth_t Led;
bool SignalDetected;
uint32_t TimerNoSignal;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        Led.Task();
        CC.Task();
        // Switch led off in case of timeout
        if(SignalDetected) {
            if(Delay.Elapsed(&TimerNoSignal, 4005)) {   // timeout was not reset during this time
                Led.RampDown();
                SignalDetected = false;
            }
        } // if(SignalDetected)
    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div8);
    SystemCoreClockUpdate();

    Delay.Init();
    Delay.ms(63);
    UART_Init();
    Led.Init(GPIOB, 9, TIM4, 250, 0, 4, false);
    Led.Off();
    SignalDetected = false;

    // Setup CC
    CC.Init();
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(207);
    CC.Shutdown();
    CC.Wake();

    klPrintf("\rOlwen receiver\r");
}

// =============================== CC handling =================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
#define RX_NON_STOP
void CC_t::Task(void) {
    if (IsShutdown) return;
#ifdef RX_NON_STOP
    static uint32_t Tmr;
#endif

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
            #ifdef RX_NON_STOP
            if (Delay.Elapsed(&Tmr, 999)) {
            #endif
                //klPrintf("RX\r");
                EnterRX();
            #ifdef RX_NON_STOP
            }
            break;
        case CC_STB_RX:
            if (Delay.Elapsed(&Tmr, 45)) { // Time to sleep
                EnterIdle();
            }
            #endif
            break;

        default: // Just get out in other cases
            //klPrintf("Other: %X\r", State);
            break;
    } //Switch InnerState
}

void CC_t::IRQHandler() {
    // Will be here if packet received successfully or in case of wrong address
    if (ReadRX()) {
        // Check address
        if(RX_Pkt.To == 207) {   // This packet is ours
            //klPrintf("RSSI: %i\r", RSSI_dBm());
//            if (RSSI_dBm() > ON_THRESHOLD)
            Delay.Reset(&TimerNoSignal);    // Reset led-off timeout
            if (!SignalDetected) {
                SignalDetected = true;
                Led.RampUp();
            }
        }
    } // if size>0
    FlushRxFIFO();
}
