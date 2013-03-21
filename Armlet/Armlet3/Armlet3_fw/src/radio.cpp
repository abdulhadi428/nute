/*
 * radio.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: g.kruglov
 */

#include "radio.h"
#include "cc1101.h"

// ==== Packet ====
struct Pkt_t {
    uint32_t IdArr[3];
    uint8_t RSSI;
    uint8_t LQI;
} PACKED;
#define CC_PKT_LEN  (sizeof(Pkt_t)-2)

#define RX


// =============================== Variables ===================================
Radio_t Radio;

Thread *PThread;
BinarySemaphore semIrq;

static RadioState_t IState;

Pkt_t PktTx, PktRx;

// ============================== Implementation ===============================
static WORKING_AREA(waRadioThread, 128);
static msg_t RadioThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Radio");

    PinSetupOut(GPIOB, 0, omPushPull, pudNone);

    PktTx.IdArr[0] = 0x12345678;
    PktTx.IdArr[1] = 0;
    PktTx.IdArr[2] = 0xDEADBEEF;

    chBSemInit(&semIrq, 0); // Not taken

    Uart.Printf("RX\r");

    while(1) {
//        switch(IState) {
//            case rIdle:
#ifdef RX
        PinSet(GPIOB, 0);
        CC.EnterRX();
        chBSemWait(&semIrq);
        PinClear(GPIOB, 0);
        uint8_t r = CC.ReadFifo((uint8_t*)&PktRx, sizeof(Pkt_t));
        if(r) {
            //Uart.Printf("RSSI: %d\r", PktRx.RSSI);
            Uart.Printf("RSSI: %d\r", CC.RSSI_dBm(PktRx.RSSI));
            //Uart.Printf("%A\r", (uint8_t*)&PktRx, sizeof(Pkt_t));
        }

        //PinClear(GPIOB, 0);
        chThdSleepMilliseconds(99);

#else

            PinSet(GPIOB, 0);
            CC.TransmitAndWaitIdle(&PktTx, CC_PKT_LEN);
            PinClear(GPIOB, 0);

            //Uart.Printf("t");
            chThdSleepMilliseconds(4);
#endif
//                break;
//
//            case rService:
//                break;
//        }
    } // while 1
    return 0;
}



void Radio_t::Init() {
    // Init variables
    IState = rIdle;
    ClearContacts();

    CC.Init();
    //CC.SetChannel(0);
    //CC.SetPower(PwrPlus12dBm);

    // ==== Create and start thread ====
    PThread = chThdCreateStatic(waRadioThread, sizeof(waRadioThread), HIGHPRIO, RadioThread, NULL);
}

// ============================= Interrupts ====================================
extern "C" {

CH_IRQ_HANDLER(EXTI4_IRQHandler) {
    CH_IRQ_PROLOGUE();
    EXTI->PR = (1 << 4);  // Clean irq flag

    //PinClear(GPIOB, 0);

    chBSemSignalI(&semIrq);

    CH_IRQ_EPILOGUE();
}

} // extern c
