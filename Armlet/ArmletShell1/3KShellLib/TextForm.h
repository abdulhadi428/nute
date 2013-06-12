#pragma once

//TextForm is a container of Controls.
// it draws (renders) it's TextField on the screen
// Usage:
//  1. Init
//  2. forget. Just play with controls
//
//Special:
// Form cropps controls content to it's size

//160*128
//20*16

#include "TextField.h"

namespace ThreeKShell {

#define HEIGHT  16	
#define WIDTH  20


class TextForm
{
	public:
	TextField* _pControls;
	int _controlsCount;
	Size _Size;
	IRender *_pRender;

	//Registers controls on the form, reoders them in the display ZOrder
	fresult RegisterControls();
	
	//Registers a control on the form
	fresult RegisterControl(TextField* pControl);

	//Display control on screen
	fresult DrawControl(TextField* pControl);

	//displays char on screen using specified formatting
	fresult PutCharToScreen(Position* position, char character, char format);

public:
	
	//Initialization
	fresult Init(TextField* textFields, int controlsCount, Size* size, IRender *pRender);

	//Logic
	
};

}