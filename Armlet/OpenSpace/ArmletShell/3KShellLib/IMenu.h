#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class IMenu
{
public:
	virtual sbyte_t  GetSelectedItemIndex() {return -1;};

	virtual IMenuItem*  GetItem(ubyte_t index) {return NULL;};

	virtual void SetMustHaveSelected(bool_t mustHaveSelected) {return;};
	virtual bool_t GetMustHaveSelected() {return FALSE;};

	virtual fresult ProcessButton(ButtonState key, bool* found) {return GENERAL_ERROR;};
	
	virtual fresult  SelectItem(ubyte_t index) {return GENERAL_ERROR;};
	virtual fresult  SelectNext() {return GENERAL_ERROR;};
	virtual fresult  SelectPrevious(){return GENERAL_ERROR;};

	virtual fresult  Draw(){return GENERAL_ERROR;};
};
