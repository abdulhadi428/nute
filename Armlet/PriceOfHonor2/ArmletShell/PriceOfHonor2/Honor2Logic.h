#pragma once
#include "med.h"
class Honor2Logic
{
public:

	fresult Init();

	//UI logic entries
	char* OnNewWound(TARGET trg);
	/*fresult OnKnockout();

	//System callouts
	fresult OnPillConnected(CURE_ID pillId);
	fresult OnExposion();
	fresult SetRegenRate(ubyte_t regenRate);
*/
};