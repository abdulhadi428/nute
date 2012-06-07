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
#include "nute.h"

LedBlinkInverted_t Led;
Tixe_t Tixe;

// Prototypes
void GeneralInit(void);
void ClbckFound(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    Tixe.Address = 72;
    Tixe.Callback = ClbckFound;

    uint32_t Tmr;
    while (1) {
        //Led.Task();
        CC.Task();
        Nute.Task();
        if(Delay.Elapsed(&Tmr, 1800)) {
            Led.Off();
            Nute.Search(&Tixe);
        }
    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);
    UART_Init();

    Led.Init(GPIOB, 1);
    Led.On();

    Nute.Init(1);

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(Nute.TX_Pkt.AddrFrom);

    klPrintf("\rCollar station\r");
}

void ClbckFound(void) {
    if (Tixe.IsOnline) {
        klPrintf("Found\r");
        Led.On();
        klPrintf("Time: %u:%u:%u\r", Tixe.Situation.Time.H, Tixe.Situation.Time.M, Tixe.Situation.Time.S);
        if (Tixe.Situation.IsFixed)
            klPrintf("Lat: %i; Lng: %i; SatCount: %u; Precision: %u\r", Tixe.Situation.Lattitude, Tixe.Situation.Longtitude, Tixe.Situation.SatCount, Tixe.Situation.Precision);
        else klPrintf("No fix\r");
    }
    else klPrintf("No answer\r");
}
