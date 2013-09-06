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

	//some crap for visualizer
#ifdef _MSC_VER
	for (int i=0; i<MaxCureId; i++) {
		ArmletApi::WritePill(i,i+10);
		ArmletApi::SetPillName(i, (char*)CureName[i]);
	}
	for (int i=0; i<MaxTortureId; i++) {
		ArmletApi::WritePill(i+20,i+30);
		ArmletApi::SetPillName(i+20, (char*)TortureName[i]);
	}
#endif
}

bool __CALLBACK _QueryLustraTimerCallback(int elapsed)
{
	App.Logic->OnLustraTimer();
	return TRUE;
}


bool __CALLBACK _MedicineTimerTickCallback(int elapsed)
{
	App.Logic->OnMedTick();
	return TRUE;
}

bool __CALLBACK _QuerySystemStatusTimerCallback(int elapsed)
{
	return App.OnSystemTimer();
}

void _OnPillConnected( ubyte_t pill_id, sword_t charges )
{
	return App.Logic->OnPillConnected(pill_id, charges);
}

void _OnSetPlayerName( char* name )
{
	return App.Logic->SetPlayerName(name);
}


void _OnExplosion(sword_t roomId, ubyte_t probability, ubyte_t explosionType)
{
	return App.Logic->OnExposion(roomId, probability, explosionType);
}

void _OnServerMessage(char* msg)
{
	return App.Logic->OnNewMessage(msg);
}

void _OnSetRegenerationRate(sword_t regenRate)
{
	return App.Logic->SetRegenRate((ubyte_t)regenRate);
}