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

fresult GetFontById(ubyte_t fontId, FontDescription* oFont );
fresult GetTextFormat(ubyte_t fontId, Color bgColor, Color fgColor, TextFormat* oFormat);

class FormatsRepository
{
	TextFormat* _formats;
	sbyte_t _formatsCount;
public:
	fresult Init( TextFormat* formats, sbyte_t formatsCount );
	TextFormat* GetFormatById(sbyte_t id);
};

//namespace Fonts {
//	extern const ubyte_t Font_6x8_Data[256][6];
//} //namespace

//using namespace Fonts;