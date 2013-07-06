#pragma once
#include "MenuItemBase.h"
#include "Image.h"
#include "TextField.h"
#include "IMenuHandler.h"

namespace ThreeKShell {

class MenuItem : public MenuItemBase
{
	bool_t _hasIcon;
	bool_t _hasText;
protected:
	TextField* _textField;
	TextFormat _NotSelectedFormat;
	TextFormat _SelectedFormat;

	Image* _image;

public:

	//controlToDraw use Panel containing both, if text item has both text and image
	fresult Init(TextField* tf, TextFormat* selFormat, Image* img, IControl* controlToDraw, IMenuHandler* handler, ButtonState accelerator);

	fresult virtual Select();

	fresult virtual Deselect();

};

}
