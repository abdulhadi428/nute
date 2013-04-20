#include "xTestTextField.h"
#include "TextHelper.h"
#include <iostream>


fresult xTestTextField::CreateTextField( char* string, Size size, Position position, TextFormat* format, TextField** opTextField, char** oBuff)
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


fresult xTestTextField::Init()
{
	fresult	fres=SUCCESS;
	Size size;
	size.Height=3;
	size.Width=5;
	
	_renderer = new ToStringRenderer();
	fres = _renderer->Init(size);

	return fres;
}


void xTestTextField::doTests()
{
	char *testName;
	try
	{
		testName = "xDrawAndScroll";
		xDrawAndScroll();

		testName = "xPlayWithCharSize";
		xPlayWithCharSize();

		testName = "xDrawAreas";
		xDrawAreas();

		testName = "xColorTest";
		xColorTest();
		
		std::cout << "xTestTextField tests passed.\n";
	}
	catch (char* err)
	{
		std::cout <<"xTestTextField FAILED: " << testName <<": "<< err;
	}
}

void xTestTextField::xInit()
{
	int a=9;
}
void xTestTextField::xDrawAndScroll()
{
	TextFormat tx;
	tx.BgColor = 0;
	tx.FgColor =0;
	Font fnt;
	fnt.GlyphSize.Height =8;
	fnt.GlyphSize.Width =8;
	fnt.ID =1;
	fnt.Name = "Kreyls" ;
	tx.Font = fnt;
	
	xDrawAndScroll(&tx);

}

void xTestTextField::xDrawAndScroll(TextFormat* format)

{
	fresult fres;
	char* screen=NULL;
	TextField* tf=NULL;
	char* oBuff=NULL;
	Size size;
	Position pos;
	char* content;
	char* test;
	
	
	//////////////////////////////////////////////////////////////////////////
	tf=NULL;
	oBuff=NULL;
	size.Height =3*format->Font.GlyphSize.Height;
	size.Width =5*format->Font.GlyphSize.Width;
	pos.Left =0;
	pos.Top =0;
	content = "aaaaa\nbbbbb\nccccc";
	test =    "aaaaa\nbbbbb\nccccc";

	fres = CreateTextField(content, size, pos, format, &tf, &oBuff);
	if (fres!=SUCCESS)
		throw "Can't create TextField 3x5 aaaaa.";
	if (tf==NULL)
		throw "Can't create TextField 3x5 aaaaa. TextField is NULL";
	if (oBuff==NULL)
		throw "Can't create TextField 3x5 aaaaa. oBuff is NULL";
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing TextField 3x5 aaaaa.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing TextField 3x5 aaaaa. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing TextField 3x5 aaaaa. Wrong screen content.";
	delete screen;
	delete oBuff;
	delete tf;

	//////////////////////////////////////////////////////////////////////////
	tf=NULL;
	oBuff=NULL;
	size.Height =3*format->Font.GlyphSize.Height;
	size.Width =5*format->Font.GlyphSize.Width;
	pos.Left =0;
	pos.Top =0;
	content = "aaaaaeee\nbbbbb\nccee";
	test = "aaaaa\nbbbbb\nccee$";

	_renderer->Cls();
	fres = CreateTextField(content, size, pos, format, &tf, &oBuff);
	if (fres!=SUCCESS)
		throw "Can't create 3x5 TextField with 3x(8,5,4) text.";
	if (tf==NULL)
		throw "Can't create 3x5 TextField with 3x(8,5,4) text. TextField is NULL";
	if (oBuff==NULL)
		throw "Can't create 3x5 TextField with 3x(8,5,4) text. oBuff is NULL";
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text. Wrong screen content.";
	delete screen;

	//scroll 1 down
	///////////////////////
	_renderer->Cls();
	pos.Top = 1;
	pos.Left = 0;
	//"aaaaaeee\nbbbbb\nccee"
	test = "bbbbb\n"
		   "ccee$\n"
		   "$$$$$";
	tf->SetScrollPosition(pos);
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 1,0.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 1,0. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 1,0. Wrong screen content.";
	delete screen;

	//scroll  1 left
	///////////////////////
	_renderer->Cls();
	pos.Left = 1;
	pos.Top =0;
	//"aaaaaeee\nbbbbb\nccee"
	test = "aaaae\n"
		   "bbbb$\n"
		   "cee$$";
	tf->SetScrollPosition(pos);
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,1.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,1. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,1. Wrong screen content.";
	delete screen;

	//scroll  8 left
	///////////////////////
	_renderer->Cls();
	pos.Left = 8;
	pos.Top =0;
	//"aaaaaeee\nbbbbb\nccee"
	test =	"e$$$$\n"
			"$$$$$\n"
			"$$$$$";
	fres=tf->SetScrollPosition(pos);
	if (fres!=GENERAL_WARNING)
		throw "Error scrolling 3x5 TextField with 3x(8-7) text to 8,0. Warning was required.";
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 8,0.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 8,0. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 8,0. Wrong screen content.";
	delete screen;

	//scroll  out off height
	///////////////////////
	_renderer->Cls();
	pos.Left = 0;
	pos.Top =3;
	//"aaaaaeee\nbbbbb\nccee"
	test =	"ccee$\n"
		"$$$$$\n"
		"$$$$$";
	tf->SetScrollPosition(pos);
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,3.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,3. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,3. Wrong screen content.";
	delete screen;

	//scroll  out off height
	///////////////////////
	_renderer->Cls();
	pos.Left = 0;
	pos.Top =8;
	//"aaaaaeee\nbbbbb\nccee"
	test =	"ccee$\n"
			"$$$$$\n"
			"$$$$$";
	tf->SetScrollPosition(pos);
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,8.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,8. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8-7) text scrolled to 0,8. Wrong screen content.";

	delete screen;
	delete oBuff;
	delete tf;
}


