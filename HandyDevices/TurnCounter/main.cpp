/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_f100.h"
#include "ch.h"
#include "hal.h"
#include "cmd_uart_f10x.h"
#include "lcd1200.h"
#include "beep.h"
#include "sequences.h"

#define Key1IsPressed()  (!PinIsSet(GPIOB, 3))
#define Key2IsPressed()  (!PinIsSet(GPIOB, 4))
#define SnsIsOn()       (PinIsSet(GPIOB, 5))

int main(void) {
    // ==== Init clock system ====
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();
    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    JtagDisable();
    Uart.Init(115200);
    Uart.Printf("\rTurnCounter  AHB=%u; APB1=%u; APB2=%u\r\n", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    Lcd.Init();
    Lcd.Backlight(0);
//    Lcd.Printf(0, 0, "Aiya!");

    Beeper.Init();
    Beeper.Beep(BeepBeep);

    // Keys
    PinSetupIn(GPIOB, 3, pudPullUp);
    PinSetupIn(GPIOB, 4, pudPullUp);
    bool Key1WasPressed = false;
    bool Key2WasPressed = false, BckltIsOn = false;

    // TurnCounter
    uint32_t Counter = 0;
    Lcd.PrintfFont(Times_New_Roman18x16, 36, 4, "%u     ", Counter);
    bool SnsWasOn = false;
    PinSetupIn(GPIOB, 5, pudPullDown);

    // ==== Main cycle ====
    while(true) {
        chThdSleepMilliseconds(9);
        // ==== Keys ====
        // Key1
        if(Key1IsPressed() and !Key1WasPressed) {
            Key1WasPressed = true;
            Uart.Printf("\rReset");
            Beeper.Beep(BeepBeep);
            Counter = 0;
            Lcd.PrintfFont(Times_New_Roman18x16, 36, 4, "%u     ", Counter);
        }
        else if(!Key1IsPressed() and Key1WasPressed) {
            Key1WasPressed = false;
        }

        // Key2
        if(Key2IsPressed() and !Key2WasPressed) {
            Key2WasPressed = true;
            BckltIsOn = !BckltIsOn;
            if(BckltIsOn) Lcd.Backlight(54);
            else Lcd.Backlight(0);
            Beeper.Beep(BeepKeyOk);
        }
        else if(!Key2IsPressed() and Key2WasPressed) {
            Key2WasPressed = false;
        }

        // ==== Sensor ====
        if(SnsIsOn() and !SnsWasOn) {
            SnsWasOn = true;
            Beeper.Beep(BeepShort);
            Counter++;
            Lcd.PrintfFont(Times_New_Roman18x16, 36, 4, "%u     ", Counter);
        }
        else if(!SnsIsOn() and SnsWasOn) {
            SnsWasOn = false;
        }
    }
}
