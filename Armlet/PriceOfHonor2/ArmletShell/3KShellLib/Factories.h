#ifndef _NAMESPACE_PROTECT_
#error "Use ThreeKShell.h"
#endif

class Factories
{
	PanelFactory* _pnlFactory;
	PictureBoxFactory* _pbxFactory;
	ImageListFactory* _imglFactory;
	TextFieldFactory* _tfFactory;
	MenuItemFactory* _miFactory;
	MenuFactory* _mnuFactory;

public:

	fresult Init(PanelFactory* pnlFactory, PictureBoxFactory* pbxFactory, ImageListFactory* imglFactory, TextFieldFactory* tfFactory, MenuItemFactory* miFactory,MenuFactory* mnuFactory);

	PanelFactory*      GetPanelFactory();
	PictureBoxFactory* GetPictureBoxFactory();
	ImageListFactory*  GetImageListFactory();
	TextFieldFactory*  GetTextFieldFactory();
	MenuItemFactory*   GetMenuItemFactory(); 
	MenuFactory*       GetMenuFactory();

};



