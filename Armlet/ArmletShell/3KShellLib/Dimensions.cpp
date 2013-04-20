#include "project.h"
#include "Dimensions.h"

int SizeIsSmaller( const Size size1, const Size size2 )
{
	if (size1.Width > size2.Width)
	{
		return -1;
	}

	if (size1.Height > size2.Height)
	{
		return -2;
	}

	return 0;
}

int IsEmpty(Size size )
{
	if (size.Width > 0 && size.Height >0)
		return 0;
	
	return 1;
}

int IsPositionInsideSize(Size size, Position position)
{
	if (position.Top > size.Height || position.Left > size.Width)
	{
		return 0;
	}

	return 1;
}

bool_t IsRectInsideRect(Position testPos, Size testSize, Position checkedPosition, Size checkedSize)
{
	if (testPos.Left < checkedPosition.Left)
	{
		return 0;
	}

	if (testPos.Top < checkedPosition.Top)
	{
		return 0;
	}

	if (testSize.Width < checkedSize.Width)
	{
		return 0;
	}

	if (testSize.Height < checkedSize.Height)
	{
		return 0;
	}
	return 1;
}

bool_t GetLineIntersection( ubyte_t start1, ubyte_t length1, ubyte_t start2, ubyte_t length2, ubyte_t* resStart, ubyte_t* resLength)
{
	ubyte_t _leftEnd, _rightStart, _rightEnd;
	if (start1 <= start2) 
	{
		//_leftStart = start1;
		_leftEnd = start1+length1;
		_rightStart = start2;
		_rightEnd = start2+length2;
	}
	else
	{
		//_leftStart = start2;
		_leftEnd = start2+length2;
		_rightStart = start1;
		_rightEnd = start1+length1;
	}

	if (_leftEnd < _rightStart)
		return FALSE;

	*resStart = _rightStart;
	if (_rightEnd < _leftEnd) 
	{
		*resLength = _rightEnd - _rightStart;
	}
	else
	{
		*resLength = _leftEnd - _rightStart;
	}
	return TRUE;
}

bool_t GetRectClipping(Position pos1, Size size1, Position pos2, Size size2, Position* resPos, Size* resSize)
{
	bool_t res = FALSE;

	if (GetLineIntersection(pos1.Left,size1.Width,pos2.Left,size2.Width, &resPos->Left, &resSize->Width))
	{
		if (GetLineIntersection(pos1.Top,size1.Height,pos2.Top,size2.Height, &resPos->Top, &resSize->Height))
		{
			return TRUE;
		}
	}
	return FALSE;
}

