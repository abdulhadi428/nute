#include "stm32f10x.h"
#include <stdlib.h>
#include "kl_lib.h"
#include "led.h"
#include "lis3d.h"




static void Init();

int main() {
    Init();

    uint32_t Tmr;
    while(1) {
        Uart.Task();
        if(Delay.Elapsed(&Tmr, 99)) {
            Acc.Read();
//            uint16_t tmp = Acc.ReadReg(OUT_X_H);
//            tmp <<= 8;
//            tmp |= Acc.ReadReg(OUT_X_L);
//            int16_t a = (int16_t)tmp;
//            //Uart.Printf("X: %d; Y: %d; Z: %d\r", );
//            Uart.Printf("tmp: %04X; a: %d\r", tmp, a);
        }
    }
}

static void Init() {
    klJtagDisable();
    InitClock(clk8MHzInternal);

    Delay.Init();
    Uart.Init(115200);
    Uart.Printf("\rAcc test2\r");

    Acc.Init();
}


