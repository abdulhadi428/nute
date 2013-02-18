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


// =============================== Variables ===================================
Radio_t Radio;
static RadioState_t IState;

Pkt_t PktTx;

// ============================== Implementation ===============================
static WORKING_AREA(waRadioThread, 128);
static msg_t RadioThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Radio");
    while(1) {
        switch(IState) {
            case rIdle:
                CC.TransmitAndWaitIdle(&PktTx, CC_PKT_LEN);
                Uart.Printf("t");
                chThdSleepMilliseconds(450);
                break;

            case rService:
                break;
        }
    } // while 1
    return 0;
}



void Radio_t::Init() {
    // Init variables
    IState = rIdle;
    ClearContacts();

    CC.Init();
    CC.SetChannel(0);
    CC.SetPower(Pwr0dBm);

    // ==== Create and start thread ====
    chThdCreateStatic(waRadioThread, sizeof(waRadioThread), NORMALPRIO, RadioThread, NULL);
}


