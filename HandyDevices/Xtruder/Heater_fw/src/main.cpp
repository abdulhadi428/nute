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

#define PWM_TICK    99  // ms
#define PWM_TOP     10
class Outputs_t {
private:
    klPin_t IPin[4];
public:
    uint8_t Pwm[4];    // 0...10
    void Task(void);
    void Init(void);
} Outputs;

// Prototypes
void GeneralInit(void);
void RegulationTask(void);
uint8_t Regulator(int32_t Err, uint8_t Y);

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
        Outputs.Task();
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

    Outputs.Init();

    Uart.Printf("\rHeater\r");
}

inline void RegulationTask() {
    static uint32_t t=0;
    static uint16_t Tmr;
    if(Delay.Elapsed(&Tmr, 100)) t++;

    if(Ads.NewData) {
        Ads.NewData = false;
        // Display current temperatures
        for(uint8_t i=0; i<ADS_CH_COUNT; i++) Interface.DisplayT(i+1, Ads.Temperature[i]);
        //Uart.Printf("%u;%i\r", t, Ads.Temperature[0]);

        // Regulate them
        Outputs.Pwm[0] = Regulator(Interface.tToSet[0] - Ads.Temperature[0], 6);
        Outputs.Pwm[1] = Regulator(Interface.tToSet[1] - Ads.Temperature[1], 7);
    }
}

#define kp  ((int32_t)(0x10000 * 4.0e-1))
#define ki  ((int32_t)(0x10000 * 4.0e-4))

uint8_t Regulator(int32_t Err, uint8_t Y) {
    // Gain
    int32_t Gain = Err * kp;
    // Integrator
    static int32_t IntAcc = 0;
    IntAcc += ki * Err;

    int32_t r = ((Gain + IntAcc) / 0x10000);

    Uart.Printf("x: %i; Gain: %i; IntAcc: %i; Pwm: %i\r", Err, Gain, IntAcc, r);

    if(r<0) r=0;
    else if(r > PWM_TOP) r = PWM_TOP;
    Lcd.Printf(0, Y, "Err=%i; r=%u  ", Err, r);
    return r;
}

// ================================== Outputs ==================================
void Outputs_t::Init() {
    IPin[0].Init(GPIOB, 1, GPIO_Mode_Out_PP);
    IPin[1].Init(GPIOB, 2, GPIO_Mode_Out_PP);
    IPin[2].Init(GPIOB, 3, GPIO_Mode_Out_PP);
    IPin[3].Init(GPIOB, 4, GPIO_Mode_Out_PP);

    for(uint8_t i=0; i<4; i++) Pwm[i] = 0;
}

void Outputs_t::Task() {
    static uint16_t ITimer;
    static uint8_t ICounter=0;
    if(Delay.Elapsed(&ITimer, PWM_TICK)) {
        if(ICounter == PWM_TOP) ICounter = 0;
        // Compare PWM
        for(uint8_t i=0; i<4; i++) {
            if(ICounter < Pwm[i]) IPin[i] = 1;
            else IPin[i] = 0;
        }
        ICounter++;
    }
}
