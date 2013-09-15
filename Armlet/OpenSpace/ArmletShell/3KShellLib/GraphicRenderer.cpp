#include "ThreeKShell.h"

namespace ThreeKShell {

fresult GraphicRenderer::DrawString( Position pospx, const char* str, uword_t length, TextFormat* format )
{
	ArmletAppSDK::DrawTextString(pospx.Left, pospx.Top, str, length, format->FgColor, format->BgColor);

	return SUCCESS;
}

fresult GraphicRenderer::DrawRect( Position pospx, Size sizepx, Color color )
{
	ArmletAppSDK::DrawRect(pospx.Left, pospx.Top, sizepx.Width, sizepx.Height, color);
	return SUCCESS;
}

fresult GraphicRenderer::DrawBitmap( Position pospx, Size sizepx, uword_t const*const bitmap, Size bitmapSize, Position bitmapReadPos )
{
	ArmletAppSDK::DrawBitmapRect(pospx.Left, pospx.Top, bitmapSize.Width, bitmapSize.Height, (Color*)bitmap, bitmapReadPos.Left, bitmapReadPos.Top, sizepx.Width, sizepx.Height);
	return SUCCESS;
}

}
