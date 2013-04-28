//included in both namespaces
//#pragma once

#include "PlatformDependentSizes.h"

//prevent compilation without armletApi implementation
#ifndef PFD_SIZES
	#define MUTEX_SIZE -1
	#define FILE_SIZE -1
#endif

	//SCREEN
	#define SCREENX 160
	#define SCREENY 128

	//BUTTONS
	#define NO_BUTTON	-1

	#define BUTTON_A	0
	#define BUTTON_B	1
	#define BUTTON_C	2

	#define BUTTON_X	3
	#define BUTTON_Y	4
	#define BUTTON_Z	5

	#define BUTTON_L	6
	#define BUTTON_E	7
	#define BUTTON_R	8

	#define N_BUTTONS	9
