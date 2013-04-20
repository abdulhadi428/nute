#include "xScatteredMenu.h"

#include <iostream>
#include "TextHelper.h"
#include "ToStringRenderer.h"
#include "IMenuItem.h"
#include "TextMenuItem.h"


fresult xScatteredMenu::CreateTextField( char* string, Size size, Position position, TextFormat* format, TextField** opTextField, char** oBuff)
{
	fresult fres = SUCCESS;
	TextField* tf = new TextField();
	*opTextField = tf;

	Size textSize;

	//create buff
	fres = AllocSquareBuffFromString(string, oBuff, &textSize, '\0');
	if (fres != SUCCESS)
	{
		return GENERAL_ERROR;
	}

	tf->Init(size, position, (IRender*)_renderer);
	tf->SetText(*oBuff, textSize, true, format);

	return SUCCESS;
}

fresult xScatteredMenu::CreateTextMenu( Position* positions, char** texts, ubyte_t count, Size itemSize, TextFormat* format, TextFormat* selFormat, ScatteredMenu* oMnu )
{
	IMenuItem** items = new IMenuItem*[count];
	oMnu = new ScatteredMenu();
	fresult fres;

	for (int i =0; i< count; i++)
	{
		TextMenuItem* item = new TextMenuItem();
		items[i] = item;

		TextField* tx;
		char* buff;
		fres = CreateTextField(texts[i], itemSize, positions[i], format, &tx, &buff);
		if (fres!=SUCCESS)
		{
			return fres;
		}

		fres = item->Init(tx,selFormat);
		if (fres!=SUCCESS)
		{
			return fres;
		}
	}

	//init menu
	fres = oMnu->Init(items,count);
	if (fres != SUCCESS)
	{
		return fres;
	}
	return SUCCESS;
}

fresult xScatteredMenu::CreateDefaultMenu(ScatteredMenu* mnu)
{
	fresult fres;
	char* screen=NULL;
	char* formatscreen=NULL;

	Position poss[4];
	poss[0].Top  =0;
	poss[0].Left =0;
	poss[1].Top  =2;
	poss[1].Left =0;
	poss[2].Top  =0;
	poss[2].Left =4;
	poss[3].Top  =2;
	poss[3].Left =4;

	char* items[4];
	items[0] = "a";
	items[1] = "b";
	items[2] = "c";
	items[3] = "d";

	TextFormat format;
	fres = GetTextFormat(DEFAULT_FONT, clWhite, clBlack, &format);
	if (fres != SUCCESS)
	{
		return fres;
	}
	Size size;
	size.Height = 1*format.Font.GlyphSize.Height;
	size.Width = 1*format.Font.GlyphSize.Width;

	TextFormat selformat;
	fres = GetTextFormat(DEFAULT_FONT, clWhite, clBlack, &selformat);
	if (fres != SUCCESS)
	{
		return fres;
	}

	ScatteredMenu oMnu;

	fres = CreateTextMenu(poss, items, 4, size, &format, &selformat, &oMnu);

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


	char* test;
	char* testFormat;

	testFormat = "~$$$$\n$$$$$\n$$$$$";
	test = "a$$$$\n$$$$$\n$$$$$";

	_renderer->Cls();


}




