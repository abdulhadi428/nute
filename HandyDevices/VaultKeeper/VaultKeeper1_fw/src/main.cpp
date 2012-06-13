/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib.h"
#include "led.h"
#include "sim900.h"
#include "comline.h"
#include "srvworks.h"

LedBlink_t Led;

// Prototypes
void GeneralInit(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();

    Mdm.On();
    // http://numenor2012.ru/request.php?time

    //Mdm.SendSMS("+79169895800", "Aiya Feanaro!");
    if (Mdm.GprsOn() == erOk) {
        Mdm.GET("numenor2012.ru", "/request.php?time", Mdm.DataString, 53);
        //Mdm.GET("numenor2012.ru/request.php?data=MyDaata");
        //Mdm.POST("numenor2012.ru", "/request.php", "data=aga3&value=ugu3");
//        Mdm.POST("numenor2012.ru", "/request.php",
//                "host_id=19&water_value=1188&time=20120601202122&errors=0&host_hash=1bc29b36f623ba82aaf6724fd3b16718"
//                );
        Mdm.GprsOff();
    }
    Mdm.Off();

    //uint32_t Tmr;

    while (1) {
        Led.Task();

        //if(Delay.Elapsed(&Tmr, 207)) Led.Blink(45);

    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);

    Led.Init(GPIOD, 2);
    //Led.On();

    Com.Init();
    Com.Printf("\rVault Keeper1\r");
}
