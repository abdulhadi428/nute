/*
 * File:   main.cpp of Heater project
 * Author: Kreyl
 *
 * Created on Aug 21, 2012, 0:12
 */

#include "kl_lib.h"
#include "lcd1200.h"
#include "ads124x.h"
#include "keys.h"
#include "beep.h"
#include "interface.h"
#include "i2c_mgr.h"
#include "i2c_ee.h"

#define OUTPUT_SWITCH_MIN_DELAY     999 // ms
class Output_t : public klPin_t {
private:
    uint16_t ITimer;
public:
    void On(void)  { if(Delay.Elapsed(&ITimer, OUTPUT_SWITCH_MIN_DELAY)) Set();   }
    void Off(void) { if(Delay.Elapsed(&ITimer, OUTPUT_SWITCH_MIN_DELAY)) Clear(); }
};
Output_t Out[4];

// Prototypes
void GeneralInit(void);
void RegulationTask(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    // ==== Main cycle ====
    while (1) {
        Uart.Task();
        Lcd.Task();
        Beep.Task();
        Keys.Task();
        Interface.Task();
        RegulationTask();
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
    Keys.Init();
    Beep.Init();
    Beep.SetFreqHz(2007);
    Beep.Squeak(2, 4);
    Lcd.Init();
    Lcd.BacklightOn();
    // Draw logo
//    Lcd.DrawImage(0, 0, ImgLogo);
//    for(uint16_t i=0; i<999; i++) Lcd.Task();
//    Delay.ms(3006);
//    Lcd.Cls();
    Interface.Init();
    i2cMgr.Init();
    ee.Init();

    Out[0].Init(GPIOB, 1, GPIO_Mode_Out_PP);

    Uart.Printf("\rHeater\r");
}

inline void RegulationTask() {
    if(Ads.NewData) {
        Ads.NewData = false;
        // Display current temperatures
        for(uint8_t i=0; i<ADS_CH_COUNT; i++) Interface.DisplayT(i+1, Ads.Temperature[i]);
        // Regulate them
        if(Ads.Temperature[0] < Interface.tToSet[0]) Out[0].On();
        else Out[0].Off();
    }
}
