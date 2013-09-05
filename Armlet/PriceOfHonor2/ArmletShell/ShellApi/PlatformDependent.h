#pragma once

#ifdef _MSC_VER

	#define PFD_SIZES
	#define MUTEX_SIZE sizeof(void*)
	#define FILE_SIZE sizeof(void*)

	#define __CALLBACK __cdecl

#if !(defined(_PC_VISUALIZER_)||defined(_PC_KERNEL_)||defined(_ARMLET_))
	#error "Platform component unknown"
#endif

#ifdef _PC_VISUALIZER_
	#define __CALLOUT __declspec(dllimport) __cdecl		//Visualizer calls ApplicationDll
	#define __LOWCALL __declspec(dllimport) __cdecl		//Visualizer calls KernelDll
	#define __LOWVAR  __declspec(dllimport)				//pfn to Visualizer for KernelDll
#endif
#ifdef _PC_KERNEL_
	#define __LOWCALL __declspec(dllexport) __cdecl		//KernelDll called by Visualizer
	#define __SYSCALL __declspec(dllexport) __cdecl		//KernelDll called by ApplicationDll
	#define __LOWVAR  __declspec(dllexport)				//pfn to Visualizer for KernelDll
#endif
#ifdef _ARMLET_
	#define __NOCALL __cdecl							//implemented by ApplicationDll
														//never calls Visualizer directly
	#define __SYSCALL __declspec(dllimport) __cdecl		//Application calls KernelDll		(Armlet App/OS)
	#define __CALLOUT __declspec(dllexport) __cdecl		//Application called by Visualizer	(by Armlet App/OS)
#endif

#else //!_MSC_VER

#define PFD_SIZES

    #define MUTEX_SIZE 1
    #define FILE_SIZE   558 //sizeof(FIL)

	#define __CALLBACK

	#define __NOCALL
	#define __SYSCALL
	#define __CALLOUT

#endif

