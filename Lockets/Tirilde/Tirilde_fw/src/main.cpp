/*
 * File:   main.cpp of Tirilde
 * Author: Kreyl
 *
 */

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "kl_lib.h"
#include "led.h"
#include "cc1101.h"

#define ENABLE_TX
#define ENABLE_RX
//#define ALWAYS_RX

LedRGB_t Led;
klPin_t dp, gp;

// Sync module
#define CYCLE_DURATION  243     // ms
#define CYCLE_COUNT     4
class Sync_t {
private:
public:
    uint16_t CycleCounter;
    void Init(void);
    void Task(void);
} Sync;

// Prototypes
void GeneralInit(void);

// ============================ Implementation =================================
int main(void) {
    GeneralInit();
    //uint32_t Tmr;
    // ==== Main cycle ====
    while (1) {
    	Uart.Task();
        //Led.Task();
        CC.Task();
        Sync.Task();

        //dp = (CC.Aim != caRx);

//        if(Delay.Elapsed(&Tmr, 450)) {
//            dp = !dp;
////        	if(Cl == clBlack) Cl = clBlue;
////        	else Cl = clBlack;
////        	Led.SetColorSmoothly(Cl);
//        	//CC.Transmit();
//        }
    } // while(1)
}

inline void GeneralInit(void) {
    InitClock(clk1MHzInternal);
    klJtagDisable();

    Delay.Init();
    // DEBUG
    //Led.Init();
    dp.Init(GPIOA, 11, GPIO_Mode_Out_PP);
    dp=1;
    gp.Init(GPIOA, 8, GPIO_Mode_Out_PP);
    gp=1;

    Uart.Init(57600);
    Uart.Printf("\rTirilde\r");

    // Setup CC
    CC.Init();
    CC.SetChannel(0);
    CC.SetAddress(0);   // dummy
    CC.SetPower(plN6dBm);

    Sync.Init();
}

// ==================================== Sync ===================================
void Sync_t::Init() {
    CycleCounter = 0;
}

void Sync_t::Task() {
    static uint32_t FTmr;
    if(Delay.Elapsed(&FTmr, CYCLE_DURATION)) {
        dp=0;
        if (++CycleCounter >= CYCLE_COUNT) CycleCounter = 0;
        // Transmit at every cycle start
        PktTx.Addr = 4;
        CC.Transmit();
        //CC.TransmitAndWaitIdle();
        //CC.Receive();
        dp=1;
    }
}

void CC_t::TxEndHandler() {
    Receive();
    // Enter RX at zero cycle, otherwise sleep
//    if(Sync.CycleCounter == 0) Receive();
//    else EnterIdle();
}

void CC_t::NewPktHandler() {
    Uart.Printf("R %u\r", PktRx.TimerValue);
}


