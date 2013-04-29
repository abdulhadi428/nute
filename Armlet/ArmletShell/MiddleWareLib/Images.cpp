#include "ArmletShell.h"

namespace ArmletShell {


fresult ImagesRepository::Init( BitmapImage* images, uword_t imagesCount )
{
	_images = images;
	_imagesCount = imagesCount;

	return SUCCESS;
}

BitmapImage* ImagesRepository::GetImageById(ubyte_t id )
{
	if (id < _imagesCount)
	{
		return &_images[id];
	}
}
}