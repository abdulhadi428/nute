#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

//TODO: Rename To ImageBox
class Image : public ScrollableControlBase
{
	uword_t const* _bitmap;
	Size _bitmapSize;
public:

	virtual fresult DrawArea( Position pos, Size size );
	fresult Init(Size size, Position position, IRender* render)
	{
		fresult fres;
		fres = ScrollableControlBase::BaseInit(size, position, render);
		return fres;
	}

	fresult SetImage(uword_t const*const bitmap, Size bitmapSize)
	{
		fresult fres = SUCCESS;
		_bitmap = bitmap;
		_bitmapSize = bitmapSize;
		return fres;
	}

	fresult SetImage(BitmapImage* bmp)
	{
		FAILIF(bmp==NULL);
		return SetImage(bmp->Bitmap, bmp->ImageSize);
	}

};

class PictureBox : public Image
{

};
