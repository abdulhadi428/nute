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
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"
#include "cc1190.h"

LedBlinkInverted_t Led;
cc1190_t cc1190;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();
    //uint32_t Tmr;
    //bool gn = false;
    while (1) {
        //Led.Task();
        CC.Task();
//        if(Delay.Elapsed(&Tmr, 1800)) {
//            //Led.Blink(45);
//            if(gn) cc1190.SetHighGainMode();
//            else cc1190.SetLowGainMode();
//            gn = !gn;
//        }
    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();

    Delay.Init();
    Delay.ms(63);
    UART_Init();

    klJtagDisable();

    Led.Init(GPIOB, 1);
    Led.Off();

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(4);
    cc1190.Init();
    //cc1190.SetHighGainMode();

    klPrintf("\rCollar rcvr\r");
}

// =============================== CC handling =================================
void CC_t::Task(void) {
    // Do with CC what needed
    GetState();
    switch (IState) {
        case CC_STB_RX_OVF:
            FlushRxFIFO();
            klPrintf("RX ovf\r");
            break;
        case CC_STB_TX_UNDF:
            FlushTxFIFO();
            klPrintf("TX ovf\r");
            break;

        case CC_STB_IDLE:
            cc1190.LnaEnable();
            EnterRX();
            //if (Delay.Elapsed(&Timer, 100)) {
              //  klPrintf("TX\r");
                // Prepare packet to send
//                TX_Pkt.PktID++;
//                WriteTX();
                //cc1190.SetHighGainMode();
                //cc1190.SetLowGainMode();
//                cc1190.PaEnable();
//                EnterTX();
            //}
            break;

        case CC_STB_RX:
            //Uart.PrintString("\rRX");
//            if (GDO0_IsHi()) GDO0_WasHi = true;
//            // Check if GDO0 has fallen
//            else if (GDO0_WasHi) {
//                //UART_PrintString("\rIRQ\r");
//                GDO0_WasHi = false;
//                FifoSize = ReadRegister(CC_RXBYTES); // Get number of bytes in FIFO
//                if (FifoSize != 0) {
//                    ReadRX(RX_PktArray, (CC_PKT_LEN+2));    // Read two extra bytes of RSSI & LQI
//                    EVENT_NewPacket();
//                } // if size>0
//            } // if falling edge
            break;

        case CC_STB_TX:
            //UART_PrintString("\rTX");
            break;

        default: // Just get out in other cases
            //Uart.PrintString("\rOther: ");
            //Uart.PrintUint(CC.State);
            break;
    }//Switch
}

void CC_t::IRQHandler() {
    if (ReadRX()) {
        //klPrintf("RX: %A\r", (uint8_t*)&RX_Pkt, CC_PKT_LEN+2);
        klPrintf("RSSI: %i\r", RSSI_dBm());
        FlushRxFIFO();
    } // if size>0
}
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
/*#define RX_NON_STOP
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
*/
