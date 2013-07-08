#include "PanelFactory.h"
#include "ArmletShell.h"

namespace ThreeKShell {



	IControl*[] PanelFactory::AllocControlArray( ubyte_t controlsCount )
	{
		return new IControl*[controlsCount];
	}

	Panel* PanelFactory::AllocPanel()
	{
		return new Panel();
	}

	fresult PanelFactory::GetPanel( Size sz, Position pos, ubyte_t controlsCount, Panel* o_pnl )
	{
		fresult fres;
		
		IControl* controls = AllocControlArray(controlsCount);
		FAILIF(controlsCount==NULL);

		Panel* pnl = AllocPanel();
		FAILIF(pnl==NULL);
		
		fres = pnl->Init(sz,pos,_render, controls, controlsCount);
		ENSURESUCCESS(fres);

		return fres;
	}

	PanelFactory::PanelFactory( IRender* render )
	{
		_render = render;
	}

}



