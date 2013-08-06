/*
 * signs.h
 *
 *  Created on: 06.03.2013
 *      Author: kreyl
 */

#ifndef SIGNS_H_
#define SIGNS_H_

#include "kl_lib_f100.h"

// Types
struct Sign_t {

};

#define ACC_VECTOR_SZ   21

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

#define IDLE_STATUS -1

#define AARD_SOURCE_STATUS 0
#define AARD_FORCE_STATUS 1
#define AARD_EXEC_STATUS 2

#define IGNI_SOURCE_STATUS 6
#define IGNI_FORCE_STATUS 7
#define IGNI_EXEC_STATUS 8

#define KVEN_SOURCE_STATUS 12
#define KVEN_FORCE_STATUS 13
#define KVEN_EXEC_STATUS 14

#define IRDEN_SOURCE_STATUS 9
#define IRDEN_FORCE_STATUS 10
#define IRDEN_EXEC_STATUS 11

#define GELIO_SOURCE_STATUS 3
#define GELIO_FORCE_STATUS 4
#define GELIO_EXEC_STATUS 5

#define RESET_STATUS 15

// Data
/* prototype consts
const int16_t Subsign[][ACC_VECTOR_SZ] = {
        {-39,506,18,-89,469,143,-66,495,24,-20,496,39,-86,484,69,-149,475,-58,97,-283,-401},      //0 01.jpg - (source)aard
        {-21,496,97,80,-78,480,12,94,496,81,480,91,-85,31,491,-169,466,65,97,-298,-391},          //1 02.jpg (force)aard, (force)gelio, (force)igni, (force)irden, (force)kven
        {31,482,147,-37,451,205,18,495,60,41,496,-23,217,-394,171,232,-291,331,124,-330,-351},    //2 003.jpg (exec)aard, (exec)igni
        {-334,-374,20,345,268,217,480,-62,111,470,78,123,481,-43,-123,480,28,103,348,-313,-162},  //3 01.jpg (source)gelio
        {-249,417,132,-183,435,160,-38,59,501,-79,464,-157,-24,480,121,-143,480,-9,36,-488,-76},  //4 03.jpg (exec)gelio
        {-91,375,321,-43,388,302,-18,489,100,52,267,-417,17,469,157,-147,432,198,198,-323,311},   //5 01.jpg (source)igni
        {-47,-467,-177,11,-500,100,-46,68,499,-83,-127,476,-8,4,497,-144,-25,483,-95,117,-482},   //6 01.jpg (source)irden
        {-128,472,122,-12,-187,459,-10,495,81,-20,494,60,3,468,160,-135,476,71,-8,-141,-483},     //7 03.jpg (exec)irden, (exec)kven
        {-457,-115,182,56,-494,-123,47,-488,76,59,-184,461,59,-480,31,111,-461,165,-8,79,-500}    //8 01.jpg (init)kven
};
*/
/* first of a kind */
//const int16_t Subsign[][ACC_VECTOR_SZ] = {
//        {-28,495,133,-73,509,-16,-44,499,84,31,500,99,-49,513,44,-16,502,129,0,0,0},      //0 01.jpg - (source)aard
//        {-66,513,27,-87,107,495,18,68,522,24,510,46,53,90,502,74,482,168,0,0,0},          //1 02.jpg (force)aard, (force)gelio, (force)igni, (force)irden, (force)kven
//        {42,512,50,-22,506,76,34,483,150,50,507,-51,215,-441,130,209,-442,111,0,0,0},    //2 003.jpg (exec)aard, (exec)igni
//        {15,-381,332,417,256,152,499,-76,60,468,186,31,489,-101,104,499,-71,60,0,0,0},  //3 01.jpg (source)gelio
//        {-389,261,221,-98,502,65,-16,467,209,-40,498,110,73,-108,497,138,457,191,0,0,0},  //4 03.jpg (exec)gelio
//        {289,417,80,16,511,-55,24,504,-17,-40,111,-507,15,517,12,-2,520,61,0,0,0},   //5 01.jpg (source)igni
//        {25,-475,174,-33,-434,263,-68,-88,506,108,-147,473,-99,-72,499,-100,-208,441,0,0,0},   //6 01.jpg (source)irden
//        {-335,317,233,-5,-17,514,60,471,192,-40,503,96,66,503,95,55,476,194,0,0,0},     //7 03.jpg (exec)irden, (exec)kven
//        {-155,-43,484,-67,-498,102,-60,-484,63,69,-55,502,-46,-512,28,-16,-506,-70,0,0,0},    //8 01.jpg (init)kven
//        {0,-500,0,0,-500,0,0,-500,0,0,-500,0,0,-500,0,0,-500,0,0,0,0} //9 reset
//};
//modified
const int16_t Subsign[][ACC_VECTOR_SZ] = {
        {-28,495,133,-73,509,-16,-44,499,84,31,500,99,-49,513,44,-16,502,129,0,0,0},      //0 01.jpg - (source)aard
        {-93,497,77,-49,86,502,2,44,523,24,486,147,122,40,496,93,453,215,0,0,0},          //1 02.jpg (force)aard, (force)gelio, (force)igni, (force)irden, (force)kven
        {-47,498,91,-114,466,171,-94,451,215,-27,505,62,141,-352,349,91,-356,337,0,0,0},    //2 003.jpg (exec)aard, (exec)igni
        {15,-381,332,417,256,152,499,-76,60,468,186,31,489,-101,104,499,-71,60,0,0,0},  //3 01.jpg (source)gelio
        {-114,435,239,-111,488,100,16,-9,521,-41,506,43,110,409,277,130,452,205,0,0,0},  //4 03.jpg (exec)gelio
        {289,417,80,16,511,-55,24,504,-17,-40,111,-507,15,517,12,-2,520,61,0,0,0},   //5 01.jpg (source)igni
        {-204,-454,127,11,-312,407,-58,-128,497,60,-18,503,35,-116,502,27,-133,478,0,0,0},   //6 01.jpg (source)irden
        {-335,317,233,-5,-17,514,60,471,192,-40,503,96,66,503,95,55,476,194,0,0,0},     //7 03.jpg (exec)irden, (exec)kven
        {-316,-35,399,63,-454,224,44,-472,139,48,74,503,-14,-497,146,22,-480,160,0,0,0},    //8 01.jpg (init)kven
        {0,-500,0,0,-500,0,0,-500,0,0,-500,0,0,-500,0,0,-500,0,0,0,0} //9 reset
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

const Sign_t Aard = {

};


#endif /* SIGNS_H_ */
