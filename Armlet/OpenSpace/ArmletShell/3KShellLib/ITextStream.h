#pragma once

class ITextStream
{
public:	
	virtual char* GetTextAt (const uword_t pos, const uword_t size, uword_t* o_gotSize){return NULL;};
	virtual uword_t GetLength(){return 0;};
};
