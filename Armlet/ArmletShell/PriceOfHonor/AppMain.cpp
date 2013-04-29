#include "ArmletShell.h"
#include "UserInterface.h"
#include "MenuDelegate.h"
#include "MiddleInterface.h"
#include "Images.h"
//#include <string.h>
//#include "UpInterface.h"
//#include "AppMain.h"
//#include "TextField.h"
//#include "GraphicRenderer.h"
//#include "Med.h"

/*TextField tf;
GraphicRenderer rend;
*/

UserInterface UI;


void __CALLBACK AppOnButtonClick(ubyte_t button)
{
/*	if (button == BUTTON_Y) {
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
*/
	UI._SystemOnButtonClick(button);
	return;
}

void __CALLBACK AppOnButtonHold(ubyte_t button)
{
	UI._SystemOnButtonClick(BUTTON_HOLD_OFFSET+button);
	return;
}

uword_t gx=0;
uword_t gy=0;
Color gc=0x0000;
bool __CALLBACK MoveBoxByTimer()
{
	DrawRect(gx,gy,30,30,0);
	gc+=0x111;
	if (gc>=0x1000) gc=0x0000;
	gx+=15; gy+=5;
	if (gx>SCREENX) gx-=SCREENX;
	if (gy>SCREENY) gy-=SCREENY;
	DrawRect(gx,gy,30,30,gc);
	return false;
}

void __CALLBACK AppMainThread(void* param)
{
	/*const char* str = "PRICE of HONOR!";
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
	
	RequestTimer(300, MoveBoxByTimer);

	if (
		!price_of_honor::CheckCures() ||
		!price_of_honor::CheckWounds()
		)
	{
		tf.SetText("CURE-WOUNDS");
		tf.Draw();
	}
	*/
	RegisterButtonHandlers(AppOnButtonClick, AppOnButtonHold);

	//extern const unsigned short ArrowDownOrangeBitmap[];
	//DrawBitmap_kel(0,0,24,24,(short*)ArrowDownOrangeBitmap);

	fresult fres;
	InitMenuHandlerDelegates(&UI);
	InitImages();
	fres = UI.Init();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to init UI";
		DrawTextString(10,10,err,Length(err),0,0);
	}
	else
	{
		fres = UI.Draw();
		if (fres!=SUCCESS)
		{
			char* err = "Failed to draw UI";
			DrawTextString(10,10,err,Length(err),0,0);
		}
	}
	
}