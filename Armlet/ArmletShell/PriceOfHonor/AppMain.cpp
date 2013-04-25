#include <string.h>
#include "UpInterface.h"
#include "AppMain.h"
#include "TextField.h"
#include "GraphicRenderer.h"

TextField tf;
GraphicRenderer rend;

void __stdcall AppOnButtonClick(int button)
{
	if (button == BUTTON_Y) {
		Position pos = tf.GetScrollPosition();
		pos.Top++;
		tf.SetScrollPosition(pos);
		Clear(0);
		tf.Draw();
	}
	if (button == BUTTON_X) {
		Position pos = tf.GetScrollPosition();
		if (pos.Top==0)
			return;
		pos.Top--;
		tf.SetScrollPosition(pos);
		Clear(0);
		tf.Draw();
	}
	return;
}

void __stdcall AppOnButtonHold(int button)
{
	return;
}

int gx=0;
int gy=0;
int gc=0x0000;
bool __stdcall MoveBoxByTimer()
{
	DrawRect_kel(gx,gy,30,30,0);
	gc+=0x111;
	if (gc>=0x1000) gc=0x0000;
	gx+=15; gy+=5;
	if (gx>SCREENX) gx-=SCREENX;
	if (gy>SCREENY) gy-=SCREENY;
	DrawRect_kel(gx,gy,30,30,gc);
	return false;
}

void __stdcall AppMainThread(void* param)
{
	const char* str = "PRICE of HONOR!";
	DrawTextString(10,10,str,strlen(str),0,0);

	Position pos;
	Size size,size2;	

	Clear(0);
	char buff[56];
	size2.Height = 7; size2.Width = 8;
	InitPositionAndSize(10,10,80,80, &pos, &size);
	tf.Init(size, pos, buff, size2, &rend);
	str = 
		"Статус: "
		"Хиты: 12"
		"Отсек №4"
		"        "
		"В отсеке"
		"взрыв,  "
		"радиация";

	tf.SetText(str);
	tf.Draw();
	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);
	RequestTimer(300, MoveBoxByTimer);
}