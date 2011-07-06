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
#include "cc1101.h"
#include "acc_mma.h"
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

        CC.Task();
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
    CC.Init();
    // Leds
    Leds.Init();

    // Sensors
    ESns.Init();
    EIRSirc.Init();
    // LCD
    Lcd.Init();
    Lcd.DrawImage(0,0, ImageLogo, NotInverted);
    // Sound
    SD.Init();
    Vs.Init();
    ESnd.Init();
    // Radio
    CC.Init();
    CC.Addr = (uint8_t) ReadInt32("Radio", "Channel", "Rock.ini");
    UART_StrInt("Radio channel: ", CC.Addr);
    CC.TX_Pkt.SenderAddr = (uint8_t)CC.Addr;
    CC.TimebaseInit();
    // Artifact
    ERock.Init();

    ESnd.Play("alive.wav");

    ERock.Type = atKusok;
    ERock.ChargeCount = 1;
}

// ================================== Events ===================================
// RF: new packet received
void EVENT_NewPacket(void) {
    if (CC.RX_Pkt.CommandID == 0x7C) {
        // Count this one if did not do it yet
        OthersIDs[CC.RX_Pkt.SenderAddr] = (uint8_t)CC.RX_Pkt.ArtType;
        // Adjust our timer if sender address is lower, and do not otherwise
        if (CC.RX_Pkt.SenderAddr < CC.Addr) {
            CC.TimerStop();
            CC.CycleCounter = CC.RX_Pkt.SenderCycle;
            TIM_SetCounter(TIM5, (CC.RX_Pkt.SenderTime-20));
            CC.TimerStart();
        } // if addr
    }
}

void EVENT_SensorsStateChanged(void) {
    UART_PrintString("Sns: ");
    UART_PrintArrAsHex((uint8_t*)&SnsState, sizeof(SnsState_t));
    if(SnsState.KeyTouched[0] && SnsState.KeyTouched[1] && SnsState.KeyTouched[2]) ERock.TryToActivate(actOne);
    if(SnsState.MagnetNear) ERock.TryToActivate(actTwo);

//    if(SnsState.KeyTouched[0]) ERock.TryToActivate(actOne);
//    if(SnsState.KeyTouched[1]) ERock.TryToActivate(actTwo);
//    if(SnsState.KeyTouched[2]) ERock.TryToActivate(actThree);

}
