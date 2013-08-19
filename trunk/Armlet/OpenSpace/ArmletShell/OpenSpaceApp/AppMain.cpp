#include "ArmletApi.h"
#include "ArmletShell.h"
#include "MiddleInterface.h"
/*#include "UserInterface.h"
#include "MenuDelegate.h"
#include "AppSpecificImages.h"
#include "ColorSchema.h"
#include "Med.h"*/
#include "ThreeKShell.h"

#include "OpenSpace.h"

//UserInterface UI;

OpenSpaceApp App;

void AppOnButtonClick(ubyte_t button)
{
	App.OnButtonEvent(button);
	return;
}

void AppOnButtonHold(ubyte_t button)
{
	App.OnButtonEvent(button);
	return;
}


void AppMainThread(void* param)
{
	fresult fres = App.Init();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to init App";
		DrawTextString(10,10,err,Length(err),0,0);	
		return;
	}
	
	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);

	fres = App.Start();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to Show main form";
		DrawTextString(10,10,err,Length(err),0,0);	
		return;
	}
}


/*
void AppMainThread(void* param)
{
	App.Init();
	
	
	

	_medInit();
#ifdef _MSC_VER
	for (int i=0; i<MaxCureId; i++) {
		ArmletApi::WritePill(i,i+10);
		ArmletApi::SetCureName(i, (char*)CureNames[i]);
	}
#endif

	

        //MenuItemFactory* a = new MenuItemFactory();
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
*/


bool __CALLBACK _QueryLustraTimerCallback(int elapsed)
{
        return true;
	//return UI.OnLustraTimer();
}


bool __CALLBACK _MedicineTimerTickCallback(int elapsed)
{
	//UI.DoMedTick();
	return true;
}

bool __CALLBACK _QueryBatteryStatusTimerCallback(int elapsed)
{
        return true;
	//return UI.OnBatteryTimer();
}

void _OnPillConnected( sword_t cure_id, sword_t charges )
{
        return;
	//UI.OnPillConnected(cure_id, charges);
}

void _OnSetPlayerName( char* name )
{
        return;
	//UI.SetPlayerName(name);
}


void _OnExplosion(sword_t roomId)
{
        return;
	//UI.OnExplosion(roomId);
}

void _OnServerMessage(char* msg)
{
        return;
	//UI.OnServerMessage(msg);
}

void _OnSetRegenerationRate(sword_t regenRate)
{
        return;
	//_medSetRegenerationRate(regenRate);
}

/*
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
*/