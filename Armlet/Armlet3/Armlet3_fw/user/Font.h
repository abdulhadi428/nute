#ifndef FONT__H
#define FONT__H

#include <stdint.h>
#include "sd/integer.h"
#include "sd/ff.h"

class Font
{
public:
    Font(const char* filename);
    ~Font();

    int8_t drawChar(uint8_t x, uint8_t y, uint16_t color, char ch);
    void drawString(uint8_t x, uint8_t y, uint16_t color, const char* s);
    uint8_t getHeight() const;

private:
    int8_t drawCharIntl(FIL* fil, uint8_t x, uint8_t y, uint16_t color, uint16_t pos);
    uint16_t findChar(char ch);
    int8_t findKerning(uint16_t p);

    char* fileName;
    uint16_t count;
    uint8_t height;
    char* symbols;
    DWORD* positions;

#pragma pack(push, 1)
    typedef struct
    {
        uint16_t kernPair;
        int8_t dx;
    } Kerning;
#pragma pack(pop)

    uint16_t kerningSize;
    Kerning* kerning;
};

#endif
