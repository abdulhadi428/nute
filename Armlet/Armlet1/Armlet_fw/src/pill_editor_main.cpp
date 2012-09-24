#include "stm32f10x.h"
#include <stdlib.h>
#include "kl_lib.h"
#include "kl_assert.h"
#include "led.h"
#include "rf_comm.h"
#include "lcd1200.h"
#include "beep.h"
#include "battery.h"
#include "i2c_mgr.h"
#include "i2c_ee.h"
#include "keys.h"
#include "vibro.h"


static void Init();

void PillEditorMain() {
    Init();

    Lcd.Printf(0, 3, "I'm a pill.");
    Lcd.Printf(0, 4, "Edit me!");

    while (1) {
    	Uart.Task();
    	Lcd.Task();
        Vibro.Task();

    	if (assertion_raised)
    		continue;
		Battery.Task();
		Beep.Task();
		Keys.Task();
		Radio.Task();
		i2cMgr.Task();
		Pill.Task();

		// logic goes here (perhaps in a separate task-like function, whatever)
    }
}

static inline void Init() {
    InitClock(clk2MHzInternal);
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
    Uart.Printf("\rPill editor\r");
    Lcd.Init();
    Lcd.Backlight(0);

    Keys.Init();

    Battery.Init();
    Radio.Init();

    i2cMgr.Init();
    Pill.Init(INNER_EE_ADDR);

    Beep.Init();
    Beep.SetFreqHz(2007);
    Beep.Squeak(2, 4);

    Vibro.Init();
    Vibro.Flinch(2);

    // Get unique CPU ID and place it into packet to transmit
    GetUniqueID(&Radio.PktTx.IdArr[0]);
    Uart.Printf("ID: %08X %08X %08X\r", Radio.PktTx.IdArr[0], Radio.PktTx.IdArr[1], Radio.PktTx.IdArr[2]);
}
