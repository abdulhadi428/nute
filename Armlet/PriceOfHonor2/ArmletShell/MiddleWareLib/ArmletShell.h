#pragma once
#include "Common.h"				//common defines
#include "ServerProtocol.h"		//specific server radio protocol

extern const unsigned char Font_6x8_Data[256][6];

namespace ArmletShell {

#define _NAMESPACE_PROTECT_
	#include "Types.h"				//common types
	#include "Colors.h"				//common colors
	#include "Fonts.h"				//common fonts
	#include "Drawings.h"			//common graphic output
	#include "Dimensions.h"			//common graphic calculations
	#include "strlib.h"				//common strings
	#include "Buttons.h"			//common button input
	#include "Images.h"				//common images API
#undef _NAMESPACE_PROTECT_

}

using namespace ArmletShell;
using namespace ServerProtocol;