/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "i2c_mgr.h"
#include "leds_pca.h"
#include "delay_util.h"
#include "sd.h"
#include "vs.h"
#include "media.h"
#include "sensors.h"
#include "rock_action.h"
#include "kl_ini.h"

#include "lcd110x.h"
#include "images.h"

#include "uart.h"

void GeneralInit(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Let's rock!\r");

    GeneralInit();

    // ==== Main cycle ====
    while(1) {
        i2cMgr.Task();
        ESnd.Task();
        ESns.Task();
        Leds.Task();
        ERock.Task();
        EIRSirc.Task();
        Lcd.Task();
    } // while(1)
    return 0;
}

void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    // Init peripheral
    i2cMgr.Init();
    Delay.Init();
    // Leds
    Leds.Init();
    // Sensors
    ESns.Init();
    EIRSirc.Init();

    // Sound
    SD.Init();
    Vs.Init();
    ESnd.Init();
    // LCD
    Lcd.Init();
    Lcd.DrawImage(0,0, ImageLogo, NotInverted);    // Artifact
    ERock.Init();

    // Read initial charge
    ERock.H.Counter = ReadInt32("Init", "HCounter", "rock.ini");
    ERock.H.Value   = ReadInt32("Init", "HValue",   "rock.ini");
    ERock.E.Counter = ReadInt32("Init", "ECounter", "rock.ini");
    ERock.E.Value   = ReadInt32("Init", "EValue",   "rock.ini");
    ERock.C.Counter = ReadInt32("Init", "CCounter", "rock.ini");
    ERock.C.Value   = ReadInt32("Init", "CValue",   "rock.ini");
    ERock.ChooseType();

    ESnd.Play("alive.wav");
}

// ================================== Events ===================================
void EVENT_SensorsStateChanged(void) {
    UART_PrintString("Sns: ");
    UART_PrintArrAsHex((uint8_t*)&SnsState, sizeof(SnsState_t));
    if(SnsState.KeyTouched[1]) ERock.TryToActivate(actOne);
    if(SnsState.KeyTouched[2]) ERock.TryToActivate(actTwo);
    //if(SnsState.MagnetNear) ERock.TryToActivate(actTwo);
    if(SnsState.KeyTouched[0] && SnsState.KeyTouched[2]) ERock.TryToActivate(actThree);
}
