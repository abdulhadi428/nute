/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib_f0.h"
#include "stm32f0xx_usart.h"

void Init();

int main(void) {
    Init();
    Uart.Printf("klNFC\r");
    PinSetupOut(GPIOC, 8, poPushPull, pudNone);
    //PinSetupOut(GPIOC, 9, poPushPull, pudNone);
    PinSet(GPIOC, 8);

    uint32_t Tmr;
    while(1) {
        if(Delay.Elapsed(&Tmr, 198)) {
            PinToggle(GPIOC, 8);
            Uart.Printf("ege\r");
            //USART_SendData(USART1, 'e');
        }
        //Delay.ms(999);
        //PinToggle(GPIOC, 8);
    }
}

void Init() {
    Delay.Init();
    Uart.Init(115200);
}
