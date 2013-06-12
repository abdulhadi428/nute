#include "ArmletShell.h"
#include "TextForm.h"

//TextForm is a container of Controls.
// it draws (renders) it's TextField on the screen
// Usage:
//  1. Init
//  2. forget. Just play with controls
//
//Special:
// Form crops controls content to it's size

//160*128
//20*16

namespace ThreeKShell {

//Registers controls on the form, reorders them in the display ZOrder
fresult TextForm::RegisterControls()
{
	fresult result = SUCCESS;

	//TODO: add ZOrder sort before registering!


	//register controls
	for (int i=0; i<_controlsCount;i++)
	{
		result = RegisterControl(&_pControls[i]);
		if (result != SUCCESS)
		{
			return result;
		}
	}

	return result;
}

//Registers a control on the form
fresult TextForm::RegisterControl(TextField* pControl)
{
	fresult result = SUCCESS;

	//TODO: register callbacks

	return result;
}


//Initialization
fresult TextForm::Init(TextField* textFields, int controlsCount, Size* size, IRender *pRender)
{
	fresult result = SUCCESS;

	_pControls = textFields;
	_controlsCount = controlsCount;
	_Size = *size;
	_pRender = pRender;

	result = RegisterControls();
	
	return result;
}

}

	