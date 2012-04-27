/*
 * arm_common.h
 *
 *  Created on: 12.09.2011
 *      Author: Kreyl
 */

#ifndef ARM_COMMON_H_
#define ARM_COMMON_H_

#define FORCE_INLINE inline __attribute__ ((__always_inline__))
#define PACKED __attribute__ ((__packed__))

// Common types
typedef enum {erOk, erTimeout, erError, erBusy} Error_t;

// Functional types
typedef void(*ftVoid_Void)(void);

#endif /* ARM_COMMON_H_ */
