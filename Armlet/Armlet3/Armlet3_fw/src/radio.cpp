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

//#define RX

#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    6
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1);
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1);
#endif


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

#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif

    PktTx.IdArr[0] = 0x12345678;
    PktTx.IdArr[1] = 0;
    PktTx.IdArr[2] = 0xDEADBEEF;

    chBSemInit(&semIrq, 0); // IRQ semaphore, Not taken

    //Uart.Printf("RX\r");

    while(1) {
//        switch(IState) {
//            case rIdle:
#ifdef RX
        DBG1_SET();
        CC.EnterRX();
        chBSemWait(&semIrq);
        DBG1_CLR();
        uint8_t r = CC.ReadFifo((uint8_t*)&PktRx, sizeof(Pkt_t));
        if(r) {
            //Uart.Printf("RSSI: %d\r", PktRx.RSSI);
            Uart.Printf("RSSI: %d\r", CC.RSSI_dBm(PktRx.RSSI));
            //Uart.Printf("%A\r", (uint8_t*)&PktRx, sizeof(Pkt_t));
        }

        //PinClear(GPIOB, 0);
        //chThdSleepMilliseconds(99);

#else

        DBG1_SET();
        CC.TransmitAndWaitIdle(&PktTx, CC_PKT_LEN);
        DBG1_CLR();

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
