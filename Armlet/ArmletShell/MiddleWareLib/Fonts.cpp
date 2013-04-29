#include "ArmletShell.h"

namespace ArmletShell {

//#define _FONT_PROTECTION_
//namespace Fonsts {
	#include "KreylFont.cpp"
//}
//#undef _FONT_PROTECTION_

	fresult GetFontById( ubyte_t fontId, FontDescription* oFont )
	{
		if (fontId != 1)
			return GENERAL_ERROR;

		oFont->GlyphSize.Height = 8;
		oFont->GlyphSize.Width = 6;
		oFont->ID = fontId;
		oFont->Name = "KreyloFont";
		return SUCCESS;
	}

	fresult GetTextFormat(ubyte_t fontId, Color bgColor, Color fgColor, TextFormat* oFormat )
	{
		fresult fres;
		fres = GetFontById(fontId, &(oFormat->Font));
		if (fres != SUCCESS)
			return fres;

		oFormat->BgColor= bgColor;
		oFormat->FgColor = fgColor;

		return SUCCESS;
	}

} //namespace

