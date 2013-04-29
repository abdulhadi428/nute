#pragma once

namespace ArmletApi {
	#include "Common.h"

	//by default API return bool with false is a fail, true success
	//						or void
	//using pure c++ types

	//nocall:	kernel never calls this api, api is implemented by shell
	//callout:  kernel calls this API to notify shell about input, api is implemented by shell
	//syscalls: shells calls kernel, api is implemented by kernel
	//callback: shell provides callback to kernel via some syscall, callback function is implemented by shell

	bool CALLOUT InitializeShell();										//initialization callout (kernel calls shell)
	void CALLOUT CriticalError(char* error);							//critical error callout (kernel calls shell to display aka "bsod"
	void NOCALL  Log(char* fmt, ...);									//internal realization, wrote result to log.txt via sprintf
	void SYSCALL sprintf(buf,fmt,....);									//syscall sprintf support

	//ARMLET
	unsigned char SYSCALL GetBatteryLevel();							//syscall returns 0-100%
	void SYSCALL DrawPixel(int x, int y, unsigned short c);				//syscal draws a pixel
	void SYSCALL DrawArea(int x, int y, unsigned short *const pc, int len,
					int sx, int sy);									//syscall draws an area of pixels
	void SYSCALL DoVibro(int msecs);									//syscall performs vibration for duration

	void CALLOUT OnButtonPress(int button_id);							//callout for  button
	void CALLOUT OnButtonRelease(int button_id);						//callout for button
	void CALLOUT OnPillConnect(int cure_id, int charges);				//callout cure/pill
	bool SYSCALL WritePill(int cure_id, int charges);					//syscall writes cure to pill

	unsigned char SYSCALL GetLustraId();								//syscall returns lustra id (UNKNOWN_ID=none)
	unsigned char FUTURE GetLockId();									//UNKNOWN_ID	syscall return lock id (UNKNOWN_ID=none)
	bool FUTURE OpenLock();												//TRUE			syscall should request lock to open
	bool FUTURE DenyLock();												//TRUE			syscall should request lock to show access denied
	
	//SERVER
	//радиопротокол реализует логику высокого уровня общения с сервером, траспортный уровень не здесь 
	void CALLOUT OnRadioPacket(unsigned char* packet, int len);			//callout for radio transport
	void SYSCALL SendRadioPacket(unsigned char* packet, int len);		//syscall for radio transport
	short SYSCALL GetArmletId();

	//OS
	typedef char MUTEX[MUTEX_SIZE];										//OS-dependent mutext storage
	typedef char FILE[FILE_SIZE];										//OS-dependent file storage

	bool SYSCALL OpenFile(FILE* file, char* filename, bool bCreate);	//syscall opens/create file
	int SYSCALL ReadFile(FILE* file, char* buf, int len);				//syscall returns length read
	int SYSCALL WriteFile(FILE* file, char* buf, int len);				//syscall returns length written
	int SYSCALL AppendFile(FILE* file, char* buf, int len);				//syscall return length written

	typedef void (CALLBACK THREAD_PROC)(void* param);					//thread start routine callback
	typedef bool (CALLBACK TIMER_PROC)(int elapsed);					//timer routine callback,
																		//	returns false to stop
																		//	returns true for next period
	bool SYSCALL StartThread(THREAD_PROC* startProc, void* param);
	bool SYSCALL RequestTimer(TIMER_PROC* timerProc, int period);		//syscall, period in msecs
	int  SYSCALL GetUpTime();											//return msces from armlet start
	void SYSCALL Sleep(int msces);
	void SYSCALL InitMutex(MUTEX* mutex);								//syscall, should not fail
	void SYSCALL AcquireMutex(MUTEX* mutex);							//syscall, acquires mutex; TODO const qualifier
	void SYSCALL ReleaseMutex(MUTEX* mutex);							//syscall, releases mutex; TODO const qualifier
	unsigned int SYSCALL GetRandom(unsigned int max);					//returns random value in range [0..max]

	//SPECIAL PLATFORM DEPENDENT
#ifdef _MBCS
	void SYSCALL SetCureName(int cure_id, char* name);					//visualizer only syscall
#endif

} //namespace
