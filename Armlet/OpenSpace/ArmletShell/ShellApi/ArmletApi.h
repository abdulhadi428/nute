#pragma once
#include "Common.h"
#include "PlatformDependent.h"

namespace ArmletApi {

//prevent compilation without platform dependent sizes implementation
#ifndef PFD_SIZES
	#define MUTEX_SIZE -1
	#define FILE_SIZE -1
#endif

	//by default API return bool with false is a fail, true success
	//						or void
	//using pure c++ types

	//NOTE: currently all API is supposed to be serialized and is running in single thread !!!

	//__CALLBACK: shell provides __CALLBACK to kernel via some __SYSCALL, __CALLBACK function is implemented by shell
	typedef bool (__CALLBACK TIMER_PROC)(int elapsed);					//timer routine __CALLBACK,
																		//	returns false to stop
																		//	returns true for next period

	//__CALLOUT:  kernel calls this API to notify shell about input, api is implemented by shell
#ifdef __CALLOUT
	bool __CALLOUT InitializeShell();									//initialization __CALLOUT (kernel calls shell)
	void __CALLOUT CriticalError(char* error);							//critical error __CALLOUT (kernel calls shell to display aka "bsod")
	void __CALLOUT OnButtonPress(int button_id);						//__CALLOUT for button
	void __CALLOUT OnButtonRelease(int button_id);						//__CALLOUT for button
	void __CALLOUT OnPillConnect(int pill_id, int charges);				//__CALLOUT cure/torture
	void __CALLOUT OnRadioPacket(unsigned char* packet, int len);		//__CALLOUT for radio transport
	//SPECIAL PLATFORM DEPENDENT
	#ifdef _MSC_VER
		void __CALLOUT NextMedTick();									//visualizer only __CALLOUT
	#endif
#endif

	//__SYSCALLs: shells calls kernel, api is implemented by kernel
#ifdef __SYSCALL
	//ARMLET
	unsigned char __SYSCALL GetBatteryLevel();							//__SYSCALL returns 0-100%
	void __SYSCALL SetScreenBrightness(unsigned char percent);			//__SYSCALL sets screen brightness
	void __SYSCALL DrawPixel(int x, int y, unsigned short c);			//__SYSCALL draws a pixel
	void __SYSCALL DrawArea(int x, int y, unsigned short *const pc, int len,
					int sx, int sy);									//__SYSCALL draws an area of pixels
	void __SYSCALL DoVibroAndBeep(int msecs);							//__SYSCALL performs vibration for duration

	bool __SYSCALL WritePill(int pill_id, int charges);					//__SYSCALL writes cure to pill

	unsigned char __SYSCALL GetLustraId();								//__SYSCALL returns lustra id (UNKNOWN_ID=none)
	unsigned short __SYSCALL GetArmletId();

	//SERVER
	//радиопротокол реализует логику высокого уровня общения с сервером, траспортный уровень не здесь
	void __SYSCALL SendRadioPacket(unsigned char* packet, int len);		//__SYSCALL for radio transport
	void __SYSCALL SendAppState(unsigned char packet[APP_STATE_LEN]);	//__SYSCALL for datagram trapsport
	void __SYSCALL GetRadioStatus(int* gate_id, int* signal_level);

	//OS (OsEmulation.cpp)
	typedef char MUTEX[MUTEX_SIZE];										//OS-dependent mutext storage
	typedef char FILE[FILE_SIZE];										//OS-dependent file storage

	unsigned int __SYSCALL GetRandom(unsigned int max);					//returns random value in range [0..max]
	int  __SYSCALL GetUpTime();											//return msces from armlet start
	int __SYSCALL snprintf(char* buf, int bufSz, char* fmt,...);		//__SYSCALL sprintf support,
	bool __SYSCALL OpenFile(FILE* file, const char* filename, bool bCreate);	//__SYSCALL opens/create file
	bool __SYSCALL CloseFile(FILE* file);								//__SYSCALL closes file

	int __SYSCALL ReadFile(FILE* file, char* buf, int len);				//__SYSCALL returns length read
	int __SYSCALL WriteFile(FILE* file, char* buf, int len);			//__SYSCALL returns length written
	int __SYSCALL AppendFile(FILE* file, char* buf, int len);			//__SYSCALL return length written

	bool __SYSCALL RequestTimer(TIMER_PROC* timerProc, int period);		//__SYSCALL, period in msecs
	//SPECIAL PLATFORM DEPENDENT
	#ifdef _MSC_VER
		void __SYSCALL SetPillName(int pill_id, char* name);			//visualizer only __SYSCALL
	#endif
#endif

	//__NOCALL:	kernel never calls this api, api is implemented by shell
#ifdef __NOCALL
	void __NOCALL Log(char* fmt,...);									//internal realization, wrote result to log.txt via snprintf
#endif

#ifdef __FUTURE
	typedef void (__CALLBACK THREAD_PROC)(void* param);					//thread start routine __CALLBACK
	unsigned char __FUTURE GetLockId();									//UNKNOWN_ID	__SYSCALL return lock id (UNKNOWN_ID=none)
	bool __FUTURE OpenLock();											//TRUE			__SYSCALL should request lock to open
	bool __FUTURE DenyLock();											//TRUE			__SYSCALL should request lock to show access denied
	bool __FUTURE StartThread(THREAD_PROC* startProc, void* param);		//FALSE			__SYSCALL creates a thread
	void __FUTURE  Sleep(int msces);
	void __FUTURE  InitMutex(MUTEX* mutex);								//				__SYSCALL, should not fail
	void __FUTURE  AcquireMutex(MUTEX* mutex);							//				__SYSCALL, acquires mutex; TODO const qualifier
	void __FUTURE  ReleaseMutex(MUTEX* mutex);							//				__SYSCALL, releases mutex; TODO const qualifier
#endif

} //namespace
