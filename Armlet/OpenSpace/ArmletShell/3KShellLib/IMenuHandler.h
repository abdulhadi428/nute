#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class IMenuHandler
{
public:
	virtual fresult OnClick(IMenuItem* sender)=0;
};
