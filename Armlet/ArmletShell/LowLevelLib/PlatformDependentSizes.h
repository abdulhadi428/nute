#pragma once

#ifdef _MBCS
	#define PFD_SIZES
	#define MUTEX_SIZE sizeof(void*)
	#define FILE_SIZE sizeof(void*)
	#define SYSCALL	__cdecl
	#define CALLOUT	__cdecl
	#define NOCALL  __cdecl
	#define FUTURE	__cdecl
	#define CALLBACK __cdecl
#endif

#ifndef _MBCS
	#define NOCALL
	#define CALLOUT
	#define SYSCALL
	#define FUTURE
	#define CALLBACK
#endif

//TODO KREYL - add CHIBI OS sizes