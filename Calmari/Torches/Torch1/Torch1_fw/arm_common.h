/*
 * arm_common.h
 *
 *  Created on: 12.09.2011
 *      Author: Kreyl
 */

#ifndef ARM_COMMON_H_
#define ARM_COMMON_H_

#define FORCE_INLINE inline __attribute__ ((__always_inline__))
#define nop( )  asm volatile ("nop\n" ::)

#endif /* ARM_COMMON_H_ */
