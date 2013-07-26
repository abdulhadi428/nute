#pragma once 

class OpenSpaceStatusBar : public StatusBarBase
{
	fresult CreateStatusBar();
	TextField* txtHeader;
	TextField* txtSubtitle;
	TextField* txtSubtitle2Line;

	ubyte_t _headerMinLeft;

public:
	fresult Init(Repositories* reps, Factories* facts);
	fresult SetTitle(char* szTitle, char* szSubtitle);
	
};