#include "xScatteredMenu.h"

#include <iostream>
#include "TextHelper.h"
#include "ToStringRenderer.h"
#include "IMenuItem.h"
#include "MenuItem.h"
#include "ArmletShell.h"
#include "defs.h"
using namespace ThreeKShell;


fresult xScatteredMenu::CreateTextField( char* str, Size size, Position position, TextFormat* format, TextField** opTextField, Size buffSize)
{
	fresult fres = SUCCESS;
	TextField* tf = new TextField();
	*opTextField = tf;

	char* buff = new char[buffSize.Height*buffSize.Width];

	tf->Init(size, position, buff, buffSize, (IRender*)_renderer);
	tf->SetWordWrap(FALSE);
	tf->SetTextFormat(format);
	fres = tf->SetText(str);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	return SUCCESS;
}

fresult xScatteredMenu::CreateTextMenu( Position* positions, char** texts, ubyte_t count, Size itemSize, TextFormat* format, TextFormat* selFormat, ScatteredMenu** oMnu )
{
	IMenuItem** items = new IMenuItem*[count];
	(*oMnu) = new ScatteredMenu();
	fresult fres;

	for (ubyte_t i =0; i< count; i++)
	{
		MenuItem* item = new MenuItem();
		items[i] = item;

		TextField* tx;
		
		sword_t len = Length(texts[i]);
		Size buffSizeTx;
		buffSizeTx.Height =1;
		buffSizeTx.Width = (ubyte_t)len;

		fres = CreateTextField(texts[i], itemSize, positions[i], format, &tx, buffSizeTx);
		if (fres!=SUCCESS)
		{
			return fres;
		}

		fres = item->Init(tx,selFormat, NULL, tx, NULL, 0);
		if (fres!=SUCCESS)
		{
			return fres;
		}
	}

	//init menu
	fres = (*oMnu)->Init(items,count);
	if (fres != SUCCESS)
	{
		return fres;
	}
	return SUCCESS;
}

fresult xScatteredMenu::CreateDefaultMenu(ScatteredMenu** mnu)
{
	fresult fres;

	TextFormat format;
	fres = CreateTextFormat(1, clBlack, clWhite, &format);
	if (fres != SUCCESS)
	{
		return fres;
	}
	TextFormat selformat;
	fres = CreateTextFormat(1, clGrey, clRed, &selformat);
	if (fres != SUCCESS)
	{
		return fres;
	}
	ubyte_t fontWidth = format.Font.GlyphSize.Width;
	ubyte_t fontHeight = format.Font.GlyphSize.Height;

	Position poss[4];
	poss[0].Top  =0*fontHeight;
	poss[0].Left =0*fontWidth;
	poss[1].Top  =2*fontHeight;
	poss[1].Left =0*fontWidth;
	poss[2].Top  =0*fontHeight;
	poss[2].Left =4*fontWidth;
	poss[3].Top  =2*fontHeight;
	poss[3].Left =4*fontWidth;

	char* items[4];
	items[0] = "a";
	items[1] = "b";
	items[2] = "c";
	items[3] = "d";

	Size size;
	size.Height = 1*fontHeight;
	size.Width = 1*fontWidth;

	fres = CreateTextMenu(poss, items, 4, size, &format, &selformat, mnu);

	return fres;
}

void xScatteredMenu::doTests()
{
	char *testName;
	try
	{
		testName = "xLoopThroughtItems";
		xLoopThroughtItems();

		std::cout << "xScatteredMenu tests passed.\n";
	}
	catch (char* err)
	{
		std::cout <<"xScatteredMenu FAILED: "<< testName <<": "<< err;
	}
}

fresult xScatteredMenu::Init()
{
	fresult	fres=SUCCESS;
	Size size;
	size.Height=3;
	size.Width=5;

	_renderer = new ToStringRenderer();
	fres = _renderer->Init(size);

	return fres;

}

void xScatteredMenu::xLoopThroughtItems()
{
	fresult fres;
	ScatteredMenu* mnu =NULL;
	char* screen = NULL;
	char* formatScreen = NULL;
	char* formatScreenTest = NULL;
	char* test = NULL;

	fres = CreateDefaultMenu(&mnu);
	if (fres != SUCCESS)
		throw "Can't create basic menu";
	
	_renderer->Cls();

	test = "a$$$c\n$$$$$\nb$$$d";
	fres = mnu->Draw();
	if (fres != SUCCESS)
		throw "Can't draw basic menu";
	
	screen = _renderer->GetScreen();
	if (!StringEquals(screen, test))
		throw  "Can't draw basic menu. Screen differs.";
	
	formatScreenTest = "~$$$~\n$$$$$\n~$$$~";
	formatScreen = _renderer->GetFormatScreen();
	if (!StringEquals(formatScreen, formatScreenTest))
		throw  "Can't draw basic menu. Format screen differs.";


	mnu->SelectItem(1);
	formatScreenTest = "~$$$~\n$$$$$\n!$$$~";
	formatScreen = _renderer->GetFormatScreen();
	if (!StringEquals(formatScreen, formatScreenTest))
		throw  "Can't select item 2 in menu. Format screen differs.";

}




