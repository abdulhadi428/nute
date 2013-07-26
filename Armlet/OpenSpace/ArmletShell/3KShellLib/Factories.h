#ifndef _NAMESPACE_PROTECT_
#error "Use ThreeKShell.h"
#endif

class Factories
{
	PanelFactory* _pnlFactory;
	PictureBoxFactory* _pbxFactory;
	TextFieldFactory* _tfFactory;
	MenuItemFactory* _miFactory;
	MenuFactory* _mnuFactory;

public:

	fresult Init(PanelFactory* pnlFactory, PictureBoxFactory* pbxFactory, TextFieldFactory* tfFactory, MenuItemFactory* miFactory,MenuFactory* mnuFactory);

	PanelFactory*      GetPanelFactory();
	PictureBoxFactory* GetPictureBoxFactory();
	TextFieldFactory*  GetTextFieldFactory();
	MenuItemFactory*   GetMenuItemFactory(); 
	MenuFactory*       GetMenuFactory();

};



