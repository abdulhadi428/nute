#include "ThreeKShell.h"
#include "kl_allocator.h"

namespace ThreeKShell {

static Alloc_t<PictureBox, 17> SPictureBoxArr;

	PictureBox* PictureBoxFactory::AllocPictureBox()
	{
		return SPictureBoxArr.Allocate("PictureBox"); // new PictureBox();
	}

	fresult PictureBoxFactory::Init( IRender* render, ImagesRepository* images )
	{
		_render = render;
		_images = images;

		return SUCCESS;
	}

	fresult PictureBoxFactory::GetPictureBox(Position pos, ImageHandle imageId, PictureBox** o_pbx)
	{
		fresult fres;
		Size sz;

		//Get Image
		BitmapImage* bmp = _images->GetImageById(imageId);
		FAILIF(bmp==NULL);

		sz.data = bmp->ImageSize.data;
		fres = GetPictureBox(sz, pos, bmp, o_pbx);
		ENSURESUCCESS(fres);

		return SUCCESS;
	}

	fresult PictureBoxFactory::GetPictureBox(Size sz, Position pos, ImageHandle imageId, PictureBox** o_pbx)
	{
		fresult fres;
		PictureBox* pbx = AllocPictureBox();
		FAILIF(pbx==NULL);

		fres = pbx->Init(sz, pos,_render);
		ENSURESUCCESS(fres);

		//Get Image
		BitmapImage* bmp = _images->GetImageById(imageId);
		if(bmp==NULL)
		{
			Size sz;
			sz.data=0;
			fres = pbx->SetImage(NULL, sz);
			ENSURESUCCESS(fres);
		}
		else
		{
			fres = pbx->SetImage(bmp->Bitmap, bmp->ImageSize);
			ENSURESUCCESS(fres);
		}

		*o_pbx=pbx;
		return SUCCESS;
	}

	fresult PictureBoxFactory::GetPictureBox(Size sz, Position pos, BitmapImage* bmp, PictureBox** o_pbx)
	{

		fresult fres;
		PictureBox* pbx = AllocPictureBox();
		FAILIF(pbx==NULL);

		fres = pbx->Init(sz, pos,_render);
		ENSURESUCCESS(fres);

		fres = pbx->SetImage(bmp->Bitmap, bmp->ImageSize);
		ENSURESUCCESS(fres);

		*o_pbx=pbx;
		return SUCCESS;
	}

}

