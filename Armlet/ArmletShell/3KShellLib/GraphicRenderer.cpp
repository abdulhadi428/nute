#include "GraphicRenderer.h"
#include "UpInterface.h"

fresult GraphicRenderer::DrawString( Position pospx, const char* str, int length, TextFormat* format )
{
	DrawTextString(pospx.Left, pospx.Top, str, length, format->FgColor, format->BgColor);

	return SUCCESS;
}
