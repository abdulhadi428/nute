#pragma once

class StatusBarBase
{
protected:	

	Repositories* _Repositories;
	Factories* _Factories;
//TODO: prefixes
	Panel* _pnlBasePanel;
	TextField* _txtHeader;
	TextField* _txtClock;


	ScaledValueImageList* _imglBatteryLevel;
	ScaledValueImageList* _imglNetworkLevel;

	bool_t _Visible;
	Size _Size;

	fresult BaseInit(Size sz, Repositories* reps, Factories* facts);	
	virtual fresult CreateStatusBar();
	
	virtual fresult CreateBatteryLevelControl(Position pos, ScaledValueImageList** o_imglBatteryLevel);
	virtual fresult GetBatteryImages(ImageHandle** o_batteryImages, ubyte_t* o_count);
	fresult AddBatteryLevel(Position pos);
	
	virtual fresult CreateNetworkLevelControl(Position pos, ScaledValueImageList** o_imglNetworkLevel);
	virtual fresult GetNetworkImages(ImageHandle** o_networkImages, ubyte_t* o_count);
	fresult AddNetworkLevel(Position pos);

	fresult AddClock(Position pos);
public:
	virtual fresult SetVisible(bool_t visibileValue);
	virtual bool_t  GetVisible();
	virtual fresult Draw();
	virtual Size GetSize();
	virtual ubyte_t GetControlsCount();
	virtual fresult SetHeaderText(char* headerText);

	//>100 for charching
	virtual fresult SetBatteryLevel(ubyte_t level);
	
	//in -db
	virtual fresult SetNetworkSignalStrength(sbyte_t dbLevel);
	virtual fresult SetTime(char* timeString);

};