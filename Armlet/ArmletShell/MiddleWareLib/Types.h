#ifndef _NAMESPACE_PROTECT_
	#error "Use ArmletShell.h"
#endif

#define COMPILE_TIME_CHECK(e) extern ubyte_t _COMPILE_TIME_CHECK_[(e)?1:-1]

#define NULL 0
#define TRUE 1
#define FALSE 0

typedef bool bool_t;
typedef signed char sbyte_t;		//Int8
typedef unsigned char ubyte_t;		//UInt8
typedef signed short sword_t;		//Int16
typedef unsigned short uword_t;		//UInt16
typedef signed int sdword_t;		//Int32
typedef unsigned int udword_t;		//UInt32

typedef sbyte_t fresult;

// 0 - ok
// +value - warning
// -value - error

#define WIDTH_EXCEEDS -1
#define HEIGHT_EXCEEDS -2
#define GENERAL_ERROR -1
#define GENERAL_WARNING 1
#define SUCCESS 0

#pragma warning(disable:4201)

typedef union _gpoint_t {
	uword_t data;
	struct {
		ubyte_t Top;
		ubyte_t Left;
	};
} gpoint_t, Position;

typedef union _gsize_t {
	uword_t data;
	struct {
		ubyte_t Width;
		ubyte_t Height;
	};
} gsize_t, Size;

typedef union _grect_t {
	udword_t data;
	struct {
		gpoint_t Position;
		gsize_t Size;
	};
	struct {
		ubyte_t Left;
		ubyte_t Top;
		ubyte_t Width;
		ubyte_t Height;
	};
} grect_t, aRect;

#pragma warning(default:4201)

inline void InitPositionAndSize(
	ubyte_t left, ubyte_t top, ubyte_t width, ubyte_t height, 
	Position* pos, Size* size)
{
	pos->Left = left;
	pos->Top = top;
	size->Height = height;
	size->Width = width;
}