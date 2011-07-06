/*
 * kl_ini.h
 *
 *  Created on: 03.07.2011
 *      Author: Kreyl
 */

#ifndef KL_INI_H_
#define KL_INI_H_

#include "ff.h"
#include "diskio.h"
#include <inttypes.h>

#define INI_BUF_SIZE    512

char *skipleading(char *S);
char *skiptrailing(char *S, const char *base);
char *striptrailing(char *S);

char *ReadString(const char *ASection, const char *AKey, char *AOutput, uint32_t AMaxLength, const char *AFileName);
int32_t ReadInt32(const char *ASection, const char *AKey, const char *AFileName);

#endif /* KL_INI_H_ */
