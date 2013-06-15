#include "ArmletShell.h"
#include "ColorSchema.h"


#define FORMATS_COUNT 4
//0xADD9EF 
#define HEADER_BACKGROUND 0xD9EF 
#define HEADER_FOREGROUND 0x000000

#define MENU_BACKGROUND 0xFF61
#define MENU_FOREGROUND 0x0000

#define TEXT_FOREGROUND 0xFFFF
#define TEXT_BACKGROUND 0x0000

#define DEFAULT_FONT 1

TextFormat gTextFormats[FORMATS_COUNT];
FormatsRepository gFormatsRepository; 
 
 
fresult InitColorSchema()
{
	fresult fres;
	
	fres = GetTextFormat(DEFAULT_FONT, HEADER_BACKGROUND, HEADER_FOREGROUND,&gTextFormats[FormatHeader]);
	if  (fres!=SUCCESS)
		return fres;
	
	fres = GetTextFormat(DEFAULT_FONT, MENU_BACKGROUND, MENU_FOREGROUND,&gTextFormats[FormatMenu]);
	if  (fres!=SUCCESS)
		return fres;		

	fres = GetTextFormat(DEFAULT_FONT, TEXT_BACKGROUND, TEXT_FOREGROUND,&gTextFormats[FormatText]);
	if  (fres!=SUCCESS)
		return fres;

	fres = GetTextFormat(DEFAULT_FONT, GREEN, RED,&gTextFormats[FormatParrot]);
	if  (fres!=SUCCESS)
		return fres;	

	gFormatsRepository.Init((TextFormat*)gTextFormats, FORMATS_COUNT);
	
	return SUCCESS;
}
