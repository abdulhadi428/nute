#pragma once
#include "ThreeKShell.h"

class IDialogForm
{
public:

	virtual char* GetDialogName(){return NULL;};
	virtual fresult ShowYNDialog(char* name, char* title, char* text, ImageHandle icon){return GENERAL_ERROR;};
	virtual fresult ShowMessage(char* name, char* title, char* text, ImageHandle icon){return GENERAL_ERROR;};
};