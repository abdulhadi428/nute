// Roman. Похоже это файл с main-функцией
#include "ArmletApi.h"
#include "ArmletShell.h"
#include "UserInterface.h"
#include "MenuDelegate.h"
#include "MiddleInterface.h"
#include "Images.h"
#include "ColorSchema.h"
#include "Osanve.h"

UserInterface UI;
uword_t gx=0;
uword_t gy=0;
Color gc=0x0000;

// Нажатие кнопки (с кодом кнопки?)
void AppOnButtonClick(ubyte_t button)
{
	UI._SystemOnButtonClick(button);
	return;
}

// Удержание кнопки?
void AppOnButtonHold(ubyte_t button)
{
	UI._SystemOnButtonClick(BUTTON_HOLD_OFFSET+button);
	return;
}

// таймер на отправку осанве
bool __CALLBACK _OsanveSendTimer(int elapsed)
{
	return UI.OnOsanveTimer();
}

// таймер восстановления сил
bool __CALLBACK _ForceTimerTickCallback(int elapsed)
{
	UI.DoForceTick();
	return true;
}

// таймер проверки батареи
bool __CALLBACK _QueryBatteryStatusTimerCallback(int elapsed)
{
	return UI.OnBatteryTimer();
}

// таймер выхода из предбоевого режма
bool __CALLBACK _PreFightTimerCallback(int elapsed)
{
	return UI.OnPreFightTimer();
}

// screensaver?
bool MoveBoxByTimer()
{
	DrawRect(gx,gy,30,30,0);
	gc+=0x111;
	if (gc>=0x1000) gc=0x0000;
	gx+=15; gy+=5;
	if (gx>SCREENX) gx-=SCREENX;
	if (gy>SCREENY) gy-=SCREENY;
	DrawRect(gx,gy,30,30,gc);
	return false;
}

// Main?
void AppMainThread(void* param)
{
	fresult fres;
	PlayerInit();
//#ifdef _MSC_VER
//	for (int i=0; i<MaxCureId; i++) {
//		ArmletApi::WritePill(i,i+10);
//		ArmletApi::SetCureName(i, (char*)CureNames[i]);
//	}
//#endif

	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);

	//extern const unsigned short ArrowDownOrangeBitmap[];
	//DrawBitmap_kel(0,0,24,24,(short*)ArrowDownOrangeBitmap);

	InitMenuHandlerDelegates(&UI);
	InitImages();
	InitColorSchema();
	fres = UI.Init();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to init UI";
		DrawTextString(10,10,err,Length(err),0,0);
	}	else	{
		fres = UI.Draw();
		if (fres!=SUCCESS)
		{
			char* err = "Failed to draw UI";
			DrawTextString(10,10,err,Length(err),0,0);
		}
	}
}

// установить имя
void _OnSetPlayerName( char* name )
{
	UI.SetPlayerName(name);
}
// установить скорость регенирации
void _OnSetRegenerationRate(ubyte_t regenRate)
{
	SetFPH(regenRate);
}
// реакция на атаку
void _OnAtackPacket(ubyte_t enemyId, ubyte_t atack, ubyte_t cons)
{
  Atacked( atack, cons, enemyId);
  if( Player.status == AL_STATUS_DEFEAT && cons < CONS_COUNT)
	UI.MessageBoxShow("Поражение!", CONS[cons], NO_IMAGE);
}

void _OnHealPacket(ubyte_t enemyId, ubyte_t heal)
{
   Healed( heal);
}

void _OnOsanvePacket(ubyte_t userId, ubyte_t force,ubyte_t maxForce, ubyte_t osanve)
{  	// ищем этого противника
  if( userId > MAX_ARMLET)
	  return;
  struct nick* fp = &FGHT[userId];
  // проверка на осведомленность, если давно не слышали - сразу перерисуем
  int t = ArmletApi::GetUpTime();
  bool needSet = false;
  if( !fp->userId || (t-fp->time)>OSANVE_MAX_WAIT_TIME)
	  needSet = true;
	fp->userId = userId;
  	fp->osanve = osanve;
	fp->force = force;
	fp->maxForce = maxForce;
	fp->time = t;
	if( needSet && Player.status == AL_STATUS_OSANVE)
	  UI.OnOsanveTimer();
// перерисовка осанве - по таймеру
}

