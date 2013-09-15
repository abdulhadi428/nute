#pragma once
#include "ThreeKShell.h"

typedef enum _FORM_SHOW_RESULTS {
	fsrOK,
	fsrCancel,
	fsrNone,
	FormShowResultsCount
} FormShowResults;

class IForm
{
public:
	virtual char* GetName() {return NULL;};

	virtual fresult OnButtonEvent(ButtonState key, bool* found) {return GENERAL_ERROR;};
	virtual fresult OnAfterDialogShown( IForm* prevFrom, char* dialogName, FormShowResults results, bool* needRedraw ){return FALSE;};
	virtual fresult OnBeforeShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult){return GENERAL_ERROR;};
	virtual fresult OnAfterShown(IForm* prevFrom, bool_t reActivation, FormShowResults formShowResult){return GENERAL_ERROR;};
	virtual fresult OnLoad(){return GENERAL_ERROR;};
	virtual fresult DoLayout(){return GENERAL_ERROR;};
	virtual fresult Draw(){return GENERAL_ERROR;};
	virtual bool_t  IsDialog(){return FALSE;};

};