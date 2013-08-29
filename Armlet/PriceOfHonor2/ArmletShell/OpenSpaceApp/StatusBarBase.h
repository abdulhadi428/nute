#pragma once

class StatusBarBase
{
protected:	

	Repositories* _Repositories;
	Factories* _Factories;

	Panel* _pnlBasePanel;
	TextField* _txtHeader;

	ScaledValueImageList* _BatteryLevel;
	ScaledValueImageList* _NetworkLevel;

	fresult BaseInit(Repositories* reps, Factories* facts);	
	virtual fresult CreateStatusBar();
	
	virtual fresult CreateBatteryLevelControl(Position pos, ScaledValueImageList** o_batteryLevel);
	virtual fresult GetBatteryImages(ImageHandle** o_batteryImages, ubyte_t* o_count);
	fresult AddBatteryLevel(Position pos);
	
	virtual fresult CreateNetworkLevelControl(Position pos, ScaledValueImageList** o_networkLevel);
	virtual fresult GetNetworkImages(ImageHandle** o_networkImages, ubyte_t* o_count);
	fresult AddNetworkLevel(Position pos);
public:

	virtual fresult Draw();
	virtual Size GetSize();
	virtual ubyte_t GetControlsCount();
	virtual fresult SetHeaderText(char* headerText);

};