#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class ScatteredMenu : public MenuBase
{
public:
	
	fresult Init(IMenuItem** items, ubyte_t itemsCount)
	{
		 return MenuBaseInit(items,itemsCount);
	}
	
	virtual void SetMustHaveSelected( bool_t mustHaveSelected ) 
	{
		_MustHaveSelected = mustHaveSelected;
	}

	virtual fresult Draw() 
	{
		return MenuBase::Draw();
	}

};

