#include "ArmletShell.h"

namespace ArmletShell {

/*
BitmapImage* ImagesRepository::GetImageById(sword_t id)
{
	NULLIF(id<0 || !(id < _imagesCount));

	return _images[id];
}

ImagesRepository::ImagesRepository()
{
	_usedImages  =0;
	_imagesCount =0;
}	

fresult ImagesRepository::RegisterImage( BitmapImage* bitmap )
{
	FAILIF(!(_usedImages<_imagesCount));
	_images[_imagesCount++] = bitmap;
	return SUCCESS;
}
*/
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
