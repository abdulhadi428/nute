#pragma once
#include "ArmletShell.h"

namespace ThreeKShell {
#define _NAMESPACE_PROTECT_
	
	//rendering
	#include "IRender.h"
	#include "GraphicRenderer.h"

	#include "Styles.h"

	#include "IControl.h"
	#include "IScrollable.h"
	#include "ITextStream.h"

	#include "ControlBase.h"
	#include "ScrollableControlBase.h"

	#include "TextField.h"
	#include "TextFieldFactory.h"

	#include "SystemImagesLib.h"	//common images lib
	#include "PictureBox.h"
	#include "ImageList.h"
	#include "PictureBoxFactory.h"
	#include "ImageListFactory.h"

	//panels
	#include "PanelBase.h"
	#include "Panel.h"
	#include "PanelFactory.h"

	//menus
	#include "IMenuItem.h"
	#include "IMenuHandler.h"
	#include "MenuItemBase.h"
	#include "MenuItem.h"
	#include "IMenu.h"
	#include "MenuBase.h"
	#include "ScatteredMenu.h"
	#include "MenuItemFactory.h"
	#include "MenuFactory.h"

	#include "Factories.h"

//#undef _NAMESPACE_PROTECT_
}
using namespace ThreeKShell;


