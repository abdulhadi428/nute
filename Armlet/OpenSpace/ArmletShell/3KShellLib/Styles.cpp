#include "ThreeKShell.h"

namespace ThreeKShell {

#pragma region Colors
ColorsRepository::ColorsRepository()
{
	ResetColors();
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
	_colors[CL_HEADER_BACKGROUND]					= 0xD9EF;
	_colors[CL_HEADER_TEXT_FOREGROUND]				= 0x0000;
	_colors[CL_DEFAULT_TEXT_FOREGROUND]				= 0x0000;
	_colors[CL_MENU_TEXT_FOREGROUND]				= 0x0000;
	_colors[CL_MENU_BACKGROUND]						= 0xFF61;
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

fresult ColorsRepository::RegisterColor(const ubyte_t id, const uword_t color )
{
	FAILIF (!(id<COLORS_COUNT));
	_colors[id] = color;

	return SUCCESS;
}

fresult ColorsRepository::Register16ARGBColor(const ubyte_t id,const ubyte_t a,const ubyte_t r,const ubyte_t g,const ubyte_t b)
{
	FAILIF(a>15);
	FAILIF(r>15);
	FAILIF(g>15);
	FAILIF(b>15);

	uword_t color16bit = (a<<12)|(r<<8)|(g<<4)|(b) ;
	return RegisterColor(id,color16bit);
}

fresult ColorsRepository::Register24bitARGBColor(const ubyte_t id,const ubyte_t a,const ubyte_t r,const ubyte_t g,const ubyte_t b)
{
	ubyte_t a4 = a>>4;
	ubyte_t r4 = r>>4;
	ubyte_t g4 = g>>4;
	ubyte_t b4 = b>>4;

	return Register16ARGBColor(id,a4,b4,g4,b4);
}

fresult ColorsRepository::GetColorById(const ubyte_t id, Color* color)
{
	FAILIF (!(id<COLORS_COUNT));
	
	*color = _colors[id];
	return SUCCESS;
}
#pragma endregion

#pragma region //fonts
FontsRepository::FontsRepository()
{
	ResetFonts();
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

fresult FontsRepository::RegisterFont( const ubyte_t id, const ubyte_t systemFontId )
{
	FAILIF (!(id<FONTS_COUNT));
	::CreateFontById(systemFontId, &_fonts[id]);
	return SUCCESS;
}

fresult FontsRepository::GetFont( const ubyte_t id, FontDescription* font )
{
	FAILIF (!(id<FONTS_COUNT));		
	*font = _fonts[id];
	
	return SUCCESS;
}
#pragma endregion


#pragma region//formats
TextFormatsRepository::TextFormatsRepository(const ColorsRepository* colors, const FontsRepository* fonts)
{
	_colors = (ColorsRepository*)colors;
	_fonts = (FontsRepository*)fonts;
	ResetTextFormats();
}

fresult TextFormatsRepository::ResetTextFormats()
{
	fresult fres;
	TextFormat deafult_tf;

	fres = CreateTextFormatByStyles(FONT_DEFAULT, CL_WHITE, CL_BLACK, &deafult_tf);
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

fresult TextFormatsRepository::RegisterTextFormat( const ubyte_t id, TextFormat* tf)
{
	FAILIF (!(id<TEXTFORMATS_COUNT));	
	_formats[id] = *tf;

	return SUCCESS;	
}

fresult TextFormatsRepository::GetTextFormat( const ubyte_t id, TextFormat* o_textFormat)
{
	
	FAILIF (!(id<TEXTFORMATS_COUNT));
	o_textFormat = &_formats[id];
	return SUCCESS;	
}

fresult TextFormatsRepository::CreateTextFormatByStyles( ubyte_t fontId, ubyte_t fgColorId, ubyte_t bgColorId, TextFormat* o_tf )
{
	fresult fres;

	Color fg;
	fres = _colors->GetColorById(CL_BLACK, &fg);
	ENSURESUCCESS(fres);

	Color bg;
	fres = _colors->GetColorById(CL_BLACK, &bg);
	ENSURESUCCESS(fres);

	FontDescription fd;
	fres = _fonts->GetFont(FONT_DEFAULT, &fd);
	ENSURESUCCESS(fres);

	fres = ::CreateTextFormat(fd.ID, bg, fg, o_tf);

	return fres;
}
#pragma endregion//formats

#pragma region //repositories
Repositories::Repositories( ColorsRepository* colors, FontsRepository* fonts, TextFormatsRepository* textFormats, ImagesRepository* images)
{
	Colors = colors;
	Fonts = fonts;
	TextFormats = textFormats;
	Images = images;
}

#pragma endregion

}

