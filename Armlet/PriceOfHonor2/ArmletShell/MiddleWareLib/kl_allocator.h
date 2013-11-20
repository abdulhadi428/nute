/*
 * kl_allocator.h
 *
 *  Created on: 04 сент. 2013 г.
 *      Author: kreyl
 */

#ifndef KL_ALLOCATOR_H_
#define KL_ALLOCATOR_H_

void ApiPrintf(const char *S, unsigned int N1, unsigned int N2);

template <typename T, unsigned int Sz>
class Alloc_t {
private:
    T IArr[Sz];
    unsigned int INewIndx;
public:
    T* Allocate(const char* ATypeName) {
        if(INewIndx == Sz) {
#ifdef _MSC_VER
            throw;
#endif
            return 0;   // Return NULL if unable to allocate
        }
        INewIndx++;
        ApiPrintf(ATypeName, INewIndx, Sz);
        return &IArr[INewIndx-1];
    }

    T* Allocate(unsigned int ALen, const char* ATypeName) {
        if((INewIndx + ALen) > Sz) {
#ifdef _MSC_VER
            throw;
#endif
            return 0;
        }
        T *p = &IArr[INewIndx];
        INewIndx += ALen;
        ApiPrintf(ATypeName, INewIndx, Sz);
        return p;
    }
};

#endif /* KL_ALLOCATOR_H_ */
