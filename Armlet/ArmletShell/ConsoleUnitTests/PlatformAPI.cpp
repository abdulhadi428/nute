
#include "PlatformAPI.h"

char* _allocBytesTest(int count)
{
	return new char[count];
}

char* _allocBytesFlash( int count )
{
	return _allocBytesTest(count);
}

char* _allocBytesSRAM( int count )
{
	return _allocBytesTest(count);
}

