#pragma once

#ifdef _MSC_VER

	#define PFD_SIZES
	#define MUTEX_SIZE sizeof(void*)
	#define FILE_SIZE sizeof(void*)

	#define __CALLBACK __cdecl

#ifdef _PC_VISUALIZER_
	#define __LOWCALL __declspec(dllimport) __cdecl
	#define __LOWVAR  __declspec(dllimport)
	#define __CALLOUT __declspec(dllimport) __cdecl
#else
#ifdef _PC_KERNEL_
	#define __LOWCALL __declspec(dllexport) __cdecl
	#define __LOWVAR  __declspec(dllexport)
	#define __SYSCALL __declspec(dllexport) __cdecl
#else
#ifdef _ARMLET_
	#define __SYSCALL __declspec(dllimport) __cdecl
	#define __CALLOUT __declspec(dllexport) __cdecl
	#define __NOCALL __cdecl
#else
	#error "Platform component unknown"
#endif
#endif
#endif

#endif

#ifndef _MSC_VER

#define PFD_SIZES

    #define MUTEX_SIZE 1
    #define FILE_SIZE   558 //sizeof(FIL)

	#define __NOCALL
	#define __CALLOUT
	#define __SYSCALL
	#define __CALLBACK

#endif

