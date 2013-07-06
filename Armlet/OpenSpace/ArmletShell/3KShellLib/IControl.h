#pragma once

namespace ThreeKShell {

/* Text Field:
IControl interface
*/

//#include "IRender.h"

class IControl
{

public:
	
	Size virtual GetSize()=0;
	Position virtual GetPosition()=0;
	ubyte_t virtual GetZOrder()=0;
	bool_t virtual GetVisible()=0;

	//Setters
	void virtual SetVisible(bool_t visible)=0;

	void virtual SetZOrder(ubyte_t ZOrder)=0;

	//Set the size;
	fresult virtual SetSize(Size size)=0;

	//Set the position
	fresult virtual SetPosition(Position position)=0;

	//Logic
	
	//renders control content to screen
	fresult virtual Draw()=0;

	//renders control partially to screen
	fresult virtual DrawArea(Position pos, Size size)=0;
};

}