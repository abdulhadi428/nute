#pragma once
#include "IMenuItem.h"

namespace ThreeKShell {

class IMenuHandler
{
public:
	fresult virtual	OnClick(IMenuItem* sender)=0;
};

}