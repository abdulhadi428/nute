#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

/* Text Field:
IControl interface
*/

class IControl
{

public:
	
	virtual Size GetSize(){Size sz; sz.data=0; return sz;};
	virtual Position GetPosition(){Position pos; pos.data=0; return pos;};
	virtual ubyte_t GetZOrder(){return 0;};
	virtual bool_t GetVisible(){return 0;};

	//Setters
	virtual void SetVisible(bool_t visible){return;};

	virtual void SetZOrder(ubyte_t ZOrder){return;};

	//Set the size;
	virtual fresult SetSize(Size size){return GENERAL_ERROR;};

	//Set the position
	virtual fresult SetPosition(Position position){return GENERAL_ERROR;};

	//Logic
	
	//renders control content to screen
	virtual fresult Draw(){return GENERAL_ERROR;};

	//renders control partially to screen
	virtual fresult DrawArea(Position pos, Size size){return GENERAL_ERROR;};
};
