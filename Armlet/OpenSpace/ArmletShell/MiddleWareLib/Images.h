#pragma once

#ifndef _NAMESPACE_PROTECT_
#error "Use ArmletShell.h"
#endif

struct BitmapImage 
{
	Size ImageSize;
	Color* Bitmap;
};

#define MAX_IMAGES 40

class ImagesRepository_
{
	BitmapImage* _images;
	uword_t _imagesCount;
public:
	fresult Init(BitmapImage* images, sword_t imagesCount);

	BitmapImage* GetImageById(sword_t id);
};
