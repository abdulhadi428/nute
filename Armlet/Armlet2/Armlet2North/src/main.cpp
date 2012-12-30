/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib_f205.h"

#include "stm32f2xx_usart.h"

void Init();

int main(void) {
    Init();

    PinSetupOut(GPIOB, 9, omPushPull);
    PinSet(GPIOB, 9);

    //Uart.Printf("abcdefghij\r");
    uint32_t Tmr, i=0;
    while(1) {
        //Uart.Task();
        if(Delay.Elapsed(&Tmr, 999)) {
            Uart.Printf("%02u\r", i++);
            Uart.Printf("%02u\r", i++);
        }
    }
}

inline void Init() {
    Delay.Init();
    Uart.Init(115200);
}


