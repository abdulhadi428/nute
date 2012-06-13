/*
 * kl_string.h
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */

#ifndef KL_STRING_H_
#define KL_STRING_H_

void klSPrintf(char *Dst, const char *Src, ...);

// Copies Src to Dst, null-terminating Dst. Returns pointer to NULL at Dst
char* klStrNCpy(char *Dst, const char *Src, uint32_t ACount);

#endif /* KL_STRING_H_ */
