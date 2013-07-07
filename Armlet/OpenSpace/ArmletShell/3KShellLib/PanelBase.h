#pragma once
#include "ControlBase.h"

namespace ThreeKShell {

class PanelBase : public ControlBase
{
protected:

	IControl** _Controls;
	ubyte_t _ControlsCount;

	//Initialization;
	fresult BaseInit(Size size, Position position, IRender* renderer, IControl** controls, ubyte_t controlsCount);
	
	fresult SetControls( IControl** controls, ubyte_t count );

	virtual fresult DrawArea( Position pos, Size size );

};

}
