#include "ArmletShell.h"

namespace ArmletShell {


fresult ImagesRepository::Init( BitmapImage* images, sword_t imagesCount )
{
	_images = images;
	_imagesCount = imagesCount;

	return SUCCESS;
}

BitmapImage* ImagesRepository::GetImageById(sword_t id )
{
	if (id <0)
	{
		return NULL;
	}
	
	if (id < _imagesCount)
	{
		return &_images[id];
	}
	
	return NULL;
}
}