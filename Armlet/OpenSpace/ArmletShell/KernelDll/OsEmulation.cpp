#include "ArmletKernel.h"

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <list>
#include "time.h"	//time

typedef struct _TIMER {
	int current;
	int period;
	ArmletApi::TIMER_PROC* routine;
} TIMER;
std::list<TIMER*> gTimerList;

namespace ArmletApi {

	static bool bsrand = false;
	unsigned int __SYSCALL GetRandom(unsigned int max)
	{
		if (!bsrand) {
			srand((unsigned int)time(NULL));
			bsrand = true;
		}
		if (max < 0) return 0;
		int _max = max+1;
		int rnd = rand() % (_max);
		if (rnd<0) {
			rnd = 0;
		}
		if (rnd >= _max) {
			rnd = _max - 1;
		}
		return (unsigned int)rnd;
	}

	int  __SYSCALL GetUpTime()
	{
		static time_t startTime = 0; 
		if (startTime == 0) {
			startTime = time(NULL);
			return 0;
		}
		__int64 uptime = (__int64)( time(NULL) - startTime );
		int _uptime = (int)(uptime*1000);
		if (_uptime < 0) {
			startTime = 0;
			return 0;
		}
		return _uptime;
	}

	int __SYSCALL snprintf(char* buf, int bufSz, char* fmt,...)
	{
		va_list args;
		va_start(args, fmt);
		return ::_vsnprintf( buf, bufSz, fmt, args);
	}

	//opens/create file
	bool __SYSCALL OpenFile(FILE* file, const char* filename, bool bCreate)
	{
		COMPILE_TIME_CHECK(sizeof(FILE)==sizeof(HANDLE));
		HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
			NULL, bCreate ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;
		memcpy(file, &hFile, 4);
		return true;
	}

	//__SYSCALL returns length read
	int __SYSCALL ReadFile(FILE* file, char* buf, int len)
	{
		DWORD retlen;
		COMPILE_TIME_CHECK(sizeof(FILE)==sizeof(HANDLE));
		HANDLE hFile;
		memcpy(&hFile, file, sizeof(FILE));
		if (len < 0) return 0;

		SetFilePointer(hFile,0,0,FILE_BEGIN);

		BOOL res = ::ReadFile( hFile, buf, len, &retlen, NULL);
		if (res) return retlen;
		return 0;
	}

	//__SYSCALL returns length written
	int __SYSCALL WriteFile(FILE* file, char* buf, int len)
	{
		DWORD retlen;
		COMPILE_TIME_CHECK(sizeof(FILE)==sizeof(HANDLE));
		HANDLE hFile;
		memcpy(&hFile, file, sizeof(FILE));
		if (len < 0) return 0;

		SetFilePointer(hFile,0,0,FILE_BEGIN);
		SetEndOfFile(hFile);

		BOOL res = ::WriteFile( hFile, buf, len, &retlen, NULL);
		if (res) return retlen;
		return 0;
	}

	//__SYSCALL return length written
	int __SYSCALL AppendFile(FILE* file, char* buf, int len)
	{
		DWORD retlen;
		COMPILE_TIME_CHECK(sizeof(FILE)==sizeof(HANDLE));
		HANDLE hFile;
		memcpy(&hFile, file, sizeof(FILE));
		if (len < 0) return 0;

		SetFilePointer(hFile,0,0,FILE_END);
		SetEndOfFile(hFile);

		BOOL res = ::WriteFile( hFile, buf, len, &retlen, NULL);
		if (res) return retlen;
		return 0;
	}

	//bool __FUTURE StartThread(THREAD_PROC* startProc, void* param)
	//{
	//	return false;
	//	//DWORD dwTid;
	//	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startProc, param, 0, &dwTid);
	//}

	//__SYSCALL, period in msecs
	bool __SYSCALL RequestTimer(TIMER_PROC* timerProc, int period)
	{
		//timer routine __CALLBACK,
		//	returns false to stop
		//	returns true for next period

		if (period < 100)
			return false;

		TIMER* newTimer = new TIMER;
		newTimer->current = 0;
		newTimer->period = period;
		newTimer->routine = timerProc;

		gTimerList.push_back( newTimer );
		return true;
	}

	//void __FUTURE Sleep(int msces)
	//{
	//}

	//should not fail
	//void __FUTURE InitMutex(MUTEX* mutex)
	//{
	//}

	//acquires mutex
	//void __FUTURE AcquireMutex(MUTEX* mutex)
	//{
	//}

	//releases mutex
	//void __FUTURE ReleaseMutex(MUTEX* mutex)
	//{
	//}


} //namespace

namespace KernelApi {

	void __LOWCALL CheckTimers()
	{
		//50 msecs were elasped
		std::list<TIMER*>::iterator itr;

		for(itr = gTimerList.begin(); itr != gTimerList.end();)
		{
			TIMER* curr = *itr;
			curr->current += 50;
			if ( curr->current >= curr->period )	{
				if (curr->routine(curr->period)) {
					curr->current -= curr->period;
				}
				else {
					delete curr;
					itr=gTimerList.erase(itr);
				}
			}
			else
				++itr;
		}
	}

} //namespace
