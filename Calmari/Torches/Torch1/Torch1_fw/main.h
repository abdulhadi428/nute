/**
  ******************************************************************************
  * @file    Example01_STM32L15x_2k\inc\main.h
  * @author  MCD Application Team
  * @version V0.1.1
  * @date    20-April-2011
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#ifdef USE_STM32L152_EVAL
#include "stm32l152_eval_lcd.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TSL_Timer_Init(void);
void TSL_Timer_Check_1sec_Tick(void);
void TSL_Timer_Check_100ms_Tick(void);
void TSL_Timer_Check_10ms_Tick(void);
void TSL_Timer_ISR(void);
void TSL_Timer_Adjust(uint32_t Delay);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
