#include "ThreeKShell.h"

namespace ThreeKShell {

fresult MenuItem::Select()
{
	fresult fres;

	if (_hasText)
	{
		_textField->SetTextFormat(&_SelectedFormat);
		fres = Draw();
		if (fres !=SUCCESS)
			return fres;
	}
	
	if (_hasIcon)
	{
		fres = SUCCESS;
	}

	return SUCCESS;
}

fresult MenuItem::Deselect()
{
	fresult fres;
	if (_hasText)
	{
		_textField->SetTextFormat(&_NotSelectedFormat);
		fres = Draw();
		if (fres !=SUCCESS)
			return fres;
	}

	if (_hasIcon)
	{
		fres = SUCCESS;
	}

	return SUCCESS;
}
					   
fresult MenuItem::Init( char* name, TextField* tf, TextFormat* selFormat, PictureBox* img, IControl* controlToDraw, IMenuHandler* handler, ButtonState accelerator)
{
	fresult fres;
	fres = MenuItem::MenuItemBaseInit(name, controlToDraw, handler, accelerator);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	_hasText = FALSE;
	_hasIcon = FALSE;

	_textField = NULL;
	_image = NULL;

	if (tf != NULL)
	{
		_textField=tf;
		_SelectedFormat = *selFormat;
		_NotSelectedFormat = *tf->GetTextFormat();
		_hasText = TRUE;
	}

	if (img != NULL)
	{
		_image = img;
		_hasIcon = TRUE;
	}

	return SUCCESS;
}

}