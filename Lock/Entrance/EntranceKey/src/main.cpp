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

// Variables
uint8_t ID = 6;
Acc_t Acc;
bool IsOn;

LedBlink_t Led;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    uint32_t Tmr;
    while (1) {
        Led.Task();
        CC.Task();
        Acc.Task();
        i2cMgr.Task();
        Uart.Task();
        Battery.Task();

        // Indication
        if(IsOn) {
            if(Delay.Elapsed(&Tmr, 207)) Led.Blink(45);
        }
        else if(Battery.State == bsEmpty) {
            if(Delay.Elapsed(&Tmr, 1800)) Led.Blink(99);
        }
    } // while 1
}

void GeneralInit(void) {
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
    i2cMgr.Init();

    // Accelerometer
    Acc.Init();
    Acc.Delay_ms = 4005;
    Acc.Enable();

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(ID);
    CC.Shutdown();
    IsOn = false;

    Battery.Init();

    Uart.Printf("\rTransmitter %u\r", ID);
    // Demonstrate switching on
    Led.Init(GPIOA, 1);
    Led.Blink(702);
}

/*
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

*/
// ================================ Events =====================================
void Acc_t::EvtTrigger(void) {
    if (!IsOn) {
        RCC_HCLKConfig(RCC_SYSCLK_Div2);
        SystemCoreClock = 4000000;
        Delay.Init();

        CC.Wake();
        IsOn = true;
    }
}
void Acc_t::EvtNoTrigger(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div16);
    SystemCoreClock = 500000;
    Delay.Init();

    CC.Shutdown();
    IsOn = false;
}

// =============================== CC handling =================================
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
            Uart.Printf("RX ovf\r");
            FlushRxFIFO();
            break;
        case CC_STB_TX_UNDF:
            Uart.Printf("TX undf\r");
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
