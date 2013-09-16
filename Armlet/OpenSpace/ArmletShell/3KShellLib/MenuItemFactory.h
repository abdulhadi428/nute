#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

typedef enum _MNU_ITEM_STYLE {
	HotkeyItem,
	TextOnly,
	ImageOnly,
	ImageLeft,
	ImageRight,
	ImageTop,
	ImageBottom,
	MenuItemStylesLast
} MenuItemStyles;

//TODO: move out of here, that a general purpose enum
typedef enum _H_ALIGN {
	ToRight,
	ToLeft,
	CenterHorizontaly
} HorizontalAlignmentStyles;

typedef enum _V_ALIGN {
	ToBottom,
	ToTop,
	CenterVertically
} VerticalAlignmentStyles;

struct Alignment
{
public:
	VerticalAlignmentStyles VAlign;
	HorizontalAlignmentStyles HAlign;
};

class MenuItemFactory
{
	IRender* _render;
	
	TextFieldFactory* _tfFactory; 
	PictureBoxFactory* _pbxFactory; 
	PanelFactory* _pnlFactory;
	Repositories* _repositories;

	MenuItem* allocMenuItem();
public:

	TextFormatHandle DefaultTextFormatHandle;
	TextFormatHandle CurrentTextFormatHandle;

	ubyte_t DefaultTextLines;
	ubyte_t CurrentTextLines;

	ubyte_t DefaultTextToImageOffset;
	ubyte_t CurrentTextToImageOffset;

	Size DefaultTextFieldSize;
	Size CurrentTextFieldSize;

	bool_t DefaultTextFieldVAutoSize;
	bool_t CurrentTextFieldVAutoSize;

	bool_t DefaultTextFieldVSizeByLines;
	bool_t CurrentTextFieldVSizeByLines;

	bool_t DefaultTextFieldHAutoSize;
	bool_t CurrentTextFieldHAutoSize;


	fresult Init(IRender* render, Repositories* reps, TextFieldFactory* tfFactory, PictureBoxFactory* pbxFactory, PanelFactory* pnlFactory);

	fresult GetMenuItem(Position origin, Alignment align, MenuItemStyles style, ImageHandle himg, char* text, ButtonState shrtcut, IMenuHandler* handler, char* name, IMenuItem** o_mi);
};

