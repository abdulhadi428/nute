#ifndef _NAMESPACE_PROTECT_
	#error "Use ArmletShell.h"
#endif

//Font
typedef struct _FontDescription
{
	//Logical Name
	const char* Name;
	//ID
	ubyte_t ID;
	//Size of the glyph in pixel (only MonoType fonts are supported)
	Size GlyphSize;
} FontDescription;

//TextFormat
typedef struct _TextFormat
{
	Color FgColor;
	Color BgColor;
	FontDescription Font;
} TextFormat;

fresult CreateFontById(ubyte_t fontId, FontDescription* oFont );
fresult CreateTextFormat(ubyte_t fontId, Color bgColor, Color fgColor, TextFormat* oFormat);

//TODO: remove
class FormatsRepository_DEPRECATED
{
	TextFormat* _formats;
	sbyte_t _formatsCount;
public:
	fresult Init( TextFormat* formats, sbyte_t formatsCount );
	TextFormat* GetFormatById(sbyte_t id);
};

