#include "ArmletShell.h"



namespace ArmletShell {


fresult ImagesRepository::Init( BitmapImage* images, uword_t imagesCount )
{
	_images = images;
	_imagesCount = imagesCount;

	return SUCCESS;
}

fresult ImagesRepository::GetImageById( ImageId id )
{
	if (ImageId < _imagesCount)
	{
		return _images[ImageId];
	}
}
}