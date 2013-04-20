#include "IntegrationWithKeltur.h"




fresult TODO_KELTUR_GetFontById( int fontId, Font* oFont )
{
	oFont->GlyphSize.Height =8;
	oFont->GlyphSize.Width =8;
	oFont->ID = fontId;
	oFont->Name = "KreyloFont";
	return SUCCESS;
}
