/*
 * kl_string.h
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */

#ifndef KL_STRING_H_
#define KL_STRING_H_

char* klSPrintf(char *Dst, const char *Src, ...);

// Copies Src to Dst, null-terminating Dst. Returns pointer to NULL at Dst
char* klStrCpy(char *Dst, const char *Src);

// Copies ACount bytes of Src to Dst, null-terminating Dst. Returns pointer to NULL at Dst
char* klStrNCpy(char *Dst, const char *Src, uint32_t ACount);

// Checks if string starts with substring
bool klStrStartWith(const char *Str, const char *Chunk);

// Converts string to uint. returns true in case of success
bool klHexToUint(const char *Str, uint32_t *PResult);

#endif /* KL_STRING_H_ */
