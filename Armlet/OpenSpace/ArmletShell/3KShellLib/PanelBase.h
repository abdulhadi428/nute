#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class PanelBase : public ControlBase
{
protected:

	IControl** _Controls;
	ubyte_t _ControlsCount;
	Color _BgColor;

	//Initialization;
	fresult BaseInit(Size size, Position position, IRender* renderer, IControl** controls, ubyte_t controlsCount);
	
	fresult SetControls( IControl** controls, ubyte_t count );

	virtual fresult DrawArea( Position pos, Size size );
public:
	fresult SetControl( IControl* control, ubyte_t index );
	fresult SetColor(Color bgclr);

};

