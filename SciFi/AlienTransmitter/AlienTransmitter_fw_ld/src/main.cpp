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

uint16_t ID;

void GeneralInit(void);

int main(void) {
    GeneralInit();

    while (1) {
        CC.Task();
    }
}

void GeneralInit(void) {
    // Get self ID: set by pins
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    Delay.ms(1);
    // Read ID
    ID = GPIO_ReadInputData(GPIOB);
    ID &= 0x07; // clear all unneeded
    if (ID > 6) ID = 0;
    // Deinit pins
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);

    // Proceed with init
    Delay.Init();
    Delay.ms(63);
    UART_Init();

    // Setup CC
    CC.Init();
    CC.TX_Pkt.From = ID;
    CC.SetChannel(CC_CHNL);
    CC.SetAddress(ID);

    klPrintf("\rTransmitter %u\r", ID);
}

// =============================== CC handling =================================
/*
 * Both TX and RX are interrupt-driven, so IRQ enabled at init and commented out in EnterRX.
 */
typedef enum {IsWaiting, IsReplying} WaitState_t;
WaitState_t SearchState = IsWaiting;
uint8_t PktCounter=0;

void CC_t::Task(void) {
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
