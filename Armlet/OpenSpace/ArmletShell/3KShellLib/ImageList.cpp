#include "ThreeKShell.h"

namespace ThreeKShell {

fresult ImageListBase::BaseInit(BitmapImage** images, ubyte_t imgCount, Size size, Position position, IRender* render)
{
	fresult fres;
	
	fres = PictureBoxBase::BaseInit(size, position, render);
	ENSURESUCCESS(fres);

	_Images = images;
	_ImagesCount = imgCount;

	fres = SetImageIndexInternal(-1);
	ENSURESUCCESS(fres);
	
	return SUCCESS;
}

fresult ImageListBase::SetImageIndexInternal(sbyte_t imageIndex)
{
	fresult fres;
	FAILIF(! (imageIndex < _ImagesCount));
	
	if (imageIndex == -1)
	{
		fres= SetImageInternal(NULL);
		ENSURESUCCESS(fres);
	}
	else
	{
		fres= SetImageInternal(_Images[imageIndex]);
		ENSURESUCCESS(fres);
	}

	_ActiveImageIndex = imageIndex;

	return fres;
}

sbyte_t ImageListBase::GetImageIndexInternal()
{
	return _ActiveImageIndex;
}


fresult ImageList::Init( BitmapImage** images, ubyte_t imgCount, Size size, Position position, IRender* render )
{
	return BaseInit(images, imgCount, size, position, render);
}

fresult ImageList::SetImageIndex( sbyte_t imageIndex )
{
	return SetImageIndexInternal(imageIndex);
}

sbyte_t ImageList::GetImageIndex()
{
	return GetImageIndexInternal();
}

fresult ScaledValueImageList::Init( const sword_t* Scale, BitmapImage** images, ubyte_t imgCount, Size size, Position position, IRender* render )
{
	_Scale = Scale;
	return BaseInit(images, imgCount, size, position, render);
}

fresult ScaledValueImageList::SetValue( sword_t numericValue)
{
	fresult fres;
	ubyte_t imgIndex = 0;
	//scale value to image
	for (int i=0; i< _ImagesCount; i++)
	{
		if (numericValue>=_Scale[i])
		{
			imgIndex = i;
		}
		else
			break;
	}

	//set image
	fres = SetImageIndexInternal(imgIndex);
	ENSURESUCCESS(fres);

	return SUCCESS;
}
}