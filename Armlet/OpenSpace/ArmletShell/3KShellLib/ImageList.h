#ifndef _NAMESPACE_PROTECT_
#error "Use ThreeKShell.h"
#endif
//#pragma once

class ImageListBase : public PictureBoxBase
{
protected:
	BitmapImage** _Images;
	ubyte_t _ImagesCount;
	sbyte_t _ActiveImageIndex;

	fresult BaseInit(BitmapImage** images, ubyte_t imgCount, Size size, Position position, IRender* render);

	fresult SetImageIndexInternal(sbyte_t imageIndex);
	sbyte_t GetImageIndexInternal();

public:
};

class ImageList : public ImageListBase
{
public:
	fresult Init(BitmapImage** images, ubyte_t imgCount, Size size, Position position, IRender* render);
	fresult SetImageIndex(sbyte_t imageIndex);
	sbyte_t GetImageIndex();
};

class ScaledValueImageList: public ImageListBase
{
protected:	
	const sword_t* _Scale;
public:
	fresult Init(const sword_t* Scale, BitmapImage** images, ubyte_t imgCount, Size size, Position position, IRender* render);
	fresult SetValue(sword_t numericValue);
};
