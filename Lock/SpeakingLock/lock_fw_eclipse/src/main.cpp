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
//#include "rock_action.h"
#include "lock_action.h"
#include "keyboard.h"
#include "kl_ini.h"

#include "lcd110x.h"
#include "images.h"
#include "usb_unit.h"

#include "uart.h"

void GeneralInit(void);

int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Let's rock!\r");

    GeneralInit();

    PrintFileToUART("lock.ini");
    WriteString("PASSWORD", "PASSLENGTH", "5", "lock.ini");
    PrintFileToUART("lock.ini");
    // ==== Main cycle ====
    while(1) {
        //i2cMgr.Task();
        ESnd.Task();
        //ESns.Task();
        EUSB.Task();
        //Leds.Task();
        ELock.Task();
        EKeyboard.Task();
        EIRSirc.Task();
        //Lcd.Task();
    } // while(1)
    return 0;
}



void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    // Init peripheral
    //i2cMgr.Init();
    Delay.Init();
    // Leds
    //Leds.Init();
    // Sensors
    //ESns.Init();
    EIRSirc.Init();

    // Sound
    SD.Init();
    Vs.Init();
    ESnd.Init();
    ELock.Init();
    EKeyboard.Init();
    EUSB.Init();


    // LCD
    //Lcd.Init();
    //Lcd.DrawImage(0,0, ImageLogo, NotInverted);    // Artifact
//    ERock.Init();

    // Read initial charge
//    ERock.H.Counter = ReadInt32("Init", "HCounter", "rock.ini");
//    ERock.H.Value   = ReadInt32("Init", "HValue",   "rock.ini");
//    ERock.E.Counter = ReadInt32("Init", "ECounter", "rock.ini");
//    ERock.E.Value   = ReadInt32("Init", "EValue",   "rock.ini");
//    ERock.C.Counter = ReadInt32("Init", "CCounter", "rock.ini");
//    ERock.C.Value   = ReadInt32("Init", "CValue",   "rock.ini");
//    ERock.ChooseType();

    ESnd.Play("magnet.wav");
}

// ================================== Events ===================================
void EVENT_KeyPressed(uint8_t chRow,uint8_t chCol)
{
 uint8_t chRes=0;

 chRes+=(chCol-1)*3;
 chRes+=chRow;
 if (chRes==11) chRes=0; // там ноль на 11-ом месте
 if (chRes==10) chRes='*';
 if (chRes==12) chRes='#';
 UART_PrintString("New key event. Press ");
 UART_PrintInt(chRes);
 UART_PrintString(" \r");
 if (ELock.KeyPressed(chRes)==1) UART_PrintString("New key event not accepted.\r");
}

/*******************************************************************************
* Function Name  : EVENT_USBDataRx.
* Description    : get the received data from USB.
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void EVENT_USBDataRx(uint8_t* data_buffer, uint8_t Nb_bytes)
{
  ELock.UsbRecData(data_buffer,Nb_bytes);
  UART_PrintString("Get USB data, ");
  UART_PrintInt(Nb_bytes);
  UART_PrintString(" bytes \r");
}

void EVENT_SensorsStateChanged(void) {
//    UART_PrintString("Sns: ");
//    UART_PrintArrAsHex((uint8_t*)&SnsState, sizeof(SnsState_t));
//    if(SnsState.KeyTouched[1]) ERock.TryToActivate(actOne);
//    if(SnsState.KeyTouched[2]) ERock.TryToActivate(actTwo);
//    //if(SnsState.MagnetNear) ERock.TryToActivate(actTwo);
//    if(SnsState.KeyTouched[0] && SnsState.KeyTouched[2]) ERock.TryToActivate(actThree);
}
