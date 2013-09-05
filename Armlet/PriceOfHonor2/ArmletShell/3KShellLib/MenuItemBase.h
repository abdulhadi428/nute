#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

class MenuItemBase : public  IMenuItem
{
protected:
	IControl* _underLyningControl;
	bool_t _IsSelected;
	bool_t _IsVisible;
	bool_t _IsEnabled;

	IMenuHandler* _OnClickHandler;
	ButtonState _Accelerator;
	ButtonState _Accelerator2;

	fresult MenuItemBaseInit(IControl* _control, IMenuHandler* handler, ButtonState accelerator);

public:

	fresult virtual Draw();
	bool_t virtual IsSelected();
	fresult virtual Select();
	fresult virtual Deselect();
	fresult virtual Click();
	fresult virtual SetSecondAccelarator(ButtonState button);

	ButtonState GetAccelerator();

	bool_t virtual CheckAccelerator(ButtonState button);

	virtual fresult SetVisible( bool_t val );

	virtual bool_t GetVisible();

	virtual fresult SetEnabled( bool_t val );

	virtual bool_t GetEnabled();


};
