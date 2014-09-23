/*
 * main.cpp
 *
 *  Created on: 2014-04-14 ã.
 *      Author: Kreyl
 */

#include "ch.h"
#include "kl_lib_f072.h"
#include "cmd_uart.h"
#include "mpr121.h"

void OnCmdRx();

int main(void) {
    Clk.UpdateFreqValues();
    halInit();
    chSysInit();

    PinSetupOut(GPIOC, 6, omPushPull, pudNone);

    Uart.Init(115200);
    Uart.Printf("\rTorch4 AHB=%u", Clk.AHBFreqHz);
    Uart.OnCmdRx = OnCmdRx;

    Touch.Init();

    // Forever
    while(true) {
        chThdSleepMilliseconds(99);
        Uart.PollRx();
    }
}

void OnCmdRx() {
    Uart.Printf("CmdName: %S\r", Uart.PCmd->Name);
    if     (Uart.PCmd->NameIs("#On"))  PinSet(GPIOC, 6);
    else if(Uart.PCmd->NameIs("#Off")) PinClear(GPIOC, 6);
}
