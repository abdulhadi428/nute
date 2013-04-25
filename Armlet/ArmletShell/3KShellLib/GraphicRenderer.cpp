#include "GraphicRenderer.h"
#include "UpInterface.h"

fresult GraphicRenderer::DrawString( Position pospx, const char* str, int length, TextFormat* format )
{
	DrawTextString(pospx.Left, pospx.Top, str, length, format->FgColor, format->BgColor);

	return SUCCESS;
}

fresult GraphicRenderer::DrawRect( Position pospx, Size sizepx, Color color )
{
	DrawRect_kel(pospx.Left, pospx.Top, sizepx.Width, sizepx.Height, color);
	return SUCCESS;
}
