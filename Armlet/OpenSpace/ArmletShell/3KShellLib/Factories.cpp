#include "ThreeKShell.h"

namespace ThreeKShell
{

fresult Factories::Init( PanelFactory* pnlFactory, PictureBoxFactory* pbxFactory, TextFieldFactory* tfFactory, MenuItemFactory* miFactory,MenuFactory* mnuFactory )
{
	_pnlFactory =pnlFactory;
	_pbxFactory =pbxFactory;
	_tfFactory  =tfFactory;
	_miFactory  =miFactory;
	_mnuFactory =mnuFactory;

	return SUCCESS;
}

PanelFactory* Factories::GetPanelFactory()
{
	return _pnlFactory;
}

PictureBoxFactory* Factories::GetPictureBoxFactory()
{
	return _pbxFactory;
}

TextFieldFactory* Factories::GetTextFieldFactory()
{
	return _tfFactory;
}

MenuItemFactory* Factories::GetMenuItemFactory()
{
	return _miFactory;
}

MenuFactory* Factories::GetMenuFactory()
{
	return _mnuFactory;
}

}
