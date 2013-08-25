#include "ArmletShell.h"

namespace ArmletShell {

//old rep
fresult ImagesRepository_::Init( BitmapImage* images, sword_t imagesCount )
{
	_images = images;
	_imagesCount = imagesCount;

	return SUCCESS;
}

BitmapImage* ImagesRepository_::GetImageById(sword_t id )
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
//old rep
}
