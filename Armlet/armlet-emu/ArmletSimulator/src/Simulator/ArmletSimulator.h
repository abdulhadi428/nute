#pragma once
#include <WinDef.h>
//#include "resource.h"
void sim_putPixel(int x, int y, COLORREF c);
void sim_repaint();
COLORREF sim_getPixel(int x, int y);

void armlet_main();