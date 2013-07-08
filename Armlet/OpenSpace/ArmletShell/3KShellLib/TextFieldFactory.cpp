#include "ThreeKShell.h"

namespace ThreeKShell {

TextFieldFactory::TextFieldFactory(Repositories* styles, IRender* renderer)
{
	_styles = styles;
	_renderer = renderer;
	ResetDefaults();
}
 
fresult TextFieldFactory::ResetDefaults()
{
	fresult fres;
	fres = _styles->TextFormats->GetTextFormat(TF_DEFAULT, DefaultTextFormat);
	ENSURESUCCESS(fres);

	CurrentTextFormat = DefaultTextFormat;

	DefaultWrap = true;
	CurrentWrap = DefaultWrap;

	DefaultFrames = 3;
	CurrentFrames = DefaultFrames;

	return SUCCESS;
}


fresult TextFieldFactory::GetTextBox(Position pos, const char* text , TextField** o_tf)
{
	//autosize it
	uword_t len = Length(text);
	Size pxSize;
	pxSize.Height = CurrentTextFormat->Font.GlyphSize.Height;
	pxSize.Width = len*CurrentTextFormat->Font.GlyphSize.Width;

	return GetTextBox(pos, text, pxSize, TRUE, o_tf);
}

fresult TextFieldFactory::GetTextBox(Position pos, Size sizepx , TextField** o_tf)
{
	fresult fres;
	//must be non-static text textbox
	Size buffSz;
	char* buff;
	buff = AllocBuffBySizePx(sizepx, CurrentTextFormat, DefaultFrames, &buffSz);
	FAILIF(buff==NULL);
	
	fres = GetTextBox(pos, NULL, sizepx, buff, buffSz, o_tf);

	return fres;
}

fresult TextFieldFactory::GetTextBox(Position pos, const char* text, Size sizepx, bool_t staticText, TextField** o_tf)
{
	fresult fres;

	ubyte_t pages;
	if (staticText==TRUE)
	{
		pages = 1;
	}
	else
	{
		pages = CurrentFrames;
	}
	char* buff;
	Size buffSz;
	buff = AllocBuffBySizePx(sizepx, CurrentTextFormat, pages, &buffSz);
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

	tf->SetTextFormat(CurrentTextFormat);

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
	return new char[io_len];
}

TextField* TextFieldFactory::allocTextField()
{
	return new TextField();
}
}
