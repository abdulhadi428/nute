#pragma once

class StatusBarBase
{
protected:	

	Repositories* _Repositories;
	Factories* _Factories;

	Panel* _pnlBasePanel;
	TextField* _txtHeader;

	fresult BaseInit(Repositories* reps, Factories* facts);	

public:

	virtual fresult Draw();
	virtual Size GetSize();
	virtual fresult SetHeaderText(char* headerText);

};