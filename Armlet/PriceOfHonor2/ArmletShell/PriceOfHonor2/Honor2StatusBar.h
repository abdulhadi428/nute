#pragma once 

class Honor2StatusBar : public StatusBarBase
{
	fresult CreateStatusBar();
	TextField* _txtPlayerName;
	TextField* _txtRoomId;

	ImageHandle _batteryImages[5];
	ubyte_t _batteryImagesCount;
	ImageHandle _NetworkImages[5];
	ubyte_t _networkImagesCount;

	ubyte_t _headerMinLeft;

public:
	fresult Init(Repositories* reps, Factories* facts);
	fresult SetPlayerName(char* playerName);
	fresult SetRoom(sword_t room);

	virtual fresult GetBatteryImages( ImageHandle** o_batteryImages, ubyte_t* o_count );

	virtual fresult GetNetworkImages( ImageHandle** o_networkImages, ubyte_t* o_count );

	virtual ubyte_t GetControlsCount();
};