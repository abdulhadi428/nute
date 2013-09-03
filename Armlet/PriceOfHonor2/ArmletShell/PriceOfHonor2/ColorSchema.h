#pragma once

#include "ArmletShell.h"

#define FormatEmpty -1
#define FormatHeader 0
#define FormatMenu 1
#define FormatText 2
#define FormatParrot 3

#define DEFAULT_BACKGROUND 0xFFFF

extern FormatsRepository_DEPRECATED gFormatsRepository; 

fresult InitColorSchema();