/*
 * main.cpp
 *
 *  Created on: 27.08.2011
 *      Author: Kreyl
 */

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

#include "delay_util.h"
#include "cc2500.h"
#include "uart.h"

void GeneralInit(void);

int main(void) {
    GeneralInit();

    while (1) {

    }
}

void GeneralInit(void) {
    Delay.Init();
    Delay.ms(63);
    UART_Init();
    klPrintf("\rTransmitter\r");

}