void xTestTextField::xPlayWithCharSize()
{
	fresult fres;
	char* screen=NULL;
	TextField* tf=NULL;
	char* oBuff=NULL;
	Size size;
	Position pos;
	char* content;
	char* test;

	TextFormat tx;
	tx.BgColor = 0;
	tx.FgColor =0;
	Font fnt;
	fnt.GlyphSize.Height = 3;
	fnt.GlyphSize.Width = 6;
	fnt.ID =1;
	fnt.Name = "Kreyls" ;
	tx.Font = fnt;

	//////////////////////////////////////////////////////////////////////////
	tf=NULL;
	oBuff=NULL;
	size.Height =3*fnt.GlyphSize.Height;
	size.Width =5*fnt.GlyphSize.Width;
	pos.Left =0;
	pos.Top =0;
	content = "aaaaa\nbbbbb\nccccc";
	test =    "aaaaa\nbbbbb\nccccc";

	fres = CreateTextField(content, size, pos, &tx, &tf, &oBuff);
	if (fres!=SUCCESS)
		throw "Can't create TextField 3x5 aaaaa.";
	if (tf==NULL)
		throw "Can't create TextField 3x5 aaaaa. TextField is NULL";
	if (oBuff==NULL)
		throw "Can't create TextField 3x5 aaaaa. oBuff is NULL";
	fres = tf->Draw();
	if (fres!=SUCCESS)
		throw "Error drawing TextField 3x5 aaaaa.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing TextField 3x5 aaaaa. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing TextField 3x5 aaaaa. Wrong screen content.";
	delete screen;
	delete oBuff;
	delete tf;

	xDrawAndScroll(&tx);
}

