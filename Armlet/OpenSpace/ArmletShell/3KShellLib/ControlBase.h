#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

//Base class for any control
class ControlBase : public IControl
{

protected:
	Position _Position;
	Size _Size;
	//TODO: rename
	bool_t _bVisible;
	ubyte_t _ZOrder;
	IRender* _render;

	virtual fresult BaseInit( Size size, Position position, IRender* render)
	{
		_Size = size;
		_Position = position;
		_render = render;
		_bVisible = TRUE;

		return SUCCESS;
	}

public:

	virtual Size GetSize() 
	{
		return _Size;
	}

	virtual Position  GetPosition() 
	{
		return _Position;
	}

	virtual ubyte_t GetZOrder()
	{
		return _ZOrder;
	}

	virtual bool_t GetVisible()
	{
		return _bVisible;
	}


	virtual void SetVisible( bool_t visible )
	{
		_bVisible = visible;
	};

	virtual void SetZOrder( ubyte_t ZOrder )
	{
		_ZOrder= ZOrder;
	};

	virtual fresult SetSize( Size size )
	{
		_Size = size;
		return SUCCESS;
	}

	virtual fresult SetPosition( Position position )
	{
		_Position = position;
		return SUCCESS;
	}

	virtual fresult Draw() 
	{
		if (_bVisible == TRUE)
		{
			return DrawArea(_Position, _Size);
		}
		else
		{
			return SUCCESS;
		}
		
	}

};
