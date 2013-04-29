#pragma once

extern const unsigned char Font_6x8_Data[256][6];

namespace ArmletShell {

	#include "Common.h"			//from LowLevelLib

#define _NAMESPACE_PROTECT_
	#include "Types.h"			//common types
	#include "Colors.h"			//common colors
	#include "Fonts.h"			//common fonts
	#include "Drawings.h"		//common graphic output
	#include "Dimensions.h"		//common graphic calcs
	#include "strlib.h"			//common strings
	#include "Buttons.h"		//common button input
#undef _NAMESPACE_PROTECT_

}

namespace ServerProtocol {

	using namespace ArmletShell;
	#include "Common.h"			//from LowLevelLib

#define _NAMESPACE_PROTECT_
	#include "ServerProtocol.h"	//specific server radio protocol
#undef _NAMESPACE_PROTECT_

}

using namespace ArmletShell;
using namespace ServerProtocol;