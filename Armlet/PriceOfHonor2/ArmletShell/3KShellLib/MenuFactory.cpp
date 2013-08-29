#include "ThreeKShell.h"

namespace ThreeKShell {

#define MENU_ITEM_ROW_1_TOP 24
#define MENU_ITEM_ROW_2_TOP 40
#define MENU_ITEM_ROW_3_TOP 56
#define MENU_ITEM_ROW_4_TOP 72
#define MENU_ITEM_ROW_5_TOP 88
#define MENU_ITEM_ROW_6_TOP 104

#define MENU_ITEM_H_SCREENBORDER_OFFSET 1
#define MENU_ITEM_V_SCREENBORDER_OFFSET 1

#define MENU_ITEM_A_TOP   MENU_ITEM_ROW_1_TOP
#define	MENU_ITEM_A_LEFT  MENU_ITEM_H_SCREENBORDER_OFFSET
#define MENU_ITEM_B_TOP	  MENU_ITEM_ROW_3_TOP	
#define	MENU_ITEM_B_LEFT  MENU_ITEM_H_SCREENBORDER_OFFSET
#define MENU_ITEM_C_TOP	  MENU_ITEM_ROW_5_TOP	
#define	MENU_ITEM_C_LEFT  MENU_ITEM_H_SCREENBORDER_OFFSET

#define MENU_ITEM_X_TOP	  MENU_ITEM_ROW_2_TOP
#define	MENU_ITEM_X_LEFT  SCREENX-MENU_ITEM_H_SCREENBORDER_OFFSET
#define MENU_ITEM_Y_TOP	  MENU_ITEM_ROW_4_TOP	
#define	MENU_ITEM_Y_LEFT  SCREENX-MENU_ITEM_H_SCREENBORDER_OFFSET
#define MENU_ITEM_Z_TOP   MENU_ITEM_ROW_6_TOP
#define	MENU_ITEM_Z_LEFT  SCREENX-MENU_ITEM_H_SCREENBORDER_OFFSET

#define MENU_ITEM_L_TOP	  SCREENY - MENU_ITEM_V_SCREENBORDER_OFFSET
#define	MENU_ITEM_L_LEFT  33
#define MENU_ITEM_E_TOP   SCREENY - MENU_ITEM_V_SCREENBORDER_OFFSET
#define	MENU_ITEM_E_LEFT  96
#define MENU_ITEM_R_TOP   SCREENY - MENU_ITEM_V_SCREENBORDER_OFFSET
#define	MENU_ITEM_R_LEFT  129

IMenuItem** MenuFactory::allocMenuItems( ubyte_t count )
{
	NULLIF(!(count>0));
	return new IMenuItem*[count];
}

ScatteredMenu* MenuFactory::allocMenu()
{
	return new ScatteredMenu(); 
}

fresult MenuFactory::Init( IRender* render, Repositories* reps, TextFieldFactory* tfFactory, PictureBoxFactory* pbxFactory, PanelFactory* pnlFactory, MenuItemFactory* miFactory )
{
	fresult fres;
	_render = render;
	_tfFactory = tfFactory;
	_pbxFactory = pbxFactory;
	_pnlFactory = pnlFactory;
	_repositories = reps;
	_miFactory = miFactory;

	//get text formats
	DefaultTextFormatHandle =TF_MENU;
	CurrentTextFormatHandle = DefaultTextFormatHandle;

	DefaultEvenTextFormatHandle = TF_MENUEVEN;
	CurrentEvenTextFormatHandle = DefaultTextFormatHandle;

	DefaultInterlaced = TRUE;
	CurrentInterlaced = TRUE;

	fres = ClearSettings();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult MenuFactory::ClearSettings()
{
	for (ubyte_t i =0; i<MENU_MAX_SIZE; i++)
	{
		Settings[i].Handler = NULL;
		Settings[i].Text = NULL;
		Settings[i].ImgHandle = INVALID_HANDLE;
		Settings[i].Empty = TRUE;
		Position orgn;
		switch (i)
		{
			case ItemOriginA: 
				orgn.Top =	MENU_ITEM_A_TOP;
				orgn.Left = MENU_ITEM_A_LEFT;
			break;
			case ItemOriginB:
				orgn.Top =	MENU_ITEM_B_TOP;
				orgn.Left = MENU_ITEM_B_LEFT;
			break;
			case ItemOriginC:
				orgn.Top =	MENU_ITEM_C_TOP;
				orgn.Left = MENU_ITEM_C_LEFT;
			break;
			case ItemOriginX:
				orgn.Top =	MENU_ITEM_X_TOP;
				orgn.Left = MENU_ITEM_X_LEFT;
			break;
			case ItemOriginY: 
				orgn.Top =	MENU_ITEM_Y_TOP;
				orgn.Left = MENU_ITEM_Y_LEFT;
			break;
			case ItemOriginZ:
				orgn.Top =	MENU_ITEM_Z_TOP;
				orgn.Left = MENU_ITEM_Z_LEFT;
			break;
			case ItemOriginL:
				orgn.Top =	MENU_ITEM_L_TOP;
				orgn.Left = MENU_ITEM_L_LEFT;
			break;
			case ItemOriginE:
				orgn.Top =	MENU_ITEM_E_TOP;
				orgn.Left = MENU_ITEM_E_LEFT;
			break;
			case ItemOriginR: 
				orgn.Top =	MENU_ITEM_R_TOP;
				orgn.Left = MENU_ITEM_R_LEFT;
			break;
		default:
			return GENERAL_ERROR;
		}
		Settings[i].ScreenOrigin = orgn;
	}
	return SUCCESS;
}

fresult MenuFactory::GetMenu( MenuKinds kind, IMenu** o_mnu )
{
	ubyte_t menuItemsCount =0;
	fresult fres;

	for (ubyte_t i=0; i< MENU_MAX_SIZE; i++ )
	{
		if (Settings[i].Empty != TRUE)
		{
			menuItemsCount++;
		}
	}
	if (menuItemsCount==0)
	{
		return SUCCESS;
	}

	//alloc menu items
	IMenuItem* mi;
	IMenuItem** menuItems = allocMenuItems(menuItemsCount);
	ubyte_t currItemIndex = 0;
	FAILIF(menuItems==NULL);

	//create items
	for (ubyte_t i=0; i< MENU_MAX_SIZE; i++ )
	{

		if (Settings[i].Empty != TRUE)
		{
			FAILIF(!(currItemIndex<menuItemsCount));
			//item styles
			bool_t hasText = FALSE;
			bool_t hasImage = FALSE;
			MenuItemStyles style = HotkeyItem;
			TextFormatHandle tfmt = CurrentTextFormatHandle;
			TextFormatHandle netfmt = CurrentTextFormatHandle;
			TextFormatHandle etfmt = CurrentEvenTextFormatHandle;

			if (Settings[i].Text != NULL)
			{
				hasText = TRUE;
				style = TextOnly;

				if (!CurrentInterlaced)
				{
					etfmt = CurrentTextFormatHandle;
				}
			}

			if (Settings[i].ImgHandle != INVALID_HANDLE)
			{
				hasImage = TRUE;
				style = ImageOnly;
			}

			Alignment align;
			ButtonState shrtcut = NO_BUTTON;

			if (i == ItemOriginA || i == ItemOriginB || i==ItemOriginC)
			{
				if (i == ItemOriginA)
				{
					shrtcut = BUTTON_A;
					tfmt = netfmt;
				}
				else if (i == ItemOriginB)
				{
					shrtcut = BUTTON_B;
					tfmt = netfmt;
				}
				else if (i ==ItemOriginC)
				{
					shrtcut = BUTTON_C;
					tfmt = netfmt;
				}
				else
				{
					return GENERAL_ERROR;
				}

				if (hasText && hasImage)
				{
					style = ImageLeft;
				}

				align.HAlign = ToRight;
				align.VAlign = CenterVertically;
			}
			else if (i == ItemOriginX || i == ItemOriginY || i==ItemOriginZ)
			{
				if (i == ItemOriginX)
				{
					shrtcut = BUTTON_X;
					tfmt = etfmt;
				} 
				else if (i == ItemOriginY)
				{
					shrtcut = BUTTON_Y;
					tfmt = etfmt;
				} 
				else if (i ==ItemOriginZ)
				{
					shrtcut = BUTTON_Z;
					tfmt = etfmt;
				}
				else
				{
					return GENERAL_ERROR;
				}

				if (hasText && hasImage)
				{
					style = ImageRight;
				}

				align.HAlign = ToLeft;
				align.VAlign = CenterVertically;
			}
			else if (i == ItemOriginL || i == ItemOriginE || i==ItemOriginR)
			{
				if (i == ItemOriginL)
				{
					shrtcut = BUTTON_L;
				} 
				else if (i == ItemOriginE)
				{
					shrtcut = BUTTON_E;
				} 
				else if (i ==ItemOriginR)
				{
					shrtcut = BUTTON_R;
				}
				else
				{
					return GENERAL_ERROR;
				}

				tfmt = netfmt;

				if (hasText && hasImage)
				{
					style = ImageBottom;
				}

				align.HAlign = CenterHorizontaly;
				align.VAlign = ToTop;
			}

			_miFactory->CurrentTextFormatHandle = tfmt;
			fres = _miFactory->GetMenuItem(Settings[i].ScreenOrigin, align, style, Settings[i].ImgHandle, Settings[i].Text, shrtcut, Settings[i].Handler, &mi);
			ENSURESUCCESS(fres);
			_miFactory->CurrentTextFormatHandle = _miFactory->DefaultTextFormatHandle;
			menuItems[currItemIndex] = mi;
			currItemIndex++;
		}
	}
	//Creating mnu
	ScatteredMenu* mnu = allocMenu();
	FAILIF(mnu==NULL);
	fres = mnu->Init(menuItems, menuItemsCount);
	ENSURESUCCESS(fres);

	*o_mnu = mnu;

	return SUCCESS;
}
}