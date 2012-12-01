/*
 * GPIO_config.h
 *
 *  Created on: 26.11.2012
 *      Author: Stefan
 */

#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* Управление драйвером 485 */
#define DRIVER_485_EN_PORT          GPIOA
#define DRIVER_485_EN_CLK           RCC_APB2Periph_GPIOA
#define DRIVER_485_EN_PIN           GPIO_Pin_8
void Driver485TxEnable(void)  ;
void Driver485TxDisable(void)  ;

/*  светодиод */
#define LED_PORT          GPIOD
#define LED_CLK           RCC_APB2Periph_GPIOD
#define LED_PIN           GPIO_Pin_2
void LedOn	(void);
void LedOff	(void);
bool LedIsOn(void);

void SimpleGPIO_Init (void);


#endif /* GPIO_CONFIG_H_ */
