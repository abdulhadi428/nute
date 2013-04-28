#include "Image.h"
#include "Dimensions.h"

fresult Image::DrawArea( Position pos, Size size )
{
	fresult fres = SUCCESS;

	Size sizeToDraw;
	Position posToDraw;

	if (GetRectClipping(pos, size, _Position,_Size, &posToDraw, &sizeToDraw))
	{
		//_ScrollPosition
		fres = _render->DrawBitmap(posToDraw, sizeToDraw, _bitmap, _bitmapSize, _ScrollPosition);
		if (fres != SUCCESS)
			return fres;
	}
	
	return SUCCESS;
};
