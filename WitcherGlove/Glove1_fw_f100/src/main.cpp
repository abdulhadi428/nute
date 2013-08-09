/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f100.h"
#include "acc.h"

#include "application.h"
#include "ManyLed.h"
#include "vibro.h"

// TODO: Charge level indication

//#define TESTING

static inline void Init();
static void SwitchOffEverything();

#ifndef TESTING
extern IWDG_t Iwdg;
static void Indication();
// Pins
#define EXT_PWR_GPIO    GPIOA
#define EXT_PWR_PIN     9
#define CHARGING_GPIO   GPIOB
#define CHARGING_PIN    2
// ExtPwr & Charging
static inline bool IsExtPwrOn()   { return  PinIsSet(EXT_PWR_GPIO, EXT_PWR_PIN); }
static inline bool IsCharging() { return !PinIsSet(CHARGING_GPIO, CHARGING_PIN); }
#endif

// Application entry point.
int main() {
    // ==== Setup clock ====
    // 8/1 = 8 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();

    // Init OS
    halInit();
    chSysInit();
    Init();

    bool ExtPwrOn = false;

    while(TRUE) {
#ifndef TESTING
        Indication();
        if(IsExtPwrOn()) {
            // Detect power change
            if(!ExtPwrOn) {
                Uart.Printf("ExtPwrOn\r");
                ExtPwrOn = true;
                App.Enabled = false;
                SwitchOffEverything();
                // Lower CPU freq
                chSysLock();
                Clk.SetupBusDividers(ahbDiv8, apbDiv1, apbDiv1);
                Clk.UpdateFreqValues();
                Clk.UpdateSysTimer();
                chSysUnlock();
            }
            App.Status = IsCharging()? stCharging : stChargeCompleted;
        }
        else {
            if(ExtPwrOn) {
                Uart.Printf("ExtPwrOff\r");
                SwitchOffEverything();
                ExtPwrOn = false;
                // Increase CPU freq
                chSysLock();
                Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
                Clk.UpdateFreqValues();
                Clk.UpdateSysTimer();
                chSysUnlock();
                App.Status = stIdle;
                App.Enabled = true;
            }
        }
#else
        chThdSleepMilliseconds(999);
        if(!Acc[0].IsOperational) {
            Uart.Printf("Acc fail\r");
            Acc[0].Init();
        }
        else {
            Acc[0].ReadAccelerations();
            Uart.Printf("%d %d %d\r", Acc[0].a[0], Acc[0].a[1], Acc[0].a[2]);
        }
#endif
    } // while 1
}

// ===================================== Indication ============================
#define VIBRO_SOURCE    10
#define VIBRO_FORCE     50
#define VIBRO_EXEC      100
#define VIBRO_HOLD      VIBRO_SOURCE

#define LED_ON_DURATION         72
#define LED_OFF_DURATION        72

static inline void Blink(PwmPin_t *PLed) {
    PLed->On(LED_TOP);
    chThdSleepMilliseconds(LED_ON_DURATION);
    PLed->Off();
    chThdSleepMilliseconds(LED_OFF_DURATION);
}

static void Exec(PwmPin_t *PLed) {
    Vibro.On(VIBRO_EXEC);
    Blink(PLed);
    Blink(PLed);
    Vibro.Off();
}
static void Hold(PwmPin_t *PLed) {
    Blink(PLed);
}

void SwitchOffEverything() {
    GreenLed.Off();
    RedLed.Off();
    YellowLed.Off();
    BlueLed.Off();
    WhiteLed.Off();
    Vibro.Off();
}

void Indication() {
    switch(App.Status) {
        case stIdle:
        case stSignReset:
            SwitchOffEverything();
            chThdSleepMilliseconds(9);
        break;

        case stAardSrc:
        case stIgniSrc:
        case stKvenSrc:
        case stIrdenSrc:
        case stGelioSrc:
            SwitchOffEverything();
            Vibro.On(VIBRO_SOURCE);
            chThdSleepMilliseconds(9);
        break;

        case stAardForce:
        case stIgniForce:
        case stKvenForce:
        case stIrdenForce:
        case stGelioForce:
            SwitchOffEverything();
            Vibro.On(VIBRO_FORCE);
            chThdSleepMilliseconds(9);
        break;

        // Aard
        case stAardExec: Exec(&BlueLed); App.Status = stAardHold; break;
        case stAardHold: Hold(&BlueLed); break;
        // Igni
        case stIgniExec: Exec(&RedLed); App.Status = stIgniHold; break;
        case stIgniHold: Hold(&RedLed); break;
        // Kven
        case stKvenExec: Exec(&YellowLed); App.Status = stKvenHold; break;
        case stKvenHold: Hold(&YellowLed); break;
        // Irden
        case stIrdenExec: Exec(&GreenLed); App.Status = stIrdenHold; break;
        case stIrdenHold: Hold(&GreenLed); break;
        // Gelio
        case stGelioExec: Exec(&WhiteLed); App.Status = stGelioHold; break;
        case stGelioHold: Hold(&WhiteLed); break;

        // ==== Device state ====
        case stAccFail:
            RedLed.On(LED_LOW);
            YellowLed.On(LED_LOW);
            chThdSleepMilliseconds(450);
            break;

        case stCharging:
            GreenLed.Off();
            BlueLed.On(LED_LOW);
            chThdSleepMilliseconds(99);
            BlueLed.Off();
            chThdSleepMilliseconds(810);
            break;

        case stChargeCompleted:
            BlueLed.Off();
            GreenLed.On(LED_LOW);
            chThdSleepMilliseconds(450);
            break;
    } // switch
}

// =============================== Init ========================================
void Init() {
    JtagDisable();
    Uart.Init(USART2, 256000);
#ifndef TESTING
    // Charging and powering
    PinSetupIn(EXT_PWR_GPIO,  EXT_PWR_PIN,  pudPullDown);
    PinSetupIn(CHARGING_GPIO, CHARGING_PIN, pudPullUp);

    LedsInit();
    VibroInit();
    if(!Iwdg.ResetOccured()) {
        Uart.Printf("Glove1_f100 AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
        Vibro.On(100);
        chThdSleepMilliseconds(999);
        Vibro.Off();
    }
    else Uart.Printf("W\r");
    AccInit();

    // Application init
    App.Init();
#else
    Uart.Printf("Glove tester\r");
    PinSetupOut(GPIOA, 15, omPushPull);
    PinSet(GPIOA, 15);
    chThdSleepMilliseconds(450);
    Acc[0].SetPortAndPins(GPIOA, 1, 3, 4);
#endif
}
