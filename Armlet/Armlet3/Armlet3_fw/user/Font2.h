#ifndef FONT2__H
#define FONT2__H

#include <stdint.h>
#include "sd/integer.h"
#include "sd/ff.h"

class Font2
{
public:
    Font2(const char* filename);
    ~Font2();

    int8_t drawChar(uint8_t x, uint8_t y, uint16_t color, uint8_t ch);
    void drawString(uint8_t x, uint8_t y, uint16_t color, const char* s);
    uint8_t getHeight() const;

private:
    int8_t findKerning(uint16_t p);

    uint16_t* symbols;
    uint8_t height;
    uint8_t* data;

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
