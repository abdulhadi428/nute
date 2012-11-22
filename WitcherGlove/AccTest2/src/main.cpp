#include "stm32f10x.h"
#include <stdlib.h>
#include "kl_lib.h"
#include "led.h"
#include "lis3d.h"




static inline void Init();

int main() {
    Init();

    //uint32_t Tmr;
    while(1) {
        Uart.Task();
        Acc.Task();

        //if(Delay.Elapsed(&Tmr, 990)) Uart.Printf("1 ");



    } // while(1)
}

static void Init() {
    klJtagDisable();
    InitClock(clk8MHzInternal);

    Delay.Init();
    Uart.Init(256000);
    Uart.Printf("\rAcc test2\r");

    Acc.Init();
}


