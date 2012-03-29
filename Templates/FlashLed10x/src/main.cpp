/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "kl_lib.h"

int main(void) {
    klPin_t Led;
    Led.Init(GPIOC, 9, GPIO_Mode_Out_PP);


    while (1) {

    } // while 1
}
