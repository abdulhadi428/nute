#pragma once
#include "Panel.h"
#include "Dimensions.h"

fresult Panel::SetControls(IControl** controls, ubyte_t count )
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

fresult Panel::BaseInit( Size size, Position position, IRender* renderer, IControl** controls, ubyte_t controlsCount )
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

	if (controls == NULL)
	{
		return GENERAL_ERROR;
	}
	_ControlsCount = controlsCount;
	_Controls = controls;

	return SUCCESS;
}

//Draw child controls areas
fresult Panel::DrawArea( Position pos, Size size )
{
	Position ctrlPos;
	Size ctrlSize;
	
	Position ctrlDrawPos;
	Size ctrlDrawSize;

	IControl* control;
	bool_t hasIntersection;
	fresult fres;

	//for each control draw it's content in the give area
	for (int i=0;i < _ControlsCount; i++)
	{
		control = _Controls[i];
		ctrlPos = control->GetPosition();
		ctrlSize = control->GetSize();
		//has intersection?
		hasIntersection = GetRectClipping(pos,size, ctrlPos, ctrlSize, &ctrlDrawPos, &ctrlDrawSize);
		if (hasIntersection = TRUE)
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


