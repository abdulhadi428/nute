#pragma once

#ifndef _NAMESPACE_PROTECT_
#error "Use ArmletShell.h"
#endif


struct BitmapImage 
{
	Size ImageSize;
	Color* Bitmap;
};

typedef ubyte_t ImageId;

class ImagesRepository
{
	BitmapImage* _images;
	uword_t _imagesCount;
public:
	fresult Init(BitmapImage* images, uword_t imagesCount);

	fresult GetImageById(ImageId id);
};