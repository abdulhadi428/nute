#ifndef _NAMESPACE_PROTECT_
	#error "Use ThreeKShell.h"
#endif

//Base class for any control
class ControlBase : public IControl
{

protected:
	Position _Position;
	Size _Size;
	bool_t Visible;
	ubyte_t _ZOrder;
	IRender* _render;

	virtual fresult BaseInit( Size size, Position position, IRender* render)
	{
		_Size = size;
		_Position = position;
		_render = render;
		Visible = TRUE;

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
		return Visible;
	}


	virtual void SetVisible( bool_t visible )
	{
		Visible = visible;
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
		if (Visible == TRUE)
		{
			return DrawArea(_Position, _Size);
		}
		else
		{
			return SUCCESS;
		}
		
	}

};
