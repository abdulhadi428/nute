#include "ArmletApi.h"
#include "ArmletShell.h"
#include "UserInterface.h"
#include "MenuDelegate.h"
#include "MiddleInterface.h"
#include "AppSpecificImages.h"
#include "ColorSchema.h"
#include "Med.h"

UserInterface UI;

void AppOnButtonClick(ubyte_t button)
{
	UI._SystemOnButtonClick(button);
	return;
}

void AppOnButtonHold(ubyte_t button)
{
	UI._SystemOnButtonClick(BUTTON_HOLD_OFFSET+button);
	return;
}

bool __CALLBACK _QueryLustraTimerCallback(int elapsed)
{
	return UI.OnLustraTimer();
}


bool __CALLBACK _MedicineTimerTickCallback(int elapsed)
{
	UI.DoMedTick();
	return true;
}

bool __CALLBACK _QueryBatteryStatusTimerCallback(int elapsed)
{
	return UI.OnBatteryTimer();
}


uword_t gx=0;
uword_t gy=0;
Color gc=0x0000;
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

void AppMainThread(void* param)
{
	_medInit();
#ifdef _MSC_VER
	for (int i=0; i<MaxCureId; i++) {
		ArmletApi::WritePill(i,i+10);
		ArmletApi::SetCureName(i, (char*)CureNames[i]);
	}
#endif

	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);

	//extern const unsigned short ArrowDownOrangeBitmap[];
	//DrawBitmap_kel(0,0,24,24,(short*)ArrowDownOrangeBitmap);

	fresult fres;
	InitMenuHandlerDelegates(&UI);
	InitImages();
	InitColorSchema();
	fres = UI.Init();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to init UI";
		DrawTextString(10,10,err,Length(err),0,0);
	}
	else
	{
		fres = UI.Draw();
		if (fres!=SUCCESS)
		{
			char* err = "Failed to draw UI";
			DrawTextString(10,10,err,Length(err),0,0);
		}
	}

}

void _OnPillConnected( sword_t cure_id, sword_t charges )
{
	UI.OnPillConnected(cure_id, charges);
}

void _OnSetPlayerName( char* name )
{
	UI.SetPlayerName(name);
}


void _OnExplosion(sword_t roomId)
{
	UI.OnExplosion(roomId);
}

void _OnServerMessage(char* msg)
{
	UI.OnServerMessage(msg);
}

void _OnSetRegenerationRate(sword_t regenRate)
{
	_medSetRegenerationRate(regenRate);
}