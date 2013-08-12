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
    {-62, 486, 72, -19, 489, 48, -2, 505, 0, -31, 412, 240, -5, 505, -4, -14, 497, -3, 0, 0, 0},  //  sourceAard
    {-73, 489, 51, 29, -53, 480, 97, -66, 461, -20, 457, 174, 82, -15, 478, -5, 482, 110, 0, 0, 0},  //  force
    {-46, 490, 54, -2, 485, 75, 18, 482, 116, -15, 434, 204, 134, -388, 228, 174, -268, 277, 0, 0, 0},  //  execAardIgni
    {24, -372, 313, 419, 217, 90, 480, -54, -33, 462, 95, 132, 491, -38, -62, 487, -31, -62, 0, 0, 0},  //  sourceGelio
    {-186, 416, 185, -35, 484, 52, 15, -174, 451, -103, 436, 170, 25, 481, 136, 15, 494, 21, 0, 0, 0},  //  execGelio
    {190, 428, 109, 49, 466, 134, 42, 483, 86, 171, 180, -414, 12, 484, 111, -50, 483, 71, 0, 0, 0},  //  sourceIgni
    {-129, -439, 83, 15, -449, 171, 32, 6, 485, -58, -153, 455, 25, 21, 492, -20, -22, 485, 0, 0, 0},  //  sourceIrden
    {-227, 403, 149, -55, -123, 469, 3, 497, 64, -88, 425, 203, -6, 502, 32, -16, 495, 22, 0, 0, 0},  //  execIrdenKven
    {-171, 23, 446, 48, -473, -50, 51, -484, -17, -41, -44, 486, 70, -477, 15, 75, -468, 86, 0, 0, 0},  //  sourceKven
    {22, -465, 76, 93, -489, -35, 83, -491, -4, 46, -465, 143, 80, -483, 17, 79, -490, -6, 0, 0, 0},  //  reset
    {-176, -10, 455, -32, 18, 504, -63, -7, 487, 76, 48, 472, 16, 47, 482, 32, 11, 475, 0, 0, 0},  //  fullInit
};

const int16_t hyperRadius[] = {
    370,  //  sourceAard
    427,  //  force
    571,  //  execAardIgni
    422,  //  sourceGelio
    402,  //  execGelio
    451,  //  sourceIgni
    451,  //  sourceIrden
    397,  //  execIrdenKven
    462,  //  sourceKven
    330,  //  reset
    331   //  fullInit
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
