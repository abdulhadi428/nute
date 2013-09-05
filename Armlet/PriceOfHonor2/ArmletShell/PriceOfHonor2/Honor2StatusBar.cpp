#include "ThreeKShell.h"
#include "Honor2.h"
#include "ArmletApi.h"

#define MAX_HEADER_TOTAL_LEN 17

#define STD_IMG_HEIGHT 24
#define STD_FONT_HEIGHT 8

fresult Honor2StatusBar::Init( Repositories* reps, Factories* facts)
{
	fresult fres;

	Size sz;
	sz.Width = SCREENX;
	sz.Height = 26;

	fres = BaseInit(sz, reps, facts);
	ENSURESUCCESS(fres);

	_batteryImagesCount =5;
	_batteryImages[0] = battery_0;
	_batteryImages[1] = battery_1;
	_batteryImages[2] = battery_2;
	_batteryImages[3] = battery_3;
	_batteryImages[4] = battery_4;

	_networkImagesCount =5;
	_NetworkImages[0] = signal_4;
	_NetworkImages[1] = signal_3;
	_NetworkImages[2] = signal_2;
	_NetworkImages[3] = signal_1;
	_NetworkImages[4] = signal_0;

	fres = CreateStatusBar();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult Honor2StatusBar::CreateStatusBar()
{
	fresult fres;

	fres = StatusBarBase::CreateStatusBar();
	ENSURESUCCESS(fres);

	Size szStatusBarSize = GetSize();

	Position pbxBatteryPos;
	pbxBatteryPos.Left = szStatusBarSize.Width-20-1;
	pbxBatteryPos.Top = STD_FONT_HEIGHT+3;

	fres = AddBatteryLevel(pbxBatteryPos);
	ENSURESUCCESS(fres);
	
	fres = _BatteryLevel->SetValue(2);
	ENSURESUCCESS(fres);

	Position pbxNetworkPos;
	pbxNetworkPos.Left = pbxBatteryPos.Left - _BatteryLevel->GetSize().Width - 2;
	pbxNetworkPos.Top = pbxBatteryPos.Top;

	fres = AddNetworkLevel(pbxNetworkPos);
	ENSURESUCCESS(fres);

	fres = _NetworkLevel->SetValue(50);
	ENSURESUCCESS(fres);

	TextFieldFactory *tff = _Factories->GetTextFieldFactory();
	tff->CurrentTextFormatHandle = TF_HEADER;
	tff->CurrentFrames = 1;

	//clock
	//Title
	Position posClock;
	posClock.Top =1;
	posClock.Left = szStatusBarSize.Width - 6*5 - 1;


	//Title
	Position posRoom;
	posRoom.Top =1;
	posRoom.Left = posClock.Left - 2 - 3*6;
	fres = tff->GetTextBox(posRoom, "   ", &_txtRoomId);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(_txtRoomId);
	ENSURESUCCESS(fres);

	fres = tff->GetTextBox(posClock, "00:00", &_txtClock);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(_txtClock);
	ENSURESUCCESS(fres);

	//PlayerName
	Position posPlayerName;
	posPlayerName.Top =1;
	posPlayerName.Left = 1;
	
	tff->CurrentLines = 3;
	fres = tff->GetTextBox(posPlayerName, MAX_HEADER_TOTAL_LEN, &_txtPlayerName);
	ENSURESUCCESS(fres);
	fres = _pnlBasePanel->AppendControl(_txtPlayerName);
	ENSURESUCCESS(fres);

	tff->CurrentLines = tff->DefaultLines;
	tff->CurrentFrames = tff->DefaultFrames;
	tff->CurrentTextFormatHandle = tff->DefaultTextFormatHandle;

	return SUCCESS;
}

fresult Honor2StatusBar::GetBatteryImages( ImageHandle** o_batteryImages, ubyte_t* o_count )
{
	*o_batteryImages = _batteryImages;
	*o_count = _batteryImagesCount;
	return SUCCESS;
}

fresult Honor2StatusBar::GetNetworkImages( ImageHandle** o_networkImages, ubyte_t* o_count )
{
	*o_networkImages = _NetworkImages;
	*o_count = _batteryImagesCount;
	return SUCCESS;
}

ubyte_t Honor2StatusBar::GetControlsCount()
{
	return 5;
}

fresult Honor2StatusBar::SetRoom( sword_t room )
{
	if (_txtRoomId != NULL)
	{
		fresult fres;
		char sRoom[4];
		sRoom[3] = 0;

		if (room == UNKNOWN_ID)
			ArmletApi::snprintf(sRoom, 4, "???");
		else if (room == 0)
			ArmletApi::snprintf(sRoom, 4, "---");
		else if ((room>0)&&(room<90))
			ArmletApi::snprintf(sRoom, 4, "R%d", room);
		else 
			ArmletApi::snprintf(sRoom, 4, "R99");

		fres = _txtRoomId->SetText(sRoom);
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}

fresult Honor2StatusBar::SetPlayerName( char* playerName )
{
	fresult fres;
	if (_txtPlayerName!=NULL)
	{
		fres = _txtPlayerName->SetText(playerName);
		ENSURESUCCESS(fres);
	}
	return SUCCESS;
}
