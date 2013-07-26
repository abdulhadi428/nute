#include "ThreeKShell.h"

namespace ThreeKShell {

fresult PanelBase::SetControls(IControl** controls, ubyte_t count )
{
	bool_t isInRect;

	for (int i=0; i < count; i++)
	{
		isInRect = IsRectInsideRect(controls[i]->GetPosition(), controls[i]->GetSize(), _Position, _Size);
		if (isInRect == FALSE)
		{
			return GENERAL_ERROR;
		}
	}
	
	return SUCCESS;
}

fresult PanelBase::SetControl( IControl* control, ubyte_t index )
{
	//can ONLY replace NULLS, cause otherwise it might cause a mem leak
	FAILIF(!(index<_ControlsCount));
	FAILIF(_Controls[index]!=NULL);

	_Controls[index] = control;
	return SUCCESS;
}

fresult PanelBase::SetColor(Color bgclr)
{
	_BgColor = bgclr;
	return SUCCESS;
}

fresult PanelBase::BaseInit( Size size, Position position, IRender* renderer, IControl** controls, ubyte_t controlsCount )
{
	fresult fres;
	fres = ControlBase::BaseInit(size, position, renderer);
	if (fres != SUCCESS)
	{
		return fres;
	}

	if (controlsCount < 0)
	{
		return GENERAL_ERROR;
	}

	if (controlsCount >0 && controls == NULL)
	{
		return GENERAL_ERROR;
	}

	_ControlsCount = controlsCount;
	_Controls = controls;

	_BgColor = _TRANSPARENT;

	return SUCCESS;
}

//Draw child controls areas
fresult PanelBase::DrawArea( Position pos, Size size )
{
	Position ctrlPos;
	Size ctrlSize;
	
	Position ctrlDrawPos;
	Size ctrlDrawSize;

	IControl* control = NULL;
	bool_t hasIntersection;
	fresult fres;

	if (_BgColor != _TRANSPARENT)
	{
		fres = _render->DrawRect(pos, size, _BgColor);
		ENSURESUCCESS(fres);
	}

	//for each control draw it's content in the give area
	for (int i=0;i < _ControlsCount; i++)
	{
		control = _Controls[i];
		if( !control)
			continue;
		ctrlPos = control->GetPosition();
		ctrlSize = control->GetSize();
		//has intersection?
		hasIntersection = GetRectClipping(pos,size, ctrlPos, ctrlSize, &ctrlDrawPos, &ctrlDrawSize);
		if (hasIntersection == TRUE)
		{
			fres = control->DrawArea(ctrlDrawPos,ctrlDrawSize);
			if(fres != SUCCESS)
			{
				return fres;
			}
		}

	}

	return SUCCESS;
}

}


