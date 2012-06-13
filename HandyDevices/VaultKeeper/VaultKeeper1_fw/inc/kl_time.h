/*
 * kl_time.h
 *
 *  Created on: 13.06.2012
 *      Author: kreyl
 */

#ifndef KL_TIME_H_
#define KL_TIME_H_

#include <stdint.h>
#include "kl_lib.h"

class TimeCounter_t : public Time_t {
private:

public:
    uint16_t Year;
    uint8_t Month, Day;
    void Init(void);
};

extern TimeCounter_t Time;

#endif /* KL_TIME_H_ */
