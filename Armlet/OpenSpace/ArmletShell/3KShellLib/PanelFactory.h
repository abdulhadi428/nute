#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class PanelFactory
{
	IControl** AllocControlArray(ubyte_t controlsCount);
	Panel* AllocPanel();

	IRender* _render;
public:
	PanelFactory(IRender* render);

	fresult GetPanel(Size sz, Position pos, ubyte_t controlsCount, Panel* o_pnl);
};
