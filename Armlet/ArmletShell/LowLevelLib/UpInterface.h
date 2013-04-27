#pragma once

#include "Constants.h"
#include "Types.h"

void StartThread(THREAD_PROC routine, void* param);
void Clear(short backColor);
void DrawTextString(int x, int y, const char* string, int sz, short foreColor, short backColor);
void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler);
void DrawRect_kel(int x, int y, int sx, int sy, short color);
bool RequestTimer(int period, TIMER_PROC routine);
void DrawPixel(int x, int y, short c);