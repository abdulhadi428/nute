#pragma once

#ifdef _MSC_VER
	#define PFD_SIZES
	#define MUTEX_SIZE sizeof(void*)
	#define FILE_SIZE sizeof(void*)
	#define __SYSCALL __cdecl
	#define __CALLOUT __cdecl
	#define __NOCALL __cdecl
	#define __FUTURE __cdecl
	#define __CALLBACK __cdecl
#endif

#ifndef _MSC_VER
#error here
	#define __NOCALL
	#define __CALLOUT
	#define __SYSCALL
	#define __FUTURE
	#define __CALLBACK
#endif

//TODO KREYL - add CHIBI OS sizes