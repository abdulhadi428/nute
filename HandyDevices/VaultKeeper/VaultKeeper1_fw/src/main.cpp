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
#include "kl_time.h"
#include "kl_string.h"
#include <string.h>
#include "sha1.h"
#include "adc.h"

#define VERSION_ID  "Minya"     // "First" on Quenya. Used for HostKey generation. No more than 20 char

#define RETRY_TIMEOUT   60  // s

LedBlink_t Led;
uint32_t HostKey[5];

// Prototypes
void GeneralInit(void);
void GenerateHostKey(void);

// ============================== Implementation ===============================
int main(void) {
    GeneralInit();


//    Mdm.On();
    //Mdm.SendSMS("+79169895800", "Aiya Feanaro!");
//    if (Mdm.GprsOn() == erOk) {
//        Srv.GetTime();
        //Mdm.GET("numenor2012.ru", "/request.php?time", Mdm.DataString, 53);
//        Mdm.POST("numenor2012.ru", "/request.php",
//                "host_id=19&water_value=1188&time=20120601202122&errors=0&host_hash=1bc29b36f623ba82aaf6724fd3b16718"
//                );

    uint32_t Tmr=0;
    Time.Bypass(&Tmr, RETRY_TIMEOUT);

    while (1) {
        Led.Task();

        // Get correct time if needed
        if (!Time.IsSet()) {
            if(Time.SecElapsed(&Tmr, RETRY_TIMEOUT)) { // every 5 minutes
                Mdm.On();
                if (Mdm.GprsOn() == erOk) {
                    Srv.GetTime();
                    Mdm.GprsOff();
                } // if GPRS
                Mdm.Off();
            } // if delay
        } // if is set
        else {
            if(Time.SecElapsed(&Tmr, 1)) {
                Led.Blink(45);
                Time.Print();
            }
        }

    } // while 1
}

inline void GeneralInit(void) {
    // Setup system clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    SystemCoreClockUpdate();
    klJtagDisable();

    Delay.Init();
    Delay.ms(63);
    Com.Init();
    Com.Printf("\rVault Keeper1\r");

    Led.Init(GPIOD, 2);
    //Led.On();
    Time.Init();
    Adc.Init();

    GenerateHostKey();
}


#define IDBASE      0x1FFFF7E8  // Address of ID
void GenerateHostKey(void) {
    char FBuf[45];
    // Get uinique CPU id
    uint32_t ID0 = *((uint16_t*)(IDBASE));
    uint32_t ID1 = *((uint16_t*)(IDBASE+2));
    uint32_t ID2 = *((uint32_t*)(IDBASE+4));
    uint32_t ID3 = *((uint32_t*)(IDBASE+8));
    klSPrintf(FBuf, "%S%X4%X4%X8%X8", VERSION_ID, ID0, ID1, ID2, ID3);
    //Com.Printf("ID: %S\r", FBuf);
    Sha1(FBuf);
    Com.Printf("HostKey: %S\r", Sha1String);
}

