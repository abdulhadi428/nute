#include "TextMenuItem.h"

fresult TextMenuItem::Select()
{
	fresult fres;
	_textField->SetTextFormat(&_SelectedFormat);
	fres = Draw();
	return fres;
}

fresult TextMenuItem::Deselect()
{
	fresult fres;
	_textField->SetTextFormat(&_NotSelectedFormat);
	fres = Draw();
	return fres;
}
