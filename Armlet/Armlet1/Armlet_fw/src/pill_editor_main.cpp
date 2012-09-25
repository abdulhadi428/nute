#include "stm32f10x.h"
#include <stdlib.h>
#include "kl_lib.h"
#include "kl_assert.h"
#include "lcd1200.h"
#include "beep.h"
#include "battery.h"
#include "i2c_mgr.h"
#include "i2c_ee.h"
#include "keys.h"

static void Init();

void PillEditorMain() {
    Init();

    Lcd.Printf(0, 3, "I'm a pill.");
    Lcd.Printf(0, 4, "Edit me!");

    while (1) {
    	Uart.Task();
    	Lcd.Task();

    	if (assertion_raised) continue;
		Battery.Task();
		Beep.Task();
		Keys.Task();
		i2cMgr.Task();
		Pill.Task();
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

    i2cMgr.Init();
    Pill.Init(INNER_EE_ADDR);

    Beep.Init();
    Beep.SetFreqHz(2007);
    Beep.Squeak(2, 4);
}
