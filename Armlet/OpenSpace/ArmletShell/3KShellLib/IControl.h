#pragma once

namespace ThreeKShell {

/* Text Field:
IControl interface
*/

//#include "IRender.h"

class IControl
{

public:
	
	virtual Size GetSize()=0;
	virtual Position GetPosition()=0;
	virtual ubyte_t GetZOrder()=0;
	virtual bool_t GetVisible()=0;

	//Setters
	virtual void SetVisible(bool_t visible)=0;

	virtual void SetZOrder(ubyte_t ZOrder)=0;

	//Set the size;
	virtual fresult SetSize(Size size)=0;

	//Set the position
	virtual fresult SetPosition(Position position)=0;

	//Logic
	
	//renders control content to screen
	virtual fresult Draw()=0;

	//renders control partially to screen
	virtual fresult DrawArea(Position pos, Size size)=0;
};

}