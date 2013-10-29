/*
 * lcd.h
 *
 *  Created on: 27.10.2013
 *      Author: Roma Jam
 */

#ifndef LCD_H_
#define LCD_H_

#include "kl_lib_f100.h"
#include "lcd_font.h"
#include "lcd_images.h"
#include <stdarg.h>
#include <tiny_sprintf.h>

// ==== USART ====
#define LCD_USART       USART3

// ==== GPIOS ====
#define LCD_GPIO		GPIOB

#define LCD_SCLK        12
#define LCD_SDA         10
#define LCD_XRES        11
#define LCD_XCS         14
#define LCD_BCKLT       15

#define LCD_WIDTH		96
#define LCD_HEIGHT		65

#define LCD_STR_HEIGHT  8
#define LCD_STR_WIDTH   16

// Data sizes
//#define LCD_VIDEOBUF_SIZE       864     // = 96 * 9
#define LCD_VIDEOBUF_SIZE         88     // =  80*9

enum PseudoGraph_t {
    CornerTopLeftDouble = 0x99,
    CornerTopRightDouble = 0x8B,
    CornerBottomLeftDouble = 0x98,
    CornerBottomRightDouble = 0x8C,
    LineHorizDouble = 0x9D,
    LineHorizDoubleUp = 0x9A,
    LineHorizDoubleDown = 0x9B,
    LineVertDouble = 0x8A,
    LineVertDoubleLeft = 0x89,
    LineVertDoubleRight = 0x9C,
    LineCrossDouble = 0x9E,
};

enum DrawMode_t {
	DRAW, // zero has no effect
	CLEAR, // zero has no effect, one clears
	OVERWRITE, // write both zeros and ones
	OVERWRITE_INVERTED, // write both with negated meaning
	INVERT // zero has no effect, one inverts
};

class Lcd_t {
private:
    uint16_t IBuf[LCD_VIDEOBUF_SIZE];
    DrawMode_t draw_mode;
    uint32_t BackligthValue;
    // Pin driving functions
    inline void InitGpios() {
        PinSetupOut(LCD_GPIO, LCD_XRES, omPushPull, ps50MHz);
        PinSetupOut(LCD_GPIO, LCD_XCS, omPushPull, ps50MHz);
        PinSetupOut(LCD_GPIO, LCD_SCLK, omPushPull, ps50MHz);
        PinSetupOut(LCD_GPIO, LCD_SDA, omPushPull, ps50MHz);
    }

    void XRES_Hi(void) { PinSet(LCD_GPIO, LCD_XRES);    }
    void XRES_Lo(void) { PinClear(LCD_GPIO, LCD_XRES);  }
    void SCLK_Hi(void) { PinSet(LCD_GPIO, LCD_SCLK);    }
    void SCLK_Lo(void) { PinClear(LCD_GPIO, LCD_SCLK);  }
    void SDA_Hi (void) { PinSet(LCD_GPIO, LCD_SDA);     }
    void SDA_Lo (void) { PinClear(LCD_GPIO, LCD_SDA);   }
    void WriteCmd(uint8_t ACmd);
    void WriteData(uint8_t AData);
    // High-level
    void DrawBlock(int index, uint8_t data, uint8_t mask);
    void DrawChar(int *index, uint8_t AChar);

public:
    // IRQ
    void XCS_Hi (void) { PinSet(LCD_GPIO, LCD_XCS);     }
    void XCS_Lo (void) { PinClear(LCD_GPIO, LCD_XCS);   }
    // General use
    void Init(void);
    void Task(void);
    void Shutdown(void);
    void Backlight(uint8_t ABrightness)  { TIM15->CCR2 = ABrightness; }

    // High-level
    void SetDrawMode(DrawMode_t mode) { draw_mode = mode; }

    void Printf(int column, int row, const char *S, ...);
    void DrawImage(int x, int y, const uint8_t *img);
#ifdef ENABLE_DMAUSART_MODE
    void Cls(void) { for(int i=0; i < LCD_VIDEOBUF_SIZE; i++) IBuf[i] = 0x0001; }
#else
    void Cls(void) { for(int i=0; i < LCD_VIDEOBUF_SIZE; i++) IBuf[i] = 0x0000; }
#endif
    /* ==== Pseudographics ====
     *  Every command consists of PseudoGraph_t AChar, uint8_t RepeatCount.
     *  Example: LineHorizDouble, 11 => print LineHorizDouble 11 times.
     *  Cmds end up with 0.
     */
    void Symbols(int column, int row, ...);

    void PutPixel(int x, int y, int c) {
//    	assert(c == 0 || c == 1);
    	int index = x + (y/8)*LCD_WIDTH;
    	uint8_t mask = 1 << (7-y%8);
		DrawBlock(index, c ? mask : 0, mask);
    }
};

extern Lcd_t Lcd;
#endif /* LCD_H_ */
