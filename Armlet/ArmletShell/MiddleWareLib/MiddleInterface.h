#pragma once

typedef void (__CALLBACK BUTTON_HANDLER)(ubyte_t button);
void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler);

extern BUTTON_HANDLER* gClickButtonHandler;
extern BUTTON_HANDLER* gHoldButtonHandler;

//TODO KQUEUE

//draw pixel alpha channel support in MiddleWare

/*

bool RequestTimer(int period, TIMER_PROC routine)
{
	if (period < 100)
		return false;
	for (int i=0;i<MAX_TIMERS;i++) {
		if (gTimers[i].bUsed) {
			continue;
		}
		//todo interlocked
		gTimers[i].bUsed = true;
		gTimers[i].curr = period;
		gTimers[i].period = period;
		gTimers[i].routine = routine;
		return true;
	}
	return false;
}

void OnElapsed50msec()
{
	for (int i=0;i<MAX_TIMERS;i++) {
		if (!gTimers[i].bUsed) {
			continue;
		}
		gTimers[i].curr -= 50;
		if (gTimers[i].curr <= 20) {
			if (gTimers[i].routine()) {
				gTimers[i].curr += gTimers[i].period;
			} else {
				gTimers[i].bUsed = false;
			}
		}
	}
}

void OnButtonClick(int button)
{
	if (button == BUTTON_R) {
		Clear(0x0009);
		return;
	}
	if (button == BUTTON_E) {
		Clear(0x0333);
		char* str = "HELLO , world!";
		DrawTextString(30,42,str,strlen(str),0,0);
		return;
	}
	if (gClickButtonHandler)
		(*gClickButtonHandler)(button);
}

void OnButtonHold(int button, int seconds)
{
	if (button==BUTTON_A) {
		Clear(BLUE);
		Show_Glyphs(2,0,16,16,2,2);
		return;
	}
	if (button==BUTTON_B) {
		Clear(BLUE);
		Show_Glyphs(2,0,16,16,2,1);
		return;
	}
	if (button==BUTTON_C) {
		Clear(BLUE);
		Show_Glyphs(1,0,8,8,1,1);
		return;
	}
	if (gClickButtonHandler)
		(*gClickButtonHandler)(button);
}

void __CALLBACK my_thread(void* param)
{
	char* str = "THREAD was started";
	DrawTextString(60,15,str,strlen(str),0,0);
	DrawTextString(15,75,str,strlen(str),0,0);
}

#define MAX_TIMERS 30
typedef struct _TIMER {
	bool bUsed;
	int curr;
	int period;
	TIMER_PROC* routine;
} TIMER;
TIMER gTimers[MAX_TIMERS];

void Show_Glyphs(int dx, int dy, int cx,int cy, 
	int scaleFactor, int fillFactor);

*/

/*
1) Мы с Кривдой пишем UI и бизнес-логику как черный ящик.
Это будет компонент с несколькими нитями.
Нить №1 - собирает входящие события (нажатия кнопок, втыкание пилюль, пакеты от гейта).
Нить №2 - Вычерпывает события и производит выходные действия (отрисовывает изменения, шлет пакеты концентратору, открывает замки).
Нить №3 - Рабочий цикл: считать файл с SD, выполнить логику (шаг автомата медицины, проверку доступа к замку, обработка входящих пакетов), записать файл на SD, послать входящие события для отрисовки.
*/

//		static array<Int64>^ ButtonTicks;
//		static array<Boolean>^ ButtonPressed;
//		static array<int>^ ButtonHolded;
//		static array<Boolean>^ SkipButtonRelease;

		//ButtonTicks = gcnew array<Int64>(N_BUTTONS);
		//ButtonPressed = gcnew array<Boolean>(N_BUTTONS);
		//ButtonHolded = gcnew array<int>(N_BUTTONS);
		//SkipButtonRelease = gcnew array<Boolean>(N_BUTTONS);

/*
void FormHelper::OnButtonPress(int button)
{
	if (ButtonPressed[button])
		return;
	Log(String::Format("Button {0} down",GetButtonName(button)));
	ButtonPressed[button] = true;
	ButtonHolded[button] = 0;
	ButtonTicks[button] = DateTime::Now.Ticks;
	SkipButtonRelease[button] = false;
}

void FormHelper::OnButtonRelease(int button)
{
	if (!ButtonPressed[button])
		return;
	ButtonPressed[button] = false;
	if (SkipButtonRelease[button]) {
		SkipButtonRelease[button] = false;
		return;
	}
	
	Log(String::Format("Button {0} up",GetButtonName(button)));
	__int64 ticks = DateTime::Now.Ticks - ButtonTicks[button];
	int msecs = (int)(ticks / 10000);
	Log(String::Format("{0} msecs elapsed", msecs));
	if (msecs>=0) {
		Log(String::Format("Button {0} clicked",GetButtonName(button)));
		 OnButtonClick(button);
	 }
}

	String^ GetButtonName(int button)
	{
		if (button==BUTTON_A) return "A";
		if (button==BUTTON_B) return "B";
		if (button==BUTTON_C) return "C";
		if (button==BUTTON_X) return "X";
		if (button==BUTTON_Y) return "Y";
		if (button==BUTTON_Z) return "Z";
		if (button==BUTTON_L) return "L";
		if (button==BUTTON_E) return "E";
		if (button==BUTTON_R) return "R";
		return "?";
	}

void FormHelper::HalfSecondTick()
{
	for (int button=0; button<N_BUTTONS; button++)
	{
		if (!ButtonPressed[button])
			continue;
		__int64 ticks = DateTime::Now.Ticks - ButtonTicks[button];
		int secs = (int)(ticks / 10000000);
		if (secs > ButtonHolded[button]) {
			if (!SkipButtonRelease[button]) {
				SkipButtonRelease[button] = true;
				Log(String::Format("Button {0} was holded 1 sec SKIP RELEASE",GetButtonName(button)));
			}
			ButtonHolded[button] = secs;
			Log(String::Format("Button {0} was holded +1 {1}",GetButtonName(button),secs));
			OnButtonHold(button, secs);
		}
	}
}
*/