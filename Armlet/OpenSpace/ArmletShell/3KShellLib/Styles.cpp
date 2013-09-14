#include "ThreeKShell.h"

namespace ThreeKShell {

#pragma region Colors

fresult ColorsRepository::Init()
{
	fresult fres;
	fres= ResetColors();
	ENSURESUCCESS(fres);

	return SUCCESS;
}

fresult ColorsRepository::ResetColors()
{
	for (int i=0;i<COLORS_COUNT;i++)
		_colors[i] = WHITE;
	
	_colors[CL_TRANSPARENT]							= _TRANSPARENT;
	_colors[CL_BLACK]								= BLACK;	
	_colors[CL_RED]									= RED;	
	_colors[CL_GREEN]								= GREEN;
	_colors[CL_BLUE]								= BLUE;
	_colors[CL_WHITE]								= WHITE;
	_colors[CL_CONTROL_BACKGROUND]					= WHITE;
	_colors[CL_HEADER_BACKGROUND]					= WHITE;
	_colors[CL_HEADER_TEXT_FOREGROUND]				= BLACK;
	_colors[CL_DEFAULT_TEXT_FOREGROUND]				= BLACK;
	_colors[CL_MENU_TEXT_FOREGROUND]				= BLACK;
	_colors[CL_MENU_BACKGROUND]						= WHITE;
	_colors[CL_MENU_SELECTED_TEXT_FOREGROUND]		= RED;
	_colors[CL_MENU_SELECTED_BACKGROUND]			= BLUE;
	_colors[CL_MENU_DISABLED_TEXT_FOREGROUND]		= RED;
	_colors[CL_MENU_DISABLED_BACKGROUND]			= BLUE;
	_colors[CL_MENU_EVEN_TEXT_FOREGROUND]			= RED;
	_colors[CL_MENU_EVEN_BACKGROUND]				= BLUE;
	_colors[CL_MENU_EVEN_DISABLED_TEXT_FOREGROUND]	= RED;
	_colors[CL_MENU_EVEN_DISABLED_BACKGROUND]		= BLUE;

	return SUCCESS;
}

fresult ColorsRepository::RegisterColor(const ColorHandle id, const uword_t color )
{
	FAILIF (!(id<COLORS_COUNT));
	_colors[id] = color;

	return SUCCESS;
}

fresult ColorsRepository::Register16ARGBColor(const ColorHandle id,const ubyte_t a,const ubyte_t r,const ubyte_t g,const ubyte_t b)
{
	FAILIF(a>15);
	FAILIF(r>15);
	FAILIF(g>15);
	FAILIF(b>15);

	uword_t color16bit = (a<<12)|(r<<8)|(g<<4)|(b) ;
	return RegisterColor(id,color16bit);
}

fresult ColorsRepository::Register24bitARGBColor(const ColorHandle id,const ubyte_t a,const ubyte_t r,const ubyte_t g,const ubyte_t b)
{
	ubyte_t a4 = a>>4 & 0x0F;
	ubyte_t r4 = r>>4 & 0x0F;
	ubyte_t g4 = g>>4 & 0x0F;
	ubyte_t b4 = b>>4 & 0x0F;

	return Register16ARGBColor(id,a4,r4,g4,b4);
}

fresult ColorsRepository::GetColorById(const ColorHandle id, Color* color)
{
	FAILIF (!(id<COLORS_COUNT));
	
	*color = _colors[id];
	return SUCCESS;
}
#pragma endregion

#pragma region //fonts
fresult FontsRepository::Init()
{
	fresult fres;

	fres = ResetFonts();
	ENSURESUCCESS(fres);

	return SUCCESS;

}

fresult FontsRepository::ResetFonts()
{
	fresult fres;
	FontDescription fd;
	fres = CreateFontById(1, &fd);
	ENSURESUCCESS(fres);
	
	for (int i=0;i<FONTS_COUNT;i++)
		_fonts[i] = fd;

	_fonts[FONT_DEFAULT]						= fd;
	_fonts[FONT_SMALL]							= fd;	
	_fonts[FONT_NORMAL]							= fd;	
	_fonts[FONT_LARGE]							= fd;
	
	fres = SUCCESS;
	return fres;	
}

fresult FontsRepository::RegisterFont( const FontHandle id, const ubyte_t systemFontId )
{
	FAILIF (!(id<FONTS_COUNT));
	::CreateFontById(systemFontId, &_fonts[id]);
	return SUCCESS;
}

fresult FontsRepository::GetFont( const FontHandle id, FontDescription* font )
{
	FAILIF (!(id<FONTS_COUNT));		
	*font = _fonts[id];
	
	return SUCCESS;
}
#pragma endregion

#pragma region//formats
fresult TextFormatsRepository::Init(const ColorsRepository* colors, const FontsRepository* fonts)
{
	fresult fres;
	_colors = (ColorsRepository*)colors;
	_fonts = (FontsRepository*)fonts;
	fres = ResetTextFormats();
	ENSURESUCCESS(fres);

	return SUCCESS;

}

fresult TextFormatsRepository::ResetTextFormats()
{
	fresult fres;
	TextFormat deafult_tf;

	fres = CreateTextFormatByStyles(FONT_DEFAULT, CL_BLACK, CL_WHITE, &deafult_tf);
	ENSURESUCCESS(fres);

	for (int i=0;i<TEXTFORMATS_COUNT;i++)
		_formats[i] = deafult_tf;


	_formats[TF_DEFAULT] = deafult_tf;
	_formats[TF_NORMAL] = deafult_tf;
	
	fres = CreateTextFormatByStyles(FONT_NORMAL, CL_MENU_TEXT_FOREGROUND, CL_MENU_BACKGROUND, &_formats[TF_MENU]);
	ENSURESUCCESS(fres);

	fres = CreateTextFormatByStyles(FONT_NORMAL, CL_MENU_DISABLED_TEXT_FOREGROUND, CL_MENU_DISABLED_BACKGROUND, &_formats[TF_MENUDISABLED]);
	ENSURESUCCESS(fres);

	fres = CreateTextFormatByStyles(FONT_NORMAL, CL_MENU_EVEN_TEXT_FOREGROUND, CL_MENU_EVEN_BACKGROUND, &_formats[TF_MENUEVEN]);
	ENSURESUCCESS(fres);

	fres = CreateTextFormatByStyles(FONT_NORMAL, CL_HEADER_TEXT_FOREGROUND, CL_HEADER_BACKGROUND, &_formats[TF_HEADER]);
	ENSURESUCCESS(fres);

	fres = CreateTextFormatByStyles(FONT_NORMAL, CL_RED, CL_BLUE, &_formats[TF_PARROT]);
	ENSURESUCCESS(fres);

	return SUCCESS;	
}

fresult TextFormatsRepository::RegisterTextFormat( const TextFormatHandle id, TextFormat* tf)
{
	FAILIF (!(id<TEXTFORMATS_COUNT));	
	_formats[id] = *tf;

	return SUCCESS;	
}

fresult TextFormatsRepository::GetTextFormat( const TextFormatHandle id, TextFormat** o_textFormat)
{
	
	FAILIF (!(id<TEXTFORMATS_COUNT));
	*o_textFormat = &_formats[id];
	return SUCCESS;	
}

fresult TextFormatsRepository::CreateTextFormatByStyles( ubyte_t fontId, ubyte_t fgColorId, ubyte_t bgColorId, TextFormat* o_tf )
{
	fresult fres;

	Color fg;
	fres = _colors->GetColorById(fgColorId, &fg);
	ENSURESUCCESS(fres);

	Color bg;
	fres = _colors->GetColorById(bgColorId, &bg);
	ENSURESUCCESS(fres);

	FontDescription fd;
	fres = _fonts->GetFont(FONT_DEFAULT, &fd);
	ENSURESUCCESS(fres);

	fres = ::CreateTextFormat(fd.ID, bg, fg, o_tf);

	return fres;
}
#pragma endregion//formats

#pragma region //images
BitmapImage* ImagesRepository::GetImageById(ImageHandle id)
{
	NULLIF(id<0 || !(id < _usedImages));
	return _images[id];
}

fresult ImagesRepository::Init()
{
	_usedImages  =0;

	FillStdImages(this);

	return SUCCESS;
}	

fresult ImagesRepository::RegisterImage( BitmapImage* bitmap )
{
	FAILIF(!(_usedImages +1<MAX_IMAGES));
	_images[_usedImages++] = bitmap;
	return SUCCESS;
}
#pragma endregion Images

#pragma region //repositories
fresult Repositories::Init( ColorsRepository* colors, FontsRepository* fonts, TextFormatsRepository* textFormats, ImagesRepository* images)
{
	Colors = colors;
	Fonts = fonts;
	TextFormats = textFormats;
	Images = images;

	return SUCCESS;
}

fresult TextFormatsRepository::GetTextPxSize( TextFormatHandle tfh, const char* text, Size* o_sizePx)
{
	fresult fres;
	TextFormat* tf = NULL;
	fres = GetTextFormat(tfh, &tf);
	ENSURESUCCESS(fres);

	return GetTextPxSize(tf, text,  o_sizePx);
}

fresult TextFormatsRepository::GetTextPxSize(TextFormat* tf, const char* text, Size* o_sizePx)
{
	ubyte_t maxLineLen =0;
	sword_t currnentLineStart =0;
	sword_t prevLineStart =0;
	sword_t textLen =Length(text);
	ubyte_t textLines =1;
	Size szResult;
	szResult.data = 0;

	do 
	{
		prevLineStart = currnentLineStart;
		currnentLineStart = InStr(text, "\n", prevLineStart);

		if (currnentLineStart != -1)
		{
			textLines++;
			if (maxLineLen < currnentLineStart-prevLineStart)
			{
				maxLineLen = currnentLineStart-prevLineStart;
			}
		}
		else
		{
			if (maxLineLen < textLen-prevLineStart)
			{
				maxLineLen = textLen-prevLineStart;
			}
			break;
		}
	} while (TRUE);

	szResult.Width = maxLineLen*tf->Font.GlyphSize.Width;
	szResult.Height = textLines*tf->Font.GlyphSize.Height;

	*o_sizePx = szResult;
	return SUCCESS;
}

#pragma endregion

}