void xTestTextField::xDrawAreas()
{
	
	fresult fres;
	char* screen=NULL;
	TextField* tf=NULL;
	char* oBuff=NULL;
	Size size;
	Position pos;
	char* content;
	char* test;
	
	TextFormat format;
	format.BgColor = 0;
	format.FgColor =0;
	Font fnt;
	fnt.GlyphSize.Height =8;
	fnt.GlyphSize.Width =8;
	fnt.ID =1;
	fnt.Name = "Kreyls" ;
	format.Font = fnt;
	Position drawPos;
	Size drawSize;
	
	//////////////////////////////////////////////////////////////////////////
	size.Height =3*fnt.GlyphSize.Height;
	size.Width =5*fnt.GlyphSize.Width;
	pos.Left =0;
	pos.Top =0;
	content = "aaaaaeee\nbbbbb\nccee";

	_renderer->Cls();
	fres = CreateTextField(content, size, pos, &format, &tf, &oBuff);
	if (fres!=SUCCESS)
		throw "Can't create 3x5 TextField with 3x(8,5,4) text.";
	if (tf==NULL)
		throw "Can't create 3x5 TextField with 3x(8,5,4) text. TextField is NULL";
	if (oBuff==NULL)
		throw "Can't create 3x5 TextField with 3x(8,5,4) text. oBuff is NULL";
	
	//////////////////////////////////////////////////////////////////////////
	_renderer->Cls();
	//content = "aaaaaeee\nbbbbb\nccee";
	test =        "aaaaa\nbbbbb\nccee$";
	drawPos.Left =0;
	drawPos.Top =0;
	drawSize.Height =3*fnt.GlyphSize.Height;
	drawSize.Width =5*fnt.GlyphSize.Width;
	
	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in full area.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in full area. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in full area. Wrong screen content.";
	delete screen;

	//////////////////////////////////////////////////////////////////////////
	_renderer->Cls();
	//content = "aaaaaeee\nbbbbb\nccee";
	test =      "a$$$$\n$$$$$\n$$$$$";
	drawPos.Left =0;
	drawPos.Top =0;
	drawSize.Height =1*fnt.GlyphSize.Height;
	drawSize.Width =1*fnt.GlyphSize.Width;

	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text. Wrong screen content.";
	delete screen;

	//////////////////////////////////////////////////////////////////////////
	//outside buff
	_renderer->Cls();
	//content = "aaaaaeee\nbbbbb\nccee";
	test =        "aaaaa\n$$$$$\n$$$$$";
	drawPos.Left =0;
	drawPos.Top =0;
	drawSize.Width =6*fnt.GlyphSize.Width;
	drawSize.Height =1*fnt.GlyphSize.Height;

	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 6x1 area.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 6x1 area. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 6x1 area. Wrong screen content.";
	delete screen;

	//outside buff
	_renderer->Cls();
	//content = "aaaaaeee\nbbbbb\nccee";
	test =        "aaaaa\nbbbbb\nccee$";
	drawPos.Left =0;
	drawPos.Top =0;
	drawSize.Width =9*fnt.GlyphSize.Width;
	drawSize.Height =4*fnt.GlyphSize.Height;

	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 9x4 area.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 9x4 area. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 9x4 area. Wrong screen content.";
	delete screen;

	//Shorter width
	_renderer->Cls();
	//content = "aaaaaeee\nbbbbb\nccee";
	test =        "aaaa$\nbbbb$\nccee$";
	drawPos.Left =0;
	drawPos.Top =0;
	drawSize.Width =4*fnt.GlyphSize.Width;
	drawSize.Height =4*fnt.GlyphSize.Height;

	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 4x4 area.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 4x4 area. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 4x4 area. Wrong screen content.";
	delete screen;

	//////////////////////////////////////////////////////////////////////////
	//at the middle
	_renderer->Cls();
	//content = "aaaaaeee\nbbbbb\nccee";
	test =        "$$$$$\n$bbb$\n$cee$";
	drawPos.Left =1*fnt.GlyphSize.Width;
	drawPos.Top =1*fnt.GlyphSize.Height;
	drawSize.Width =3*fnt.GlyphSize.Width;
	drawSize.Height =2*fnt.GlyphSize.Height;

	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 2x3 area at 1,1.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 2x3 area at 1,1. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 2x3 area at 1,1. Wrong screen content.";
	delete screen;


	//////////////////////////////////////////////////////////////////////////
	//exceed scroll pos
	_renderer->Cls();
	drawPos.Left =2*fnt.GlyphSize.Width;
	drawPos.Top =1*fnt.GlyphSize.Height;
	drawSize.Width =4*fnt.GlyphSize.Width;
	drawSize.Height =2*fnt.GlyphSize.Height;

	Position scrollPos;
	scrollPos.Top =1;
	scrollPos.Left = 1;
	tf->SetScrollPosition(scrollPos);

	//content = "aaaaaeee\nbbbbb\nccee";
	//scrolled = bbbb$\ncee$$\n$$$$$
	test =        "$$$$$\n$$e$$\n$$$$$";

	fres = tf->DrawArea(drawPos, drawSize);

	if (fres!=SUCCESS)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 4x2 area at 2,1 at scroll pos 1,1.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 4x2 area at 2,1 at scroll pos 1,1. Can't get screen.";
	if (!StringEquals(screen, test))
		throw "Error drawing 3x5 TextField with 3x(8,5,4) text in 4x2 area at 2,1 at scroll pos 1,1. Wrong screen content.";
	delete screen;
}

