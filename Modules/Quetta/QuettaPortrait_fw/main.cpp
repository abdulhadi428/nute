/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f2xx.h"
#include "kl_sd.h"
#include "sound.h"
#include "cmd_uart.h"
#include "ff.h"
#include "MassStorage.h"
#include "evt_mask.h"

// External Power Input
#define PWR_EXTERNAL_GPIO   GPIOA
#define PWR_EXTERNAL_PIN    9
static inline bool ExternalPwrOn() { return  PinIsSet(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN); }

// External sensors
class Sns_t {
private:
    bool IsHi() const { return PinIsSet(PGpioPort, PinNumber); }
public:
    GPIO_TypeDef *PGpioPort;
    uint16_t PinNumber;
    bool WasHi;
    void Init() const { PinSetupIn(PGpioPort, PinNumber, pudPullDown); }
    RiseFall_t CheckEdge() {
        if(!WasHi and IsHi()) {
            WasHi = true;
            return Rising;
        }
        else if(WasHi and !IsHi()) {
            WasHi = false;
            return Falling;
        }
        else return NoRiseNoFall;
    }
};

Sns_t Sns = {GPIOA, 0};

// =============================== Main ========================================
int main() {
#if 1 // ==== Init ====
    // ==== Setup clock ====
    Clk.UpdateFreqValues();
    uint8_t ClkResult = FAILURE;
    Clk.SetupFlashLatency(12);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    Uart.Init(115200);
    SD.Init();
    // Read config
//    SD.iniReadInt32("Radio", "id", "settings.ini", &App.SelfID);
//    Uart.Printf("\rID=%u", App.SelfID);

    // USB related
    PinSetupIn(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN, pudPullDown);
    MassStorage.Init();

    Sound.Init();
    Sound.SetVolume(255);
    Sound.RegisterAppThd(chThdSelf());
    Sound.Play("alive.wav");

    // Sensor
    Sns.Init();

    Uart.Printf("\rPortrait   AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");
#endif

    // ==== Main cycle ====
    bool WasExternal = false;
    while(true) {
        chThdSleepMilliseconds(306);

#if 1 // ==== USB connected/disconnected ====
        if(WasExternal and !ExternalPwrOn()) {
            WasExternal = false;
            Usb.Shutdown();
            MassStorage.Reset();
            chSysLock();
            Clk.SetFreq12Mhz();
            Clk.InitSysTick();
            chSysUnlock();
            Uart.Printf("\rUsb Off");
        }
        else if(!WasExternal and ExternalPwrOn()) {
            WasExternal = true;
            chSysLock();
            Clk.SetFreq48Mhz();
            Clk.InitSysTick();
            chSysUnlock();
            Usb.Init();
            chThdSleepMilliseconds(540);
            Usb.Connect();
            Uart.Printf("\rUsb On");
        }
#endif

#if 1 // ==== Sensor ====
      if(Sns.CheckEdge() == Rising) {
          if(Sound.State == sndStopped) Sound.Play("alive.wav");
      }
#endif
    } // while true
}

