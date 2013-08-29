#pragma once

	#define UNREFERENCED_PARAMETER(P)	(P)
	#define COMPILE_TIME_CHECK(e) extern char _COMPILE_TIME_CHECK_[(e)?1:-1]

	//SCREEN
	#define SCREENX 160
	#define SCREENY 128

	//APP_STATE in each datagram
	#define APP_STATE_LEN	5

	//medicine tick duration, 1min (set 3-5 secs to speedup)
	#define MED_TICK_TIME 60*1000

	//IDS
	#define UNKNOWN_ID	255

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
