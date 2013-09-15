#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

typedef sword_t ButtonState;

class IMenuItem
{
public:
	virtual fresult Draw() {return GENERAL_ERROR;};
	virtual bool_t IsSelected() {return FALSE;};

	virtual fresult Select() {return GENERAL_ERROR;};
	virtual fresult Deselect() {return GENERAL_ERROR;};

	virtual fresult SetVisible(bool_t val) {return GENERAL_ERROR;};
	virtual bool_t GetVisible() {return FALSE;};

	virtual fresult SetEnabled(bool_t val) {return GENERAL_ERROR;};
	virtual bool_t GetEnabled() {return FALSE;};

	virtual fresult Click() {return GENERAL_ERROR;};

	virtual fresult ProcessKey(ButtonState button, bool* found){return FALSE;};
	virtual char* GetName() {return NULL;};				

};
