#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class PictureBoxBase : public ScrollableControlBase
{
protected:
	uword_t const* _bitmap;
	Size _bitmapSize;
	
	fresult SetImageInternal(uword_t const*const bitmap, Size bitmapSize)
	{
		fresult fres = SUCCESS;
		_bitmap = bitmap;
		_bitmapSize = bitmapSize;
		return fres;
	}

	fresult SetImageInternal(BitmapImage* bmp)
	{
		if (bmp==NULL)
		{
			Size sz;
			sz.Height=0;
			sz.Width=0;
			return SetImageInternal(NULL, sz);
		}
		else
		{
			return SetImageInternal(bmp->Bitmap, bmp->ImageSize);
		}
	}

public:
	virtual fresult DrawArea( Position pos, Size size );
	fresult BaseInit(Size size, Position position, IRender* render)
	{
		fresult fres;
		fres = ScrollableControlBase::BaseInit(size, position, render);
		return fres;
	}



};

class PictureBox : public PictureBoxBase
{
public:
	fresult Init(Size size, Position position, IRender* render)
	{
		fresult fres;
		fres = PictureBoxBase::BaseInit(size, position, render);
		return fres;
	}

	fresult SetImage(uword_t const*const bitmap, Size bitmapSize)
	{
		return PictureBoxBase::SetImageInternal(bitmap, bitmapSize);
	}

	fresult SetImage(BitmapImage* bmp)
	{
		FAILIF(bmp==NULL);
		return PictureBoxBase::SetImageInternal(bmp);
	}

};