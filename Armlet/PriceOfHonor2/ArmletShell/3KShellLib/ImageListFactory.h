#ifndef _NAMESPACE_PROTECT_
#error "Use ThreeKShell.h"
#endif

class ImageListFactory
{
	ImageList* AllocImageList();
	ScaledValueImageList* AllocScaledValueImageList();
	
	sword_t* AllocSwordArray(ubyte_t len);
	BitmapImage** AllocBitmapArray (ubyte_t len);

	IRender* _render;
	ImagesRepository* _images;

	fresult CreateBitmapArrayFromHandles( ImageHandle* handles, ubyte_t len, Size* o_sz, BitmapImage*** o_bmps);

public:
	fresult Init(IRender* render, ImagesRepository* images);

	fresult GetImageList(Position pos, ImageHandle* Images, ubyte_t imgCount, ImageList** o_imgl);
	fresult GetScaledImageList(Position pos, ImageHandle* Images, sword_t MinValue, sword_t MaxValue, ubyte_t imgCount, ScaledValueImageList** o_svimgl);
	fresult GetScaledImageList(Position pos, ImageHandle* Images, sword_t* Scale, ubyte_t imgCount, ScaledValueImageList** o_svimgl);
	
};
