#pragma once

namespace ArmletApi {
	#include "Common.h"

	//by default API return bool with false is a fail, true success
	//						or void
	//using pure c++ types

	//callout: kernel calls this API to notify shell about input
	//syscalls: shells calls kernel
	//callback: shell provides callback to kernel via some syscall

	void InitializeShell();												//initialization callout

	//ARMLET
	void DrawPixel(int x, int y, unsigned short c);						//syscal draws a pixel
	void DrawArea(int x, int y, unsigned short *const pc, int len,
					int sx, int sy);									//syscall draws an area of pixels
	void DoVibro(int msecs);											//syscall performs vibration for duration

	#define UNKNOWN_ID	255
	void OnButtonPress(int button_id);									//callout for  button
	void OnButtonRelease(int button_id);								//callout for button
	void OnPillConnect(int cure_id, int charges);						//callout cure/pill
	bool WritePill(int cure_id, int charges);							//syscall writes cure to pill

	unsigned char GetLustraId();										//syscall returns lustra id (UNKNOWN_ID=none)
	unsigned char GetLockId();											//syscall return lock id (UNKNOWN_ID=none)
	bool OpenLock();													//syscall should request lock to open
	bool DenyLock();													//syscall should request lock to show access denied
	
	//SERVER
	//радиопротокол реализует логику высокого уровня общения с сервером, траспортный уровень не здесь 
	void OnRadioPacket(unsigned char* packet, int len);					//callout for radio transport
	void SendRadioPacket(unsigned char* packet, int len);				//syscall for radio transport

	//OS
	typedef char MUTEX[MUTEX_SIZE];										//OS-dependent mutext storage
	typedef char FILE[FILE_SIZE];										//OS-dependent file storage

	bool OpenFile(FILE* file, char* filename, bool bCreate);			//syscall opens/create file
	int ReadFile(FILE* file, unsigned char* buf, int len);				//syscall returns length read
	int WriteFile(FILE* file, unsigned char* buf, int len);				//syscall returns length written
	int AppendFile(FILE* file, unsigned char* buf, int len);			//syscall return length written

	typedef void (__stdcall THREAD_PROC)(void* param);					//thread start routine callback
	typedef bool (__stdcall TIMER_PROC)(int elapsed);					//timer routine callback,
																		//	returns false to stop
																		//	returns true for next period
	bool StartThread(THREAD_PROC* startProc, void* param);
	bool RequestTimer(TIMER_PROC* timerProc, int period);				//syscall, period in msecs
	void Sleep(int msces);
	void InitMutex(MUTEX* mutex);										//syscall, should not fail
	void AcquireMutex(MUTEX* mutex);									//syscall, acquires mutex; TODO const qualifier
	void ReleaseMutex(MUTEX* mutex);									//syscall, releases mutex; TODO const qualifier

	//SPECIAL PLATFORM DEPENDENT
#ifdef _MBCS
	void SetCureName(int cure_id, char* name);							//visualizer only syscall
#endif

} //namespace
