#include "ArmletApi.h"
#include "ArmletShell.h"
#include "MiddleInterface.h"
#include "ThreeKShell.h"
#include "Honor2.h"

Honor2App App;

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
	return true;
}

void _OnPillConnected( sword_t cure_id, sword_t charges )
{
	return;
	//UI.OnPillConnected(cure_id, charges);
}

void _OnSetPlayerName( char* name )
{
	return;
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

