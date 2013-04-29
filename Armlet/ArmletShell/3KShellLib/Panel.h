#pragma once
#include "PanelBase.h"

namespace ThreeKShell {

class Panel : public PanelBase
{
public:
	fresult Init(Size size, Position position, IRender* renderer, IControl** controls, ubyte_t controlsCount)
	{
		return BaseInit(size, position, renderer, controls, controlsCount);
	}
};

}