#include "ThreeKShell.h"
#include "kl_allocator.h"

static Alloc_t<ImageList, 0> SImageListArr;
static Alloc_t<sword_t, 10> SSwordtArr;
static Alloc_t<ScaledValueImageList, 2> SScaledValueImageListArr;
static Alloc_t<BitmapImage*, 10> SBitmapImagePtrArr;

ImageList* ImageListFactory::AllocImageList()
{
	return SImageListArr.Allocate("ImageList"); // new ImageList();
}

sword_t* ImageListFactory::AllocSwordArray( ubyte_t len )
{
	return SSwordtArr.Allocate(len, "sword_t"); // new sword_t[len];
}


ScaledValueImageList* ImageListFactory::AllocScaledValueImageList()
{
	return SScaledValueImageListArr.Allocate("ScaledValueImageList"); // new ScaledValueImageList();
}


BitmapImage** ImageListFactory::AllocBitmapArray( ubyte_t len )
{
	return SBitmapImagePtrArr.Allocate(len, "BitmapImage*"); // new BitmapImage*[len];
}



fresult ImageListFactory::Init( IRender* render, ImagesRepository* images )
{
	_render = render;
	_images = images;

	return SUCCESS;
}

fresult ImageListFactory::GetImageList( Position pos, ImageHandle* Images, ubyte_t imgCount, ImageList** o_imgl )
{
	fresult fres;
	ImageList* imgl = AllocImageList();

	Size sz;
	BitmapImage** bmps;

	//convert handles to bimaps
	fres = CreateBitmapArrayFromHandles(Images, imgCount, &sz, &bmps);
	ENSURESUCCESS(fres);


	fres = imgl->Init(bmps, imgCount, sz, pos, _render);
	ENSURESUCCESS(fres);

	o_imgl = &imgl;
	return SUCCESS;
}

fresult ImageListFactory::GetScaledImageList( Position pos, ImageHandle* Images, sword_t MinValue, sword_t MaxValue, ubyte_t imgCount, ScaledValueImageList** o_svimgl )
{
	//create scales array
	sword_t* scales = AllocSwordArray(imgCount);
	if (imgCount >2)
	{
		scales[0] = MinValue;
		scales[imgCount-1] = MaxValue;
	}

	sword_t step = (MaxValue-MinValue)/imgCount;
	sword_t lastValue =MinValue;
	for (int i=0;i<imgCount-2;i++)
	{
		scales[i+1] = lastValue+step;
		lastValue = scales[i+1];
	}

	return GetScaledImageList(pos, Images, scales, imgCount, o_svimgl);
}

fresult ImageListFactory::GetScaledImageList( Position pos, ImageHandle* Images, sword_t* Scale, ubyte_t imgCount, ScaledValueImageList** o_svimgl )
{
	fresult fres;
	ScaledValueImageList* imgl = AllocScaledValueImageList();

	Size sz;
	BitmapImage** bmps = NULL;

	//convert handles to bimaps
	fres = CreateBitmapArrayFromHandles(Images, imgCount, &sz, &bmps);
	ENSURESUCCESS(fres);

	fres = imgl->Init(Scale, bmps, imgCount, sz, pos, _render);
	ENSURESUCCESS(fres);

	*o_svimgl = imgl;
	return SUCCESS;
}

fresult	ImageListFactory::CreateBitmapArrayFromHandles( ImageHandle* handles, ubyte_t len, Size* o_sz, BitmapImage*** o_bmps)
{
	Size sz;
	sz.Height =0;
	sz.Width =0;
	BitmapImage** bmps = AllocBitmapArray(len);

	for (int i=0; i<len;i++)
	{
		bmps[i] = _images->GetImageById(handles[i]);
		FAILIF(bmps[i]==NULL);

		//calc max dimensions
		if (bmps[i]->ImageSize.Height > sz.Height)
		{
			sz.Height = bmps[i]->ImageSize.Height;
		}

		if (bmps[i]->ImageSize.Width > sz.Width)
		{
			sz.Width = bmps[i]->ImageSize.Width;
		}
	}

	*o_sz = sz;
	*o_bmps = bmps;

	return SUCCESS;
}
