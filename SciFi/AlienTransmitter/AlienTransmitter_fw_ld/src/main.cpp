/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "delay_util.h"
#include "kl_util.h"
#include "led.h"
#include "cc2500.h"
#include "acc_mma.h"
#include "kl_lib.h"
#include "main.h"

//#define LED_ENABLE

// Variables
uint8_t ID = 2;
#ifdef LED_ENABLE
Led_t Led(GPIOA, 1);
#endif
Acc_t Acc;
bool IsOn;
// Switchers
Switchers_t Switchers;

// Prototypes
void GeneralInit(void);
void TxOn(void);
void TxOff(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
        Acc.Task();
        i2cMgr.Task();
        Switchers.Task();
    } // while 1
}

void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();

    Delay.Init();
    Delay.ms(63);
    UART_Init();

    // Accelerometer
    Acc.Init();
    Acc.EvtTrigger = TxOn;
    Acc.EvtNoTrigger = TxOff;

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(ID);
    CC.Shutdown();
    IsOn = false;

    // Enable PWR and BKP clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    // Switchers
    Switchers.Init();
    Switchers.UpdateState();    // Setup initial state depending on switchers

    klPrintf("\rTransmitter %u\r", ID);
}

// Switchers
void Switchers_t::Init() {
    SFront.Init(GPIOB, 1, GPIO_Mode_IPU);
    SBack.Init (GPIOB, 8, GPIO_Mode_IPU);
}

void Switchers_t::UpdateState(void) {
    if (FrontIsOn()) {
        if (BackIsOn()) {
            Acc.Disable();
            TxOn();
        }
        else {
            Acc.Enable();
            TxOff();
        }
    }
    else {
        Acc.Disable();
        TxOff();
        // Prepare to sleep: configure front switch GPIO line to be event source
        // Connect EXTI Line to front switch GPIO
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
        // Configure front switch EXTI line
        EXTI_InitTypeDef EXTI_InitStructure;
        EXTI_InitStructure.EXTI_Line = EXTI_Line1;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);
        // Enter sleep mode
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
        // At this stage the system has resumed from STOP mode
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
        EXTI_Init(&EXTI_InitStructure);
    }
}

void Switchers_t::Task(void) {
    static bool FrontWasOn = false, BackWasOn = false;
    bool HasChanged = false;
    if (Delay.Elapsed(&STimer, 198)) {
        if (FrontIsOn() && !FrontWasOn) {
            FrontWasOn = true;
            HasChanged = true;
        }
        else if (!FrontIsOn()) {
            FrontWasOn = false;
            HasChanged = true;
        }

        if (BackIsOn() && !BackWasOn) {
            BackWasOn = true;
            HasChanged = true;
        }
        else if (!BackIsOn() && BackWasOn) {
            BackWasOn = false;
            HasChanged = true;
        }

        if (HasChanged) UpdateState();
    } // if delay
}


// =============================== CC handling =================================
void TxOn(void) {
    if (!IsOn) {
#ifdef LED_ENABLE
        Led.On();
#endif
        CC.Wake();
        IsOn = true;
    }
}
void TxOff(void) {
#ifdef LED_ENABLE
    Led.Off();
#endif
    CC.Shutdown();
    IsOn = false;
}

/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
enum WaitState_t {IsWaiting, IsReplying};
WaitState_t SearchState = IsWaiting;
uint8_t PktCounter=0;
#define PKTS_TO_REPLY   2

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
        if(++PktCounter == PKTS_TO_REPLY) SearchState = IsWaiting;
    }
}
