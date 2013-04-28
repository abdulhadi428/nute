#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string.h>
#include "ArmletApi.h"
#include "Helper.h"

#define LOG_FILE	((FILE*)333)

namespace ArmletApi {

	//opens/create file
	bool OpenFile(FILE* file, char* filename, bool bCreate)
	{
		//TODO
		return true;
	}

	//syscall returns length read
	int ReadFile(FILE* file, char* buf, int len)
	{
		return 0;
	}

	//syscall returns length written
	int WriteFile(FILE* file, char* buf, int len)
	{
		return len;
	}

	//syscall return length written
	int AppendFile(FILE* file, char* buf, int len)
	{
		return 0;
	}

	bool StartThread(THREAD_PROC* startProc, void* param)
	{
		//TODO
		DWORD dwTid;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startProc, param, 0, &dwTid);
		return true;
	}

	//syscall, period in msecs
	bool RequestTimer(TIMER_PROC* timerProc, int period)
	{
		//timer routine callback,
		//	returns false to stop
		//	returns true for next period
		return true;
	}

	void Sleep(int msces)
	{
	}

	//should not fail
	void InitMutex(MUTEX* mutex)
	{
	}

	//acquires mutex
	void AcquireMutex(MUTEX* mutex)
	{
	}

	//releases mutex
	void ReleaseMutex(MUTEX* mutex)
	{
	}

} //namespace
