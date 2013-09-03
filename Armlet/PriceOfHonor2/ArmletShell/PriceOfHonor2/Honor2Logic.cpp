#include "Honor2Logic.h"

char* Honor2Logic::OnNewWound( TARGET trg )
{
	return _medNewWound(trg);
}

fresult Honor2Logic::Init()
{
	_medInit();
	return SUCCESS;
}
