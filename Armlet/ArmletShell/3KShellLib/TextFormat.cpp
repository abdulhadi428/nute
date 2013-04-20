#include "project.h"
#include "TextFormat.h"
#include "IntegrationWithKeltur.h"

fresult GetTextFormat( int fontId, Color bgColor, Color fgColor, TextFormat* oFormat )
{
	fresult fres;
	fres = TODO_KELTUR_GetFontById(fontId, &(oFormat->Font));
	if (fres != SUCCESS)
		return fres;

	oFormat->BgColor= bgColor;
	oFormat->FgColor = fgColor;

	return SUCCESS;
}

