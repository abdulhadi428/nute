#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

typedef enum _MNU_ITEM_STYLE {
	Default,
	TextOnly,
	ImageOnly,
	ImageLeft,
	ImageRight,
	ImageTop,
	ImageBottom,
	CustomControl,
	MenuItemStylesLast
} MenuItemStyles;

class MenuItemFactory
{

	IRender* _render;
public:
	MenuItemFactory(IRender* render, TextFieldFactory tfFactory);

	fresult GetPanel(Size sz, Position pos, ubyte_t controlsCount, Panel* o_pnl);
};
