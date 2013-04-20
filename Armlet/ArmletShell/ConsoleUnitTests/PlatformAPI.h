#pragma once

//That fuction is used for allocations in RAM for test purposes. MUST NOT be used on in firmware code
char* _allocBytesTest(int count);

//That fuction is used for allocations in FLASH, so that is r/o memory
char* _allocBytesFlash(int count);

//That fuction is used for allocations in SRAM, so that is r/w memory. MIND 128k limit!!!
char* _allocBytesSRAM(int count);
