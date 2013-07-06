#include "ArmletShell.h"
#include "ArmletApi.h"

using namespace ArmletApi;

void DrawGlyphPoint(
	int gx, int gy,
	int x, int y, int cx,int cy, int scaleFactor, int fillFactor,
	short color, short backColor)
{
	if ((cx < gx*scaleFactor)||(cy<gy*scaleFactor))
	{
		return;
	}
	//now we know that we should set some of scaleFactor^2 pixels
	fillFactor -=1;
	for (int i=0;i<scaleFactor;i++)
		for(int j=0;j<scaleFactor;j++)
		{
			if ((i>fillFactor)||(j>fillFactor)) {
				color = backColor;
			}
			DrawPixel(
				x + scaleFactor*gx + i,
				y + scaleFactor*(7-gy)+j,
				color);
		}
}

void DrawGlyph(int x, int y, unsigned char index,
	int cx,int cy, int scaleFactor, int fillFactor,
	short foreColor, short backColor)
{
    for (int vert = 0; vert < 6; vert++)
    {
		int vertValue = /*ArmletShell::Fonts::*/Font_6x8_Data[index][vert];
        for (int i = 0; i < 8; i++)
        {
            int mask = 1 << i;
            if ((vertValue & mask) != 0)
            {
				DrawGlyphPoint(vert,i,x,y,cx,cy,scaleFactor,fillFactor,foreColor,backColor);
			} else {
				DrawGlyphPoint(vert,i,x,y,cx,cy,scaleFactor,fillFactor,backColor,backColor);
			}
        }
    }
}

//dx,dy - skew from top-left; cx,cy - size to be drawn;
//int scaleFactor - pixels=scaleFactor^2 (1->1;2->4;3->9)
//int fillFactor - part of scaleFactor to be drawn
void Show_Glyphs(int dx, int dy, int cx,int cy, 
	int scaleFactor, int fillFactor)
{
    int x = 0;
    int y = 0;
	if ((cx-dx<0)||(cy-dy<0)) {
		return;
	}
	if (scaleFactor<=1)
		scaleFactor=1;
	if (scaleFactor>5)
		scaleFactor=5;
	if (fillFactor<=0)
		fillFactor = 1;
	if (fillFactor>scaleFactor)
		fillFactor=scaleFactor;
	for (int index=0; index<256; index++) {
		DrawGlyph(x+dx,y+dy,index,
			cx-dx,cy-dy,scaleFactor,fillFactor,
			0x0F00,0x0FFF);
		x += cx;
		if (x>(SCREENX-cx)) {
			x=0;
			y+=cy;
		}
		if (y>(SCREENY-cy)) {
			break;
		}
	}
}

namespace ArmletShell {

	//Video device - __FUTURE:
	//Simulated video memory - 160x128x16xARGB
	//LUT
	//Scrolling
	//char HScroll;
	//char VScroll;

	void Clear(Color backColor)
	{
		for (int i=0; i<SCREENX; i++)
			for(int j=0; j<SCREENY; j++)
			{
				DrawPixel(i,j,backColor);
			}
	}

	void DrawTextString(uword_t x0, uword_t y0, 
						const char* string, uword_t sz, Color foreColor, Color backColor)
	{
		uword_t x = x0;
		uword_t y = y0;
		uword_t cx = 6;
		uword_t cy = 8;
		if (y+cy > SCREENY) {
			return;
		}
		for(uword_t i=0; i<sz; i++) {
			DrawGlyph(x,y,string[i],
				cx,cy,1,1,foreColor,backColor);
			x += cx;
			if (x+cx > SCREENX) {
				return;
			}
		}
		return;
	}

	void DrawRect(uword_t x, uword_t y, uword_t sx, uword_t sy, Color color)
	{
		for (uword_t i=0; i<sx; i++)
			for(uword_t j=0; j<sy; j++)
			{
				uword_t px=x+i;
				uword_t py=y+j;
				if (px >= SCREENX) break;
				if (py >= SCREENY) return;
				DrawPixel(px,py,color);
			}
	}

	void DrawBitmap(uword_t x, uword_t y, uword_t sx, uword_t sy, Color* bitmap)
	{
		uword_t k=0;
		for(uword_t j=0; j<sy; j++)
		{
			for (uword_t i=0; i<sx; i++)
			{
				uword_t px=x+i;
				uword_t py=y+j;
				if (px >= SCREENX) break;
				if (py >= SCREENY) return;
				DrawPixel(px,py,bitmap[k++]);
			}
		}
	}

	void DrawBitmapRect(
			uword_t x, uword_t y, uword_t sx, uword_t sy, Color* bitmap,
			uword_t dx, uword_t dy, uword_t sdx, uword_t sdy)
	{
		//TODO HACK - need to implement
		DrawBitmap(x,y,sx,sy,bitmap);
	}

} //namespace
