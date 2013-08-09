/*
 * signs.h
 *
 *  Created on: 06.03.2013
 *      Author: kreyl
 */

#ifndef SIGNS_H_
#define SIGNS_H_

#include "kl_lib_f100.h"

#define ACC_VECTOR_SZ   21

// Sign sequences
#define AARD_SOURCE_DONE 0
#define AARD_FORCE_DONE 1
#define AARD_EXEC_DONE 102

#define IGNI_SOURCE_DONE 5
#define IGNI_FORCE_DONE 501
#define IGNI_EXEC_DONE 50102

#define KVEN_SOURCE_DONE 8
#define KVEN_FORCE_DONE 801
#define KVEN_EXEC_DONE 80107

#define IRDEN_SOURCE_DONE 6
#define IRDEN_FORCE_DONE 601
#define IRDEN_EXEC_DONE 60107

#define GELIO_SOURCE_DONE 3
#define GELIO_FORCE_DONE 301
#define GELIO_EXEC_DONE 30104

#define RESET_DONE 9

// Data

const int16_t Subsign[][ACC_VECTOR_SZ] = {
    {-1, 492, 102, -34, 500, -4, -13, 504, 25, 26, 475, 100, -13, 503, -23, -48, 496, -12, 0, 0, 0},  //  sourceAard
    {-62, 493, 76, 35, 73, 492, 27, 60, 494, 43, 472, 105, 38, 104, 483, -49, 482, 123, 0, 0, 0},  //  force
    {-61, 491, 72, -42, 480, 131, -27, 446, 217, 21, 472, 76, 123, -471, 79, 204, -425, 79, 0, 0, 0},  //  execAardIgni
    {10, -395, 278, 429, 202, 126, 481, -68, 32, 475, 44, 104, 492, -53, -42, 483, -59, -34, 0, 0, 0},  //  sourceGelio
    {-228, 420, 135, -99, 484, 42, -57, -121, 463, -40, 486, -59, 22, 457, 194, 16, 486, 106, 0, 0, 0},  //  execGelio
    {107, 455, 148, 41, 481, 131, 31, 468, 160, 105, 144, -459, 64, 482, 129, -4, 486, 113, 0, 0, 0},  //  sourceIgni
    {-40, -481, 128, 9, -413, 256, -11, -34, 492, -14, -136, 475, -12, -9, 496, -32, -70, 489, 0, 0, 0},  //  sourceIrden
    {-254, 421, 67, -102, -95, 483, -7, 497, 77, -64, 486, 30, -19, 503, 34, -57, 487, 32, 0, 0, 0},  //  execIrdenKven
    {-157, -41, 462, 25, -499, 33, 9, -500, 11, 9, -50, 496, 31, -499, 43, 52, -495, 84, 0, 0, 0},  //  sourceKven
    {22, -465, 76, 93, -489, -35, 83, -491, -4, 46, -465, 143, 80, -483, 17, 79, -490, -6, 0, 0, 0},  //  reset
    {-176, -10, 455, -32, 18, 504, -63, -7, 487, 76, 48, 472, 16, 47, 482, 32, 11, 475, 0, 0, 0},  //  fullInit
};

const int16_t hyperRadius[] = {
    300,  ///255,  //  sourceAard
    400,  ///343,  //  force
    364,  //  execAardIgni
    452,  //  sourceGelio
    353,  //  execGelio
    379,  //  sourceIgni
    333,  //  sourceIrden
    331,  //  execIrdenKven
    300,  ///220,  //  sourceKven
    404,  //  reset
    572   //  fullInit
};


//0 - bolshoy
//1 - ukazatelnyy
//2 - sredniy
//3 - ladon'
//4 - bezymyannyy
//5 - mizinec
//6 - zapyast'e

const int16_t SubsignMask[][ACC_VECTOR_SZ] = {
        {1,1,1,4,1,1,0}, //0--
        {1,1,1,1,1,3,0}, //1-
        {1,1,1,1,1,1,0}, //2-----
        {1,1,1,1,1,1,0}, //3--
        {1,4,4,1,4,1,0}, //4
        {1,1,1,2,1,1,0}, //5--
        {1,1,1,1,1,1,0}, //6--
        {1,1,1,1,1,1,0}, //7
        {2,2,2,2,2,2,0}, //8--
        {1,1,1,1,1,1,0} //9
};

#define SUBSIGN_CNT countof(Subsign)

#endif /* SIGNS_H_ */
