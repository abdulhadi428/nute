#pragma once
#include "ThreeKShell.h"

class IDialogForm
{
public:

	char* GetDialogName(){return NULL;};
	fresult ShowYNDialog(char* name, char* title, char* text, ImageHandle icon){return GENERAL_ERROR;};
	fresult ShowMessage(char* name, char* title, char* text, ImageHandle icon){return GENERAL_ERROR;};
};