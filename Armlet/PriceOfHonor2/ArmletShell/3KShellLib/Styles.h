#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

#define INVALID_HANDLE -1

typedef uword_t ColorHandle;

#define COLORS_COUNT 30

#define CL_TRANSPARENT						0
#define CL_BLACK							1
#define	CL_RED								2
#define CL_GREEN							3
#define CL_BLUE								4
#define CL_WHITE							5

#define PRESETCOLORSOFFSET 10

#define CL_CONTROL_BACKGROUND					PRESETCOLORSOFFSET		+1
#define CL_HEADER_BACKGROUND					PRESETCOLORSOFFSET		+2
#define CL_HEADER_TEXT_FOREGROUND				PRESETCOLORSOFFSET		+3
#define CL_DEFAULT_TEXT_FOREGROUND				PRESETCOLORSOFFSET		+4
#define CL_MENU_TEXT_FOREGROUND					PRESETCOLORSOFFSET		+5
#define CL_MENU_BACKGROUND						PRESETCOLORSOFFSET		+6
#define CL_MENU_SELECTED_TEXT_FOREGROUND		PRESETCOLORSOFFSET		+7
#define CL_MENU_SELECTED_BACKGROUND 			PRESETCOLORSOFFSET		+8
#define CL_MENU_DISABLED_TEXT_FOREGROUND		PRESETCOLORSOFFSET		+9
#define CL_MENU_DISABLED_BACKGROUND 			PRESETCOLORSOFFSET		+10
#define CL_MENU_EVEN_TEXT_FOREGROUND			PRESETCOLORSOFFSET		+11
#define CL_MENU_EVEN_BACKGROUND					PRESETCOLORSOFFSET		+12
#define CL_MENU_EVEN_DISABLED_TEXT_FOREGROUND	PRESETCOLORSOFFSET		+13
#define CL_MENU_EVEN_DISABLED_BACKGROUND 		PRESETCOLORSOFFSET		+14

#define CL_SYSTEM_LAST 							PRESETCOLORSOFFSET		+14
		
class ColorsRepository
{
private:
	Color _colors[COLORS_COUNT];

public:
	fresult Init();

	fresult ResetColors();
	fresult RegisterColor(const ColorHandle id, const Color color);
	fresult Register16ARGBColor(const ColorHandle id,const ubyte_t a,const ubyte_t r,const ubyte_t g,const ubyte_t b);
	fresult Register24bitARGBColor(const ColorHandle id,const ubyte_t a,const ubyte_t r,const ubyte_t g, ubyte_t b);
	
	fresult GetColorById(const ColorHandle id, uword_t* color);
		
};

typedef sbyte_t FontHandle;

#define FONTS_COUNT 5

#define FONT_DEFAULT			0
#define FONT_SMALL				1
#define	FONT_NORMAL				2
#define FONT_LARGE				3

class FontsRepository
{
private:
	FontDescription _fonts[FONTS_COUNT];

public:
	fresult Init();

	fresult ResetFonts();
	fresult RegisterFont(const FontHandle id, const ubyte_t systemFontId);

	fresult GetFont(const FontHandle id, FontDescription* font);

};

typedef sbyte_t TextFormatHandle;
#define TEXTFORMATS_COUNT 15

#define TF_DEFAULT			0
#define TF_NORMAL			1
#define	TF_MENU				2
#define TF_MENUDISABLED		3
#define TF_MENUEVEN			4
#define TF_HEADER			5
#define TF_PARROT			6

class TextFormatsRepository
{
private:
	TextFormat _formats[TEXTFORMATS_COUNT];
	ColorsRepository* _colors;
	FontsRepository* _fonts;

public:
	fresult Init(const ColorsRepository* _colors, const FontsRepository* _fonts);

	fresult ResetTextFormats();
	fresult RegisterTextFormat( const TextFormatHandle id, TextFormat* tf);
	fresult GetTextFormat( const TextFormatHandle id, TextFormat** o_textFormat);
	
	fresult CreateTextFormatByStyles(ubyte_t fontId, ubyte_t fgColorId, ubyte_t bgColorId, TextFormat* o_tf);

	fresult GetTextPxSize( TextFormatHandle tfh, const char* text, Size* o_sizePx);
	fresult GetTextPxSize(TextFormat* tf, const char* text, Size* o_sizePx);
};

typedef sword_t ImageHandle;

class ImagesRepository
{
	BitmapImage* _images[MAX_IMAGES];
	uword_t _imagesCount;
	uword_t _usedImages;
public:
	fresult Init();

	fresult RegisterImage(BitmapImage* bitmap);
	BitmapImage* GetImageById(ImageHandle id);
};

class Repositories
{
public:
	ColorsRepository* Colors;
	FontsRepository* Fonts;
	TextFormatsRepository* TextFormats;
	ImagesRepository* Images;

	fresult Init(ColorsRepository* colors, FontsRepository* fonts,	TextFormatsRepository* textFormats, ImagesRepository* images);
};
