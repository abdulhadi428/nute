/*
 * simple_GPIO.cpp
 *
 *  Created on: 26.11.2012
 *      Author: Stefan
 */
#include "GPIO_config.h"


void Driver485TxEnable(void)   		{GPIO_SetBits(DRIVER_485_EN_PORT,DRIVER_485_EN_PIN);}
void Driver485TxDisable(void)   		{GPIO_ResetBits(DRIVER_485_EN_PORT,DRIVER_485_EN_PIN);}
void LedOn	(void)					{GPIO_SetBits(LED_PORT,LED_PIN);}
void LedOff (void)					{GPIO_ResetBits(LED_PORT,LED_PIN);}
bool LedIsOn(void)		 { return (GPIO_ReadInputDataBit(LED_PORT, LED_PIN) == Bit_RESET); }

void SimpleGPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(
    		DRIVER_485_EN_CLK |
    		LED_CLK |
        RCC_APB2Periph_AFIO,
        ENABLE );

    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure DRIVER_485_EN as Output */
    GPIO_InitStructure.GPIO_Pin =  DRIVER_485_EN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init( DRIVER_485_EN_PORT, &GPIO_InitStructure );
    /* Configure LED as Output */
    GPIO_InitStructure.GPIO_Pin =  LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init( LED_PORT, &GPIO_InitStructure );
}
