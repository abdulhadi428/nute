#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class PanelFactory
{
	IControl** AllocControlArray(ubyte_t controlsCount);
	Panel* AllocPanel();

	IRender* _render;
	Repositories* _repositories;

public:

	fresult Init (IRender* render, Repositories* reps);
	fresult GetPanel(Size sz, Position pos, ubyte_t controlsCount, Panel** o_pnl);
	fresult GetPanel(Size sz, Position pos, ubyte_t controlsCount, ColorHandle clrhandle,  Panel** o_pnl);
};
