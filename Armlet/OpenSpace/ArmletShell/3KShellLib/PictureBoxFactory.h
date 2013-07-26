#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class PictureBoxFactory
{
	PictureBox* AllocPictureBox();

	IRender* _render;
	ImagesRepository* _images;

public:
	fresult Init(IRender* render, ImagesRepository* images);

	fresult GetPictureBox(Position pos, ubyte_t imageId, PictureBox** o_pbx);
	fresult GetPictureBox(Size sz, Position pos, ubyte_t imageId, PictureBox** o_pbx);
	fresult GetPictureBox(Size sz, Position pos, BitmapImage* bmp, PictureBox** o_pbx);
};
