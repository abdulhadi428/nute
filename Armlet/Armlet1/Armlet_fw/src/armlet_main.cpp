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


const int LIGHT_FULL = 3;
const int LIGHT_FADE = 3;
const int LIGHT_INTENSITY = 20;

int light = 0;

void Lighten() {
	light = 10*(LIGHT_FULL+LIGHT_FADE);
}

void BacklightTask() {
	static uint32_t t=0;
	if (Keys.AnyKeyWasJustPressed())
		Lighten();
	if (!Delay.Elapsed(&t, 100))
		return;
	if (light > 0)
		light--;
	if (light > 10*LIGHT_FADE)
		Lcd.Backlight(LIGHT_INTENSITY);
	else
		Lcd.Backlight(LIGHT_INTENSITY*light/(LIGHT_FADE*10));
}


enum UIState_t {
	IDLE,
	MENU
};

UIState_t state = IDLE;


const char *menu_items[] = {
	"Отмена меню",
	"руку/ногу плазмо               й", // :(
	"корпус плазмой",
	"руку/ногу прочим",
	"корпус прочим",
};
int menu_index = 0;
const int TIME_IN_MENU = 35; // in tenths of a second
int time_in_menu;

void DrawMenu() {
	Lcd.Printf(0, 2, "Ранило...");
	for (unsigned int i = 0; i < countof(menu_items); i++) {
		if (i == menu_index)
			Lcd.SetDrawMode(OVERWRITE_INVERTED);
		else
			Lcd.SetDrawMode(OVERWRITE);
		Lcd.Printf(0, i==0 ? 1 : i+2, "%s", menu_items[i]);
	}
	Lcd.SetDrawMode(OVERWRITE);
}

void Task() {
	switch (state) {
	case IDLE:
		if (Keys.Up.WasJustPressed())
			;
		if (Keys.Down.WasJustPressed())
			;
		if (Keys.Enter.WasJustPressed()) {
			menu_index = 0;
			time_in_menu = TIME_IN_MENU;
			state = MENU;
			Lcd.Cls();
			DrawMenu();
		}
		break;
	case MENU:
		if (Keys.Up.WasJustPressed()) {
			menu_index--;
			if (menu_index < 0)
				menu_index = 0;
			time_in_menu = TIME_IN_MENU;
			DrawMenu();
		}
		if (Keys.Down.WasJustPressed()) {
			menu_index++;
			if (menu_index >= (int)countof(menu_items))
				menu_index = (int)countof(menu_items)-1;
			time_in_menu = TIME_IN_MENU;
			DrawMenu();
		}
		time_in_menu--;
		if (time_in_menu == 0 || Keys.Enter.WasJustPressed()) {
			if (time_in_menu == 0)
				menu_index = 0;
			if (menu_index > 0)
				Vibro.Flinch(1);
			state = IDLE;
			Lcd.Cls();
		}
		break;
	}
}


static void Init();

void ArmletMain() {
	static uint32_t t = 0;
    Init();

    Lcd.Printf(0, 1, "Браслет");

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
		BacklightTask();

		if (Delay.Elapsed(&t, 100))
			Task();

		// logic goes here (perhaps in a separate task-like function, whatever)
    }
}

static inline void Init() {
    InitClock(clk2MHzInternal);
    klJtagDisable();

    Delay.Init();
    Uart.Init(115200);
    Uart.Printf("\rArmlet\r");
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
