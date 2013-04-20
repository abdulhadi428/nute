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

void __stdcall AppMainThread(void* param)
{
	const char* str = "PRICE of HONOR!";
	DrawTextString(10,10,str,strlen(str),0,0);

	Position pos;
	Size size,size2;	

	Clear(0);
	InitPositionAndSize(10,10,80,80, &pos, &size);
	tf.Init(size, pos, &rend);
	str = 
		"Статус: "
		"Хиты: 12"
		"Отсек №4"
		"        "
		"В отсеке"
		"взрыв,  "
		"радиация";
	size2.Height = 7; size2.Width = 8;
	tf.SetText(str,size2,TRUE, NULL);
	tf.Draw();
	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);
}