#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

typedef sword_t ButtonState;

class IMenuItem
{
public:
	virtual fresult Draw() =0;
	virtual bool_t IsSelected()=0;

	virtual fresult Select() =0;
	virtual fresult Deselect() =0;

	virtual fresult Click() =0;

	virtual bool_t CheckAccelerator(ButtonState button)=0;
};
