#pragma once 

class OpenSpaceStatusBar : public StatusBarBase
{
	fresult CreateStatusBar();
	TextField* txtHeader;
	TextField* txtSubtitle;

	ImageHandle _batteryImages[5];
	ubyte_t _batteryImagesCount;
	ImageHandle _NetworkImages[5];
	ubyte_t _networkImagesCount;

	ubyte_t _headerMinLeft;

public:
	fresult Init(Size sz, Repositories* reps, Factories* facts);
	fresult SetTitle(char* szTitle, char* szSubtitle);

	virtual fresult GetBatteryImages( ImageHandle** o_batteryImages, ubyte_t* o_count );

	virtual fresult GetNetworkImages( ImageHandle** o_networkImages, ubyte_t* o_count );

	virtual ubyte_t GetControlsCount();


};