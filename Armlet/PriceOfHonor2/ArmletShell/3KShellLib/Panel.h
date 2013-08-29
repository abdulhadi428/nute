#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class Panel : public PanelBase
{
public:
	fresult Init(Size size, Position position, IRender* renderer, IControl** controls, ubyte_t controlsCount)
	{
		return PanelBase::BaseInit(size, position, renderer, controls, controlsCount);
	}
};
