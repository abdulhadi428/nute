#include "Internal.h"

//Simulated video memory - 160x128x16xARGB
short VideoMemory[160*128];

//Video device
//LUT
//Scrolling
//char HScroll;
//char VScroll;
//void InitDrawing();

void DrawPixel(int x, int y, short c)
{
			VideoMemory[160*y+x] = c;
}

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
        int vertValue = Font_6x8_Data[index][vert];
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

void DrawTextString(int x0, int y0, const char* string, int sz, short foreColor, short backColor)
{
	int x = x0;
	int y = y0;
	int cx = 6;
	int cy = 8;
	if (y>(SCREENY-cy)) {
		return;
	}
	for(int i=0;i<sz;i++) {
		DrawGlyph(x,y,string[i],
			cx,cy,1,1,foreColor,backColor);
		x += cx;
		if (x>(SCREENX-cx)) {
			return;
		}
	}
	return;
}

void Clear(short backColor)
{
	for (int i=0; i<SCREENX; i++)
		for(int j=0; j<SCREENY; j++)
		{
			DrawPixel(i,j,backColor);
		}
}

void DrawRect_kel(int x, int y, int sx, int sy, short color)
{
	if (x<=0) return;
	if (y<=0) return;
	if (sx<=0) return;
	if (sy<=0) return;
	for (int i=0; i<sx; i++)
		for(int j=0; j<sy; j++)
		{
			int px=x+i;
			int py=y+j;
			if (px>=SCREENX) return;
			if (py>=SCREENY) return;
			DrawPixel(px,py,color);
		}
}

void DrawBitmap_kel(int x, int y, int sx, int sy, short* bitmap)
{
	if (x<=0) return;
	if (y<=0) return;
	if (sx<=0) return;
	if (sy<=0) return;
	int k=0;
	for(int j=0; j<sy; j++)
	{
		for (int i=0; i<sx; i++)
		{
			int px=x+i;
			int py=y+j;
			if (px>=SCREENX) return;
			if (py>=SCREENY) return;
			DrawPixel(px,py,bitmap[k++]);
		}
	}
}

void DrawBitmapRect_kel(int x, int y, int sx, int sy, short* bitmap, int dx, int dy, int sdx, int sdy)
{
	//TODO HACK - need to implement
	DrawBitmap_kel(x,y,sx,sy,bitmap);
}
