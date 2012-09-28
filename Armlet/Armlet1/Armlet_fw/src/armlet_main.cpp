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

const int W = LCD_STR_WIDTH;
const int H = LCD_STR_HEIGHT;

const int MAX_STATUS_LENGTH = 1000;
const int MAX_STATUS_LINES = MAX_STATUS_LENGTH/(W-2);

char status[MAX_STATUS_LENGTH+1] = "None";
char status_lines[MAX_STATUS_LINES][W+1];
int num_status_lines = 0;
int scroll_position = 0;

void SetStatus(const char *new_status) {
	assert(W > 1);
	if (strcmp(status, new_status) == 0)
		return;
	num_status_lines = 1;
	char *cur_line = status_lines[0];
	int i = 0;
	bool write_spaces = true;
	for (const char *p = new_status; ; p++) {
		bool need_new_line = false;
		if (*p == 0)
			need_new_line = true;
		else if (i == W)
			need_new_line = true;
		else if (i == W-1 &&
				 p[-1] == ' ' &&
				 p[0] != 0 && p[0] != ' ' &&
				 p[1] != ' ' && p[1] != '\n')
			need_new_line = true;
		else if (*p == '\n')
			need_new_line = true;
		// finalize current line
		if (need_new_line) {
			// TODO: replace filler with ' ' in release
			while (i < W)
				cur_line[i++] = '^';
			cur_line[i] = 0;
		}
		if (*p == 0)
			break;
		// add new line
		if (need_new_line) {
			cur_line = status_lines[num_status_lines++];
			i = 0;
			write_spaces = (*p == '\n');
		}

		if (*p != '\n' && (*p != ' ' || write_spaces))
			cur_line[i++] = *p;

		if (*p != ' ')
			write_spaces = true;
	}
	scroll_position = 0;
}

void DrawIdleScreen() {
	for (int row = 0; row < H; row++) {
		int i = row+scroll_position;
		if (i >= 0 && i < num_status_lines)
			Lcd.Printf(0, row, "%s", status_lines[i]);
		else {
			for (int j = 0; j < W; j++)
				Lcd.Printf(j, row, " ");
		}
	}

	Lcd.SetDrawMode(OVERWRITE_INVERTED);
	if (scroll_position > 0)
		Lcd.Printf((W-5)/2, 0, " %c%c%c ", 0x18, 0x18, 0x18);
	if (scroll_position+H < num_status_lines)
		Lcd.Printf((W-5)/2, H-1, " %c%c%c ", 0x19, 0x19, 0x19);
	Lcd.SetDrawMode(OVERWRITE);
}

void Task() {
	switch (state) {
	case IDLE:
		if (Keys.Up.WasJustPressed()) {
			if (scroll_position > 0) {
				scroll_position--;
				DrawIdleScreen();
			}
		}
		if (Keys.Down.WasJustPressed()) {
			if (scroll_position+H < num_status_lines) {
				scroll_position++;
				DrawIdleScreen();
			}
		}
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
			DrawIdleScreen();
		}
		break;
	}
}


static void Init();

void ArmletMain() {
	static uint32_t t = 0;
    Init();

    Lcd.Cls();

    SetStatus("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.");
    DrawIdleScreen();

    while (1) {
    	Uart.Task();
    	Lcd.Task();
        Vibro.Task();

    	if (assertion_raised)
    		continue;
		//Battery.Task();
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
