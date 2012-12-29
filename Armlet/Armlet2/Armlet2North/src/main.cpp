/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib_f205.h"

void Init();

int main(void) {
    Init();

    PinSetupOut(GPIOB, 9, omPushPull);
    PinSet(GPIOB, 9);

    Uart.Printf("Armlet2 North\r");
    while(1) {
    }
}

inline void Init() {
    //Delay.Init();
    Uart.Init(115200);
}


