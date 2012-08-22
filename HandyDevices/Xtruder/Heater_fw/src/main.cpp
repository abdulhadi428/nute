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

    //uint32_t Tmr;
    //uint8_t FData[16];
    // ==== Main cycle ====
    while (1) {
        Uart.Task();
        Lcd.Task();
        //if(Delay.Elapsed(&Tmr, 207)) {
            if(Ads.NewData) {
                Ads.NewData = false;
                Uart.Printf("%u\r", Ads.Value);
                int32_t t = Ads.Value / AdsChannels[0].a + AdsChannels[0].b;
                Lcd.Printf(0, 4, "t: %i  ", t);
                Lcd.Printf(0, 5, "u: %u  ", Ads.Value);
                Lcd.Printf(0, 6, "/: %u  ", Ads.Value/10000);
            }
            //Ads.Read(ADS_REG_SETUP, 15, FData);
            //Uart.Printf("> %A\r", FData, 15);
            //Ads.ReadResult(FData);
            //Uart.Printf("> %A\r", FData, 3);
        //}
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
