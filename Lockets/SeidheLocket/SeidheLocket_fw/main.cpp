/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_L15x.h"
#include "ch.h"
#include "hal.h"
#include "clocking_L1xx.h"
#include "cmd_uart.h"
#include "cc1101defins.h"
#include "rlvl1_defins.h"
#include "cc1101.h"

// Button
#define BTN_GPIO        GPIOB
#define BTN_PIN         8
#define BtnPressed()    (!PinIsSet(BTN_GPIO, BTN_PIN))
// Motor
#define VIBRO_GPIO      GPIOB
#define VIBRO_PIN       6
#define VibroOn()       PinSet(VIBRO_GPIO, VIBRO_PIN)
#define VibroOff()      PinClear(VIBRO_GPIO, VIBRO_PIN)

// rpkt
#define MAGIC_NUMBER    (uint32_t)0xCA110FE5    // Call of Elwen Star
const rPkt_t rPktTx = { MAGIC_NUMBER };
rPkt_t rPktRx;

int main(void) {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V2);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    Uart.Init(115200);
    Uart.Printf("\rSeidheLocket    AHB=%u", Clk.AHBFreqHz);

    // Init radioIC
    CC.Init();
    CC.SetTxPower(CC_Pwr0dBm);
    CC.SetPktSize(RPKT_LEN);
    CC.SetChannel(9);

    // Init button & vibro
    PinSetupIn(BTN_GPIO, BTN_PIN, pudPullUp);
    PinSetupOut(VIBRO_GPIO, VIBRO_PIN, omPushPull, pudNone);

    // BrrBrr at start
    VibroOn();
    chThdSleepMilliseconds(99);
    VibroOff();
    chThdSleepMilliseconds(45);
    VibroOn();
    chThdSleepMilliseconds(99);
    VibroOff();
    chThdSleepMilliseconds(45);

    int8_t Rssi;
    while(true) {
        // Transmit if btn pressed, Receive if not
        if(BtnPressed()) CC.TransmitSync((void*)&rPktTx);
        else {
            // Try to receive
            uint8_t RxRslt = CC.ReceiveSync(99, &rPktRx, &Rssi);
            if((RxRslt == OK) and (rPktRx.MagicNumber == MAGIC_NUMBER)) {
                VibroOn();
                Uart.Printf("\r%d", Rssi);
            }
            else VibroOff();
        }
    } // while true
}
