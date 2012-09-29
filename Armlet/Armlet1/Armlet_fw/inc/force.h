/*
 * force.h
 *
 *  Created on: 29.09.2012
 *      Author: kreyl
 */

#ifndef FORCE_H_
#define FORCE_H_

#include <inttypes.h>

class Force_t {
private:
    uint32_t JediCount;
public:
    Force_t() : JediCount(0), IsForceUser(false) {}
    bool IsForceUser;
    void DrawForce();
    void SetCount(uint32_t NewCount) { JediCount = NewCount; }
};

extern Force_t Force;

#endif /* FORCE_H_ */
