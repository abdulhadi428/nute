#pragma once

namespace ThreeKShell {

class TextBoxFactory
{
private:
	Repositories* _styles;
	IRender* _renderer;

	char* AllocBuffBySizePx(Size sizePx, const TextFormat* tf, ubyte_t pages, Size* o_buffSz);
	char* AllocBuffBySizeTx(Size sizeTx, ubyte_t pages, Size* o_buffSz);
	char* allocBytes(uword_t len);
	TextField* allocTextField();

public:

	TextBoxFactory(Repositories* styles, IRender* renderer);

	TextFormat* DefaultTextFormat;
	TextFormat* CurrentTextFormat;

	bool_t DefaultWrap;
	bool_t CurrentWrap;

	ubyte_t DefaultFrames;
	ubyte_t CurrentFrames;

	fresult ResetDefaults();

	fresult GetTextBox(Position pos, const char* text, TextField** o_tf);
	fresult GetTextBox(Position pos, Size sizepx, TextField** o_tf);
	fresult GetTextBox(Position pos, const char* text, Size sizepx, bool_t staticText, TextField** o_tf);
	fresult GetTextBox(Position pos, const char* text, Size sizepx, char* buff, Size buffSz, TextField** o_tf);
};

}