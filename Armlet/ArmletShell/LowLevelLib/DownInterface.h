#pragma once

#include "Constants.h"

extern short VideoMemory[SCREENX*SCREENY];
void OnButtonClick(int button);
void OnButtonHold(int button, int seconds);
void OnElapsed50msec();

void LowLevelLibMain(void);
