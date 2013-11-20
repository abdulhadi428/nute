/*
 * kl_allocator.h
 *
 *  Created on: 04 сент. 2013 г.
 *      Author: kreyl
 */

#ifndef KL_ALLOCATOR_H_
#define KL_ALLOCATOR_H_

template <typename T, unsigned int Sz>
class Alloc_t {
private:
    T IArr[Sz];
    unsigned int INewIndx;
public:
    T* Allocate() {
        if(INewIndx == (Sz-1)) {
#ifdef _MSC_VER
            throw;
#endif
            return 0;   // Return NULL if unable to allocate
        }
        INewIndx++;
        return &IArr[INewIndx-1];
    }
    T* Allocate(unsigned int ALen) {
        if((INewIndx + ALen) >= Sz) {
#ifdef _MSC_VER
            throw;
#endif
            return 0;
        }
        T *p = &IArr[INewIndx];
        INewIndx += ALen;
        return p;
    }
};

#endif /* KL_ALLOCATOR_H_ */
