#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class TextFieldFactory
{
private:
	Repositories* _styles;
	IRender* _renderer;

	char* AllocBuffBySizePx(Size sizePx, const TextFormat* tf, ubyte_t pages, Size* o_buffSz);
	char* AllocBuffBySizeTx(Size sizeTx, ubyte_t pages, Size* o_buffSz);
	char* allocBytes(uword_t len);
	TextField* allocTextField();

public:

	fresult Init(IRender* renderer, Repositories* styles);

	TextFormatHandle DefaultTextFormatHandle;
	TextFormatHandle CurrentTextFormatHandle;

	bool_t DefaultWrap;
	bool_t CurrentWrap;

	ubyte_t DefaultFrames;
	ubyte_t CurrentFrames;

	ubyte_t DefaultLines;
	ubyte_t CurrentLines;

	fresult ResetDefaults();

	fresult GetTextBox(Position pos, const char* text, TextField** o_tf);
	fresult GetTextBox(Position pos, Size sizepx, TextField** o_tf);
	fresult GetTextBox(Position pos, ubyte_t maxLineLen , TextField** o_tf);
	fresult GetTextBox(Position pos, const char* text, Size sizepx, bool_t noScroll, TextField** o_tf);
	fresult GetTextBox(Position pos, const char* text, Size sizepx, char* buff, Size buffSz, TextField** o_tf);
};
