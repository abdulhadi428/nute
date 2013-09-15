#ifndef _NAMESPACE_PROTECT_
#error "Use ThreeKShell.h"
#endif

#define MENU_MAX_SIZE 9
struct MenuItemSettings
{
public:
	char* Text;
	char* Name;
	ImageHandle ImgHandle;
	IMenuHandler* Handler;
	bool_t Empty;
	Position ScreenOrigin;
};

typedef enum _MNU_ITEM_ORIGN {
	ItemOriginA,
	ItemOriginB,
	ItemOriginC,
	ItemOriginX,
	ItemOriginY,
	ItemOriginZ,
	ItemOriginL,
	ItemOriginE,
	ItemOriginR,
	MenuItemOriginsLast
} MenuItemOrigins;

typedef enum _MNU_KINDS {
	ABCXYZLER,
	ACY,
	BXZ,
	LER
} MenuKinds;

class MenuFactory
{
	IRender* _render;

	TextFieldFactory* _tfFactory; 
	PictureBoxFactory* _pbxFactory; 
	PanelFactory* _pnlFactory;
	MenuItemFactory* _miFactory;

	Repositories* _repositories;

	IMenuItem** allocMenuItems(ubyte_t count);
	ScatteredMenu* allocMenu();

public:

	TextFormatHandle DefaultTextFormatHandle;
	TextFormatHandle CurrentTextFormatHandle;

	TextFormatHandle DefaultEvenTextFormatHandle;
	TextFormatHandle CurrentEvenTextFormatHandle;

	bool_t DefaultInterlaced;
	bool_t CurrentInterlaced;

	MenuItemSettings Settings[MENU_MAX_SIZE];
	fresult ClearSettings();

	fresult Init(IRender* render, Repositories* reps, TextFieldFactory* tfFactory, PictureBoxFactory* pbxFactory, PanelFactory* pnlFactory, MenuItemFactory* miFactory);

	fresult GetMenu(MenuKinds kind, IMenu** o_mnu);
};
