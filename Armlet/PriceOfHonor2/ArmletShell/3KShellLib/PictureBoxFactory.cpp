#include "ThreeKShell.h"

namespace ThreeKShell {

	PictureBox* PictureBoxFactory::AllocPictureBox()
	{
		return new PictureBox();
	}

	fresult PictureBoxFactory::Init( IRender* render, ImagesRepository* images )
	{
		_render = render;
		_images = images;

		return SUCCESS;
	}

	fresult PictureBoxFactory::GetPictureBox(Position pos, ubyte_t imageId, PictureBox** o_pbx)
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

	fresult PictureBoxFactory::GetPictureBox(Size sz, Position pos, ubyte_t imageId, PictureBox** o_pbx)
	{
		fresult fres;
		PictureBox* pbx = AllocPictureBox();
		FAILIF(pbx==NULL);

		fres = pbx->Init(sz, pos,_render);
		ENSURESUCCESS(fres);
		
		//Get Image
		BitmapImage* bmp = _images->GetImageById(imageId);
		FAILIF(bmp!=NULL);

		fres = pbx->SetImage(bmp->Bitmap, bmp->ImageSize);
		ENSURESUCCESS(fres);

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
 
