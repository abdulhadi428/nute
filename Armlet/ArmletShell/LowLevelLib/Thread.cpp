#include "windows.h"
#include "Internal.h"

void StartThread(THREAD_PROC routine, void* param)
{
	DWORD dwTid;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)routine, param, 0, &dwTid);
	return;
}

