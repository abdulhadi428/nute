/*
 * File:   common.h
 * Author: Kreyl
 *
 * Created on June 9, 2011, 11:11 PM
 */

#ifndef COMMON_H
#define	COMMON_H

struct Buf8_t {
    uint8_t *Buf;
    uint8_t Length;
    uint8_t CurrentItem;
};
typedef Buf8_t* Buf8_p;

#endif	/* COMMON_H */

