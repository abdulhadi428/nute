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
#define PKT_DURATION    45      // ms
#define CYCLE_DURATION  243     // ms
#define CYCLE_COUNT     4
class Sync_t {
private:
    void SetTxTimeSlot(uint16_t ATimeSlot) { TIM15->CCR1 = ATimeSlot * PKT_DURATION * 1.5; }
    uint16_t GetTimerValue(void) { return (TIM15->CNT); }
    void SetTimerValue(uint16_t AValue) { TIM15->CNT = AValue; }
    uint16_t ICycleCounter;
public:
    uint8_t Addr, FreeAddr, NetID;
    void Init(void);
    void NewCycleHandler(void);
    void TimeSlotHandler(void);
    void TimerStart(void) { TIM15->CR1 |= TIM_CR1_CEN; }
    void TimerStop(void)  { TIM15->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN)); }
} Sync;
// Declare Timer IRQ. Use externC to make it visible from asm file.
extern "C" {
void TIM1_BRK_TIM15_IRQHandler(void);
}


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
    Addr = 1;
    FreeAddr = 2;
    NetID = 0;
    ICycleCounter = 0;
    // Interrupt config
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Cycle timer: TIMER15
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
    TIM15->PSC = (uint16_t)(SystemCoreClock / 1000) - 1;    // 1 ms per tick
    TIM15->ARR = CYCLE_DURATION;                            // Top value
    SetTxTimeSlot(1);
    TimerStart();
    // Interrupt config
    TIM_ITConfig(TIM15, TIM_IT_Update | TIM_IT_CC1, ENABLE);
}

void Sync_t::NewCycleHandler() {
    dp = 0;
    /*
    if (++ICycleCounter >= CYCLE_COUNT) {  // Zero cycle begins
        ICycleCounter = 0;
#if defined ENABLE_RX || defined ALWAYS_RX
        CC.Receive();   // Enter RX-before-TX
#endif
    }
    else {
#ifdef ALWAYS_RX
        CC.Receive();
#else
        CC.EnterIdle();
#endif
    }
    //*/
    CC.Receive();
    dp=1;
}

void Sync_t::TimeSlotHandler() {
#ifdef ENABLE_TX
    //dp = 0;
//    TimerStop();
    // Setup pkt to TX
//    PktTx.Addr = Addr;
//    PktTx.FreeAddr = FreeAddr;
//    PktTx.NetID = NetID;
//    PktTx.CycleCounter = ICycleCounter;
//    PktTx.TimerValue = GetTimerValue();
    // Start transmission
//    dp = 0;
    CC.TransmitAndWaitIdle();
//    dp = 1;
//    Sync.TimerStart();
#endif
#ifdef ALWAYS_RX
    CC.Receive();  // Enter RX after TX
#elif defined ENABLE_RX
    //if (ICycleCounter == 0) CC.Receive();  // Enter RX after TX
#endif
    CC.Receive();
}

// Timer Interrupt
void TIM1_BRK_TIM15_IRQHandler(void) {
    if(TIM15->SR & TIM_IT_Update) {
        TIM_ClearITPendingBit(TIM15, TIM_IT_Update);    // Clear interrupt flag
        Sync.NewCycleHandler();
    }
    // ==== Time to TX ====
    else if (TIM15->SR & TIM_IT_CC1) {
        TIM_ClearITPendingBit(TIM15, TIM_IT_CC1);       // Clear interrupt flag
        Sync.TimeSlotHandler();
    }
}

void CC_t::NewPktHandler() {
    Uart.Printf("R %u\r", PktRx.CycleCounter);
}


