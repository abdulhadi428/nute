#include "ArmletShell.h"
#include "GraphicRenderer.h"

namespace ThreeKShell {

fresult GraphicRenderer::DrawString( Position pospx, const char* str, uword_t length, TextFormat* format )
{
	ArmletShell::DrawTextString(pospx.Left, pospx.Top, str, length, format->FgColor, format->BgColor);

	return SUCCESS;
}

fresult GraphicRenderer::DrawRect( Position pospx, Size sizepx, Color color )
{
	ArmletShell::DrawRect(pospx.Left, pospx.Top, sizepx.Width, sizepx.Height, color);
	return SUCCESS;
}

fresult GraphicRenderer::DrawBitmap( Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos )
{
	ArmletShell::DrawBitmapRect(pospx.Left, pospx.Top, bitmapSize.Width, bitmapSize.Height, (Color*)bitmap, bitmapReadPos.Left, bitmapReadPos.Top, sizepx.Width, sizepx.Height);
	return SUCCESS;
}

}
