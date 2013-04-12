#ifndef LCD2630__H
#define LCD2630__H

// Screen-related section
typedef unsigned short AL_SCREEN_COLOR_TYPE;
typedef unsigned char AL_SCREEN_COORD_TYPE;

#include <stdint.h>

class Lcd_t
{
public:
    //void Cls(Color_t Color);

    void GetBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf);
    void PutBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf);

};

extern Lcd_t Lcd;

#endif
