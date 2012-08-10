/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "kl_lib.h"
#include "led.h"

LedRGB_t Led;

int main(void) {
    // ==== Init ====
    //InitClock(clk2MHzInternal);
    Delay.Init();
    Led.Init();
    //Uart.Init(115200);
    //Uart.Printf("\rTirilde\r");

    uint32_t Tmr;
    Color_t Cl = clBlack;

    // ==== Main cycle ====
    while (1) {
    	//Uart.Task();
        Led.Task();

        if(Delay.Elapsed(&Tmr, 4005)) {
        	if(Cl == clBlack) Cl = clBlue;
        	else Cl = clBlack;
        	Led.SetColorSmoothly(Cl);
        }

        // Sensor task
//        if(Delay.Elapsed(&SnsTmr, 360)) {
//            if ((Sns == 1) and (!SnsIsOn)) {
//                SnsIsOn = true;
//                if ((Led.State == lsOff) or (Led.State == lsDown)) Led.RampUp();
//                else Led.RampDown();
//            }
//            else if ((Sns == 0) and SnsIsOn) SnsIsOn = false;
//        }

     }
}

