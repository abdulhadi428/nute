/*
 * File:   main.cpp of Heater project
 * Author: Kreyl
 *
 * Created on Aug 21, 2012, 0:12
 */

#include "kl_lib.h"
#include "lcd1200.h"
#include "ads124x.h"

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    // TODO: change linker script to C8
    GeneralInit();

    // ==== Main cycle ====
    while (1) {
        Lcd.Task();
    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);

    Uart.Init(115200);
    Ads.Init();
    Lcd.Init();
    //Lcd.BacklightOn();
    // Draw logo
//    Lcd.DrawImage(0, 0, ImgLogo);
//    for(uint16_t i=0; i<999; i++) Lcd.Task();
//    Delay.ms(3006);
//    Lcd.Cls();

    Uart.Printf("\rHeater\r");
}
