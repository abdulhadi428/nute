#include "ArmletApi.h"
#include "ArmletAppSDK.h"
#include "MiddleInterface.h"
#include "ThreeKShell.h"

#include "OpenSpace.h"

OpenSpaceApp App;

void AppOnButtonClick(ubyte_t button)
{
	bool menuFound=FALSE;
	App.OnButtonEvent(button, &menuFound);
	return;
}

void AppOnButtonHold(ubyte_t button)
{
	bool menuFound=FALSE;
	App.OnButtonEvent(button, &menuFound);
	return;
}


void AppMainThread(void* param)
{
	fresult fres = App.Init();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to init App";
		DrawTextString(10,10,err,Length(err),WHITE,0);	
		return;
	}
	
	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);

	fres = App.Start();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to Show main form";
		DrawTextString(10,10,err,Length(err),WHITE,0);	
		return;
	}
}


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

bool __CALLBACK _QuerySystemStatusTimerCallback(int elapsed)
{
	App.OnSystemTimer();
	return true;
}

void _OnPillConnected( ubyte_t pill_id, sword_t charges )
{
	int a =0;
	return;
	//UI.OnPillConnected(cure_id, charges);
}

void _OnSetPlayerName( char* name )
{
	return;
}


void _OnExplosion(sword_t roomId, ubyte_t probability, ubyte_t explosionType)
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

