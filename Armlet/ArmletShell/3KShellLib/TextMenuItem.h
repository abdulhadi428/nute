#pragma once

#include "project.h"
#include "MenuItemBase.h"
#include "TextField.h"

class TextMenuItem : public MenuItemBase
{
protected:
	TextField* _textField;
	TextFormat _NotSelectedFormat;
	TextFormat _SelectedFormat;

	PMY_CALLBACK _OnClick;
	int keyBinding;


public:

	fresult Init(TextField* tf, TextFormat* selFormat)
	{
		fresult fres;
		fres = TextMenuItem::TextMenuItemBaseInit(tf);
		if (fres!=SUCCESS)
		{
			return fres;
		}

		_textField=tf;
		_SelectedFormat = *selFormat;
		_NotSelectedFormat = *tf->GetTextFormat();

		return SUCCESS;
	}

	fresult virtual Select();

	fresult virtual Deselect();

};

