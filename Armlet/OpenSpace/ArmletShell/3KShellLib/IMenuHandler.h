#pragma once
#include "IMenuItem.h"

namespace ThreeKShell {

class IMenuHandler
{
public:
	virtual fresult OnClick(IMenuItem* sender)=0;
};

}