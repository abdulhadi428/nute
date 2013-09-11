#include "ThreeKShell.h"
#include "TextFieldFactory.h"
#include "kl_allocator.h"

namespace ThreeKShell {

static Alloc_t<TextField, 45> STextFieldFactoryArr;
static Alloc_t<char, 18000> SCharArr;

fresult TextFieldFactory::Init(IRender* renderer, Repositories* styles)
{
	fresult fres;
	_styles = styles;
	_renderer = renderer;
	fres = ResetDefaults();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult TextFieldFactory::ResetDefaults()
{
	DefaultTextFormatHandle = TF_DEFAULT;
	CurrentTextFormatHandle = DefaultTextFormatHandle;

	DefaultWrap = true;
	CurrentWrap = DefaultWrap;

	DefaultFrames = 3;
	CurrentFrames = DefaultFrames;

	DefaultLines = 1;
	CurrentLines = DefaultLines;


	return SUCCESS;
}
fresult TextFieldFactory::GetTextBox(Position pos, ubyte_t maxLineLen , TextField** o_tf)
{
	fresult fres;
	TextFormat* tfmt;
	Size pxSize;

	fres = _styles->TextFormats->GetTextFormat(CurrentTextFormatHandle, &tfmt);
	ENSURESUCCESS(fres);

	pxSize.Height = tfmt->Font.GlyphSize.Height*CurrentLines;
	pxSize.Width = maxLineLen*tfmt->Font.GlyphSize.Width;

	return GetTextBox(pos, NULL, pxSize, TRUE, o_tf);
}


fresult TextFieldFactory::GetTextBox(Position pos, const char* text , TextField** o_tf)
{
	fresult fres;
	//autosize it
	uword_t len = Length(text);
	Size pxSize;

	TextFormat* tfmt;
	fres = _styles->TextFormats->GetTextFormat(CurrentTextFormatHandle, &tfmt);
	ENSURESUCCESS(fres);

	pxSize.Height = tfmt->Font.GlyphSize.Height;
	pxSize.Width = len*tfmt->Font.GlyphSize.Width;

	return GetTextBox(pos, text, pxSize, TRUE, o_tf);
}

fresult TextFieldFactory::GetTextBox(Position pos, Size sizepx , TextField** o_tf)
{
	fresult fres;
	//must be non-static text textbox
	Size buffSz;
	char* buff;

	TextFormat* tfmt;
	fres = _styles->TextFormats->GetTextFormat(CurrentTextFormatHandle, &tfmt);
	ENSURESUCCESS(fres);

	buff = AllocBuffBySizePx(sizepx, tfmt, DefaultFrames, &buffSz);
	FAILIF(buff==NULL);

	fres = GetTextBox(pos, NULL, sizepx, buff, buffSz, o_tf);

	return fres;
}

fresult TextFieldFactory::GetTextBox(Position pos, const char* text, Size sizepx, bool_t noScroll, TextField** o_tf)
{
	fresult fres;

	ubyte_t pages;
	if (noScroll==TRUE)
	{
		pages = 1;
	}
	else
	{
		pages = CurrentFrames;
	}

	TextFormat *tfmt;
	fres = _styles->TextFormats->GetTextFormat(CurrentTextFormatHandle, &tfmt);
	ENSURESUCCESS(fres);

	char* buff;
	Size buffSz;
	buff = AllocBuffBySizePx(sizepx, tfmt, pages, &buffSz);
	FAILIF(buff==NULL);


	fres = GetTextBox(pos,text, sizepx, buff, buffSz, o_tf);
	return fres;
}

fresult TextFieldFactory::GetTextBox(Position pos, const char* text, Size sizepx, char* buff, Size buffSz , TextField** o_tf)
{
	fresult fres;

	TextField* tf = allocTextField();
	FAILIF(tf==NULL);

	fres = tf->Init(sizepx, pos, buff, buffSz, _renderer);
	ENSURESUCCESS(fres);

	TextFormat* tfmt;
	fres = _styles->TextFormats->GetTextFormat(CurrentTextFormatHandle, &tfmt);
	ENSURESUCCESS(fres);

	tf->SetTextFormat(tfmt);

	tf->SetWordWrap(DefaultWrap);

	if (text!=NULL)
	{
		fres = tf->SetText(text);
		ENSURESUCCESS(fres);
	}

	*o_tf = tf;
	return SUCCESS;
}

char* TextFieldFactory::AllocBuffBySizePx( Size sizePx, const TextFormat* tf, ubyte_t pages, Size* o_buffSz)
{
	Size szTx;
	szTx.Height = sizePx.Height / tf->Font.GlyphSize.Height;
	szTx.Width = sizePx.Width / tf->Font.GlyphSize.Width;

	return AllocBuffBySizeTx(szTx, pages, o_buffSz);
}

char* TextFieldFactory::AllocBuffBySizeTx( Size sizeTx, ubyte_t pages, Size* o_buffSz)
{
	Size buffSz;
	buffSz.Height = sizeTx.Height*pages;
	buffSz.Width = sizeTx.Width;

	char* buff = allocBytes(sizeTx.Height*sizeTx.Width*pages);
	if (buff==NULL)
	{
		 o_buffSz->data=0;
	}
	o_buffSz->data = buffSz.data;
	return buff;
}

char* TextFieldFactory::allocBytes( uword_t io_len )
{
	char* res = SCharArr.Allocate(io_len); // new char[io_len];
	StrPad(res, 0, 0, io_len);

	return res;
}

TextField* TextFieldFactory::allocTextField()
{
	return STextFieldFactoryArr.Allocate();     //new TextField();
}
}
