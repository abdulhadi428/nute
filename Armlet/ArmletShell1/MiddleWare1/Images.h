#pragma once

#ifndef _NAMESPACE_PROTECT_
#error "Use ArmletShell.h"
#endif


struct BitmapImage 
{
	Size ImageSize;
	Color* Bitmap;
};

class ImagesRepository
{
	BitmapImage* _images;
	uword_t _imagesCount;
public:
	fresult Init(BitmapImage* images, sword_t imagesCount);

	BitmapImage* GetImageById(sword_t id);
};