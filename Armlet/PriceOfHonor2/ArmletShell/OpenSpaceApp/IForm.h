#pragma once
#include "ThreeKShell.h"

class IForm
{
public:
	virtual char* GetName() {return NULL;};

	virtual fresult OnButtonEvent(ButtonState key) {return GENERAL_ERROR;};
	virtual fresult OnBeforeShow(IForm* prevFrom, bool_t reActivation){return GENERAL_ERROR;};
	virtual fresult OnAfterShow(IForm* prevFrom, bool_t reActivation){return GENERAL_ERROR;};
	virtual fresult OnLoad(){return GENERAL_ERROR;};
	virtual fresult DoLayout(){return GENERAL_ERROR;};
	virtual fresult Draw(){return GENERAL_ERROR;};
};