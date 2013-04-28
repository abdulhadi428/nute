#pragma once

#include "project.h"
#include "IMenuItem.h"

class IMenuHandler
{
public:
	fresult virtual	OnClick(IMenuItem* sender)=0;
};