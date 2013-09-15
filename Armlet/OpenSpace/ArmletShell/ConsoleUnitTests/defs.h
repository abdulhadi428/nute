#pragma once

#include <string.h>
//adds add string to src. returns newly allocated string
//char* StringAdd(const char* src, const char* add);

#include "ArmletAppSDK.h"

char CallbackImplementation(char* str);
typedef char (MY_CALLBACK)(char* str);
typedef char (*PMY_CALLBACK)(char* str);

#define clBlack		1
#define clWhite		2
#define clRed		3
#define clGrey		4