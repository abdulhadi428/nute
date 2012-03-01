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
#include "acc_mma.h"
#include "kl_lib.h"

enum {txOff, txOn, txAuto} State;

// Variables
uint8_t ID = 1;
Led_t Led(GPIOA, 1);
Acc_t Acc;
bool IsOn;
// Pins of switchers
klPin_t SFront, SBack;
#define FRONT_IS_ON()  (SFront == false)
#define BACK_IS_ON()   (SBack  == false)

// Prototypes
void GeneralInit(void);
void Event_Trigger(void);
void Event_NoTrigger(void);
void TxOn(void);
void TxOff(void);

void SwitchersTask(void);
void SetStateBySwitchers(void);

// ============================== Implementation ===============================
int main(void) {
    //GeneralInit();

    RCC_HCLKConfig(RCC_SYSCLK_Div8);

    while (1) {
        Led.Toggle();
        Delay.ms(405);

        /*CC.Task();
        Acc.Task();
        i2cMgr.Task();
        SwitchersTask();
        */
    } // while 1
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();



    // Accelerometer
    Acc.Init();
    Acc.EvtTrigger = Event_Trigger;
    Acc.EvtNoTrigger = Event_NoTrigger;

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(ID);
    CC.Shutdown();
    IsOn = false;

    // Switchers
    State = txOff;
    SFront.Init(GPIOB, 1, GPIO_Mode_IPU);
    SBack.Init(GPIOB, 8, GPIO_Mode_IPU);

    // Setup initial state depending on switchers
    SetStateBySwitchers();

    klPrintf("\rTransmitter %u\r", ID);
}

void SetStateBySwitchers(void) {
    if (FRONT_IS_ON()) {
        if (BACK_IS_ON()) {
            State = txOn;
            TxOn();
        }
        else {
            State = txAuto;
            TxOff();
        }
    }
    else {
        State = txOff;
        TxOff();
    }
}

void SwitchersTask(void) {
    static uint32_t STimer;
    static bool FrontWasOn = false, BackWasOn = false;
    bool HasChanged = false;
    if (Delay.Elapsed(&STimer, 99)) {
        if (FRONT_IS_ON() && !FrontWasOn) {
            FrontWasOn = true;
            HasChanged = true;
        }
        else if (!FRONT_IS_ON() && FrontWasOn) {
            FrontWasOn = false;
            HasChanged = true;
        }

        if (BACK_IS_ON() && !BackWasOn) {
            BackWasOn = true;
            HasChanged = true;
        }
        else if (!BACK_IS_ON() && BackWasOn) {
            BackWasOn = false;
            HasChanged = true;
        }

        if (HasChanged) SetStateBySwitchers();
    } // if delay
}


// ======== Sensor ========
void Event_Trigger(void) {
    if (State == txAuto) TxOn();
}
void Event_NoTrigger(void) {
    if (State == txAuto) TxOff();
}

// =============================== CC handling =================================
void TxOn(void) {
    if (!IsOn) {
        Led.On();
        CC.Wake();
        IsOn = true;
    }
}
void TxOff(void) {
    if (IsOn) {
        Led.Off();
        CC.Shutdown();
        IsOn = false;
    }
}


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