void xTestTextField::xColorTest()
{

	fresult fres;
	char* screen=NULL;
	char* formatscreen=NULL;
	TextField* tf=NULL;
	char* oBuff=NULL;
	Size size;
	Position pos;
	char* content;
	char* test;
	char* testFormat;

	TextFormat format;
	TextFormat selformat;
	format.BgColor = clBlack;
	format.FgColor =clWhite;
	selformat.BgColor = clGrey;
	selformat.FgColor =clRed;

	Font fnt;
	fnt.GlyphSize.Height =8;
	fnt.GlyphSize.Width =8;
	fnt.ID =1;
	fnt.Name = "Kreyls" ;
	format.Font = fnt;
	selformat.Font = fnt;

	//////////////////////////////////////////////////////////////////////////
	size.Height =1*fnt.GlyphSize.Height;
	size.Width =1*fnt.GlyphSize.Width;
	pos.Left =0;
	pos.Top =0;
	content = "a";
	testFormat = "~$$$$\n$$$$$\n$$$$$";
	test = "a$$$$\n$$$$$\n$$$$$";

	_renderer->Cls();
	fres = CreateTextField(content, size, pos, &format, &tf, &oBuff);
	if (fres!=SUCCESS)
		throw "Can't create 1x1 TextField with 'a' text.";
	if (tf==NULL)
		throw "Can't create 1x1 TextField with 'a' text. TextField is NULL";
	if (oBuff==NULL)
		throw "Can't create 1x1 TextField with 'a' text. oBuff is NULL";

	fres = tf->Draw();

	if (fres!=SUCCESS)
		throw "Error drawing 1x1 TextField with 'a' text.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 1x1 TextField with 'a' text. Can't get screen.";
	formatscreen = _renderer->GetFormatScreen();
	if (formatscreen == NULL)
		throw "Error drawing 1x1 TextField with 'a' text. Can't get format screen.";

	if (!StringEquals(screen, test))
		throw "Error drawing 1x1 TextField with 'a' text. Wrong screen content.";
	if (!StringEquals(formatscreen, testFormat))
		throw "Error drawing 1x1 TextField with 'a' text. Wrong screen format content.";	

	delete screen;
	delete formatscreen;

	/////////////////////////////////////////////
	_renderer->Cls();
	pos.Left =4*fnt.GlyphSize.Height;
	pos.Top =2*fnt.GlyphSize.Width;
	tf->SetTextFormat(&selformat);
	tf->SetPosition(pos);
	testFormat = "$$$$$\n$$$$$\n$$$$!";
	test = "$$$$$\n$$$$$\n$$$$a";

	fres = tf->Draw();

	if (fres!=SUCCESS)
		throw "Error drawing 1x1 TextField with 'a' text in sel color.";
	screen = _renderer->GetScreen();
	if (screen == NULL)
		throw "Error drawing 1x1 TextField with 'a' text  in sel color. Can't get screen.";
	formatscreen = _renderer->GetFormatScreen();
	if (formatscreen == NULL)
		throw "Error drawing 1x1 TextField with 'a' text  in sel color. Can't get format screen.";

	if (!StringEquals(screen, test))
		throw "Error drawing 1x1 TextField with 'a' text  in sel color. Wrong screen content.";
	if (!StringEquals(formatscreen, testFormat))
		throw "Error drawing 1x1 TextField with 'a' text  in sel color. Wrong screen format content.";	

	
}







