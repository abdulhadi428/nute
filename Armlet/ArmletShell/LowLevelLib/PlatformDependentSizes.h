#pragma once

#ifdef _MBCS
	#define PFD_SIZES
	#define MUTEX_SIZE sizeof(void*)
	#define FILE_SIZE sizeof(void*)
#endif

//TODO KREYL - add CHIBI OS sizes