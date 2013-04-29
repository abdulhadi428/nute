#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string.h>
#include "ArmletApi.h"
#include "Helper.h"

#define LOG_FILE	((FILE*)333)

namespace ArmletApi {

	//opens/create file
	bool __SYSCALL OpenFile(FILE* file, char* filename, bool bCreate)
	{
		//TODO
		return true;
	}

	//__SYSCALL returns length read
	int __SYSCALL ReadFile(FILE* file, char* buf, int len)
	{
		return 0;
	}

	//__SYSCALL returns length written
	int __SYSCALL WriteFile(FILE* file, char* buf, int len)
	{
		return len;
	}

	//__SYSCALL return length written
	int __SYSCALL AppendFile(FILE* file, char* buf, int len)
	{
		return 0;
	}

	bool __SYSCALL StartThread(THREAD_PROC* startProc, void* param)
	{
		//TODO
		DWORD dwTid;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startProc, param, 0, &dwTid);
		return true;
	}

	//__SYSCALL, period in msecs
	bool __SYSCALL RequestTimer(TIMER_PROC* timerProc, int period)
	{
		//timer routine __CALLBACK,
		//	returns false to stop
		//	returns true for next period
		return true;
	}

	void __SYSCALL Sleep(int msces)
	{
	}

	//should not fail
	void __SYSCALL InitMutex(MUTEX* mutex)
	{
	}

	//acquires mutex
	void __SYSCALL AcquireMutex(MUTEX* mutex)
	{
	}

	//releases mutex
	void __SYSCALL ReleaseMutex(MUTEX* mutex)
	{
	}

} //namespace
