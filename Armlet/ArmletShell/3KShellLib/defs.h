#pragma once
#pragma warning(disable:4201)

#define NULL 0
#define TRUE 1
#define FALSE 0

typedef char bool_t;
typedef signed char sbyte_t, Int8;
typedef unsigned char ubyte_t, UInt8;
typedef signed short sword_t, Int16;
typedef unsigned short uword_t, UInt16;
typedef signed int sdword_t, Int32;
typedef unsigned int udword_t, UInt32;

typedef union _gpoint_t {
	UInt16 data;
	struct {
		UInt8 Top;
		UInt8 Left;
	};

} gpoint_t, aPoint, Position;

typedef union _gsize_t {
	UInt16 data;
	struct {
		UInt8 Width;
		UInt8 Height;
	};
} gsize_t, Size;

typedef union _grect_t {
	UInt32 data;
	struct {
		gpoint_t Position;
		gsize_t Size;
	};
	struct {
		UInt8 Left;
		UInt8 Top;
		UInt8 Width;
		UInt8 Height;
	};
} grect_t, aRect;

char CallbackImplementation(char* str);
typedef char (MY_CALLBACK)(char* str);
typedef char (*PMY_CALLBACK)(char* str);

inline void InitPositionAndSize(ubyte_t left, ubyte_t top, ubyte_t width, ubyte_t height, Position* pos, Size* size)
{
	pos->Left = left;
	pos->Top = top;
	size->Height = height;
	size->Width = width;
}