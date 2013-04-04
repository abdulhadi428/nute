/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32F4xx/hal_lld.h
 * @brief   STM32F4xx/STM32F2xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          - STM32_VDD (as hundredths of Volt).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32F2XX for High-performance STM32 F-2 devices.
 *          - STM32F4XX for High-performance STM32 F-4 devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "stm32.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS TRUE

/**
 * @name    Platform identification
 * @{
 */

#define PLATFORM_NAME           "STM32F2xx High Performance"
/** @} */

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    STM32F4xx capabilities
 * @{
 */
/* ADC attributes.*/
#define STM32_HAS_ADC1          TRUE
#define STM32_ADC1_DMA_MSK      (STM32_DMA_STREAM_ID_MSK(2, 0) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 4))
#define STM32_ADC1_DMA_CHN      0x00000000

#define STM32_HAS_ADC2          TRUE
#define STM32_ADC2_DMA_MSK      (STM32_DMA_STREAM_ID_MSK(2, 2) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 3))
#define STM32_ADC2_DMA_CHN      0x00001100

#define STM32_HAS_ADC3          TRUE
#define STM32_ADC3_DMA_MSK      (STM32_DMA_STREAM_ID_MSK(2, 0) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 1))
#define STM32_ADC3_DMA_CHN      0x00000022

/* CAN attributes.*/
#define STM32_HAS_CAN1          TRUE
#define STM32_HAS_CAN2          TRUE
#define STM32_CAN_MAX_FILTERS   28

/* DAC attributes.*/
#define STM32_HAS_DAC           TRUE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA      TRUE
#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          TRUE

/* ETH attributes.*/
#define STM32_HAS_ETH           TRUE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS 23

/* GPIO attributes.*/
#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         TRUE
#define STM32_HAS_GPIOG         TRUE
#define STM32_HAS_GPIOH         TRUE
#define STM32_HAS_GPIOI         TRUE

/* I2C attributes.*/
#define STM32_HAS_I2C1          TRUE
#define STM32_I2C1_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 0) |            \
                                 STM32_DMA_STREAM_ID_MSK(1, 5))
#define STM32_I2C1_RX_DMA_CHN   0x00100001
#define STM32_I2C1_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 7)) |           \
                                 (STM32_DMA_STREAM_ID_MSK(1, 6))
#define STM32_I2C1_TX_DMA_CHN   0x11000000

#define STM32_HAS_I2C2          TRUE
#define STM32_I2C2_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 2) |            \
                                 STM32_DMA_STREAM_ID_MSK(1, 3))
#define STM32_I2C2_RX_DMA_CHN   0x00007700
#define STM32_I2C2_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 7))
#define STM32_I2C2_TX_DMA_CHN   0x70000000

#define STM32_HAS_I2C3          TRUE
#define STM32_I2C3_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_I2C3_RX_DMA_CHN   0x00000300
#define STM32_I2C3_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_I2C3_TX_DMA_CHN   0x00030000

/* RTC attributes.*/
#define STM32_HAS_RTC           TRUE
#if defined(STM32F4XX) || defined(__DOXYGEN__)
#define STM32_RTC_HAS_SUBSECONDS TRUE
#else
#define STM32_RTC_HAS_SUBSECONDS FALSE
#endif
#define STM32_RTC_IS_CALENDAR   TRUE

/* SDIO attributes.*/
#define STM32_HAS_SDIO          TRUE
#define STM32_SDC_SDIO_DMA_MSK  (STM32_DMA_STREAM_ID_MSK(2, 3) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 6))
#define STM32_SDC_SDIO_DMA_CHN  0x04004000

/* SPI attributes.*/
#define STM32_HAS_SPI1          TRUE
#define STM32_SPI1_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(2, 0) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 2))
#define STM32_SPI1_RX_DMA_CHN   0x00000303
#define STM32_SPI1_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(2, 3) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 5))
#define STM32_SPI1_TX_DMA_CHN   0x00303000

#define STM32_HAS_SPI2          TRUE
#define STM32_SPI2_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 3))
#define STM32_SPI2_RX_DMA_CHN   0x00000000
#define STM32_SPI2_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_SPI2_TX_DMA_CHN   0x00000000

#define STM32_HAS_SPI3          TRUE
#define STM32_SPI3_RX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 0) |            \
                                 STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_SPI3_RX_DMA_CHN   0x00000000
#define STM32_SPI3_TX_DMA_MSK   (STM32_DMA_STREAM_ID_MSK(1, 5) |            \
                                 STM32_DMA_STREAM_ID_MSK(1, 7))
#define STM32_SPI3_TX_DMA_CHN   0x00000000

/* TIM attributes.*/
#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          TRUE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          TRUE
#define STM32_HAS_TIM9          TRUE
#define STM32_HAS_TIM10         TRUE
#define STM32_HAS_TIM11         TRUE
#define STM32_HAS_TIM12         TRUE
#define STM32_HAS_TIM13         TRUE
#define STM32_HAS_TIM14         TRUE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE

/* USART attributes.*/
#define STM32_HAS_USART1        TRUE
#define STM32_USART1_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(2, 2) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 5))
#define STM32_USART1_RX_DMA_CHN 0x00400400
#define STM32_USART1_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(2, 7))
#define STM32_USART1_TX_DMA_CHN 0x40000000

#define STM32_HAS_USART2        TRUE
#define STM32_USART2_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 5))
#define STM32_USART2_RX_DMA_CHN 0x00400000
#define STM32_USART2_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 6))
#define STM32_USART2_TX_DMA_CHN 0x04000000

#define STM32_HAS_USART3        TRUE
#define STM32_USART3_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 1))
#define STM32_USART3_RX_DMA_CHN 0x00000040
#define STM32_USART3_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(1, 3) |            \
                                 STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_USART3_TX_DMA_CHN 0x00074000

#define STM32_HAS_UART4         TRUE
#define STM32_UART4_RX_DMA_MSK  (STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_UART4_RX_DMA_CHN  0x00000400
#define STM32_UART4_TX_DMA_MSK  (STM32_DMA_STREAM_ID_MSK(1, 4))
#define STM32_UART4_TX_DMA_CHN  0x00040000

#define STM32_HAS_UART5         TRUE
#define STM32_UART5_RX_DMA_MSK  (STM32_DMA_STREAM_ID_MSK(1, 0))
#define STM32_UART5_RX_DMA_CHN  0x00000004
#define STM32_UART5_TX_DMA_MSK  (STM32_DMA_STREAM_ID_MSK(1, 7))
#define STM32_UART5_TX_DMA_CHN  0x40000000

#define STM32_HAS_USART6        TRUE
#define STM32_USART6_RX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(2, 1) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 2))
#define STM32_USART6_RX_DMA_CHN 0x00000550
#define STM32_USART6_TX_DMA_MSK (STM32_DMA_STREAM_ID_MSK(2, 6) |            \
                                 STM32_DMA_STREAM_ID_MSK(2, 7))
#define STM32_USART6_TX_DMA_CHN 0x55000000

/* USB attributes.*/
#define STM32_HAS_USB           FALSE
#define STM32_HAS_OTG1          TRUE
#define STM32_HAS_OTG2          TRUE
/** @} */

/*===========================================================================*/
/* Platform specific friendly IRQ names.                                     */
/*===========================================================================*/

/**
 * @name    IRQ VECTOR names
 * @{
 */
#define WWDG_IRQHandler         Vector40    /**< Window Watchdog.           */
#define PVD_IRQHandler          Vector44    /**< PVD through EXTI Line
                                                 detect.                    */
#define TAMP_STAMP_IRQHandler   Vector48    /**< Tamper and TimeStamp
                                                 through EXTI Line.         */
#define RTC_WKUP_IRQHandler     Vector4C    /**< RTC wakeup EXTI Line.      */
#define FLASH_IRQHandler        Vector50    /**< Flash.                     */
#define RCC_IRQHandler          Vector54    /**< RCC.                       */
#define EXTI0_IRQHandler        Vector58    /**< EXTI Line 0.               */
#define EXTI1_IRQHandler        Vector5C    /**< EXTI Line 1.               */
#define EXTI2_IRQHandler        Vector60    /**< EXTI Line 2.               */
#define EXTI3_IRQHandler        Vector64    /**< EXTI Line 3.               */
#define EXTI4_IRQHandler        Vector68    /**< EXTI Line 4.               */
#define DMA1_Stream0_IRQHandler Vector6C    /**< DMA1 Stream 0.             */
#define DMA1_Stream1_IRQHandler Vector70    /**< DMA1 Stream 1.             */
#define DMA1_Stream2_IRQHandler Vector74    /**< DMA1 Stream 2.             */
#define DMA1_Stream3_IRQHandler Vector78    /**< DMA1 Stream 3.             */
#define DMA1_Stream4_IRQHandler Vector7C    /**< DMA1 Stream 4.             */
#define DMA1_Stream5_IRQHandler Vector80    /**< DMA1 Stream 5.             */
#define DMA1_Stream6_IRQHandler Vector84    /**< DMA1 Stream 6.             */
#define ADC1_2_3_IRQHandler     Vector88    /**< ADC1, ADC2 and ADC3.       */
#define CAN1_TX_IRQHandler      Vector8C    /**< CAN1 TX.                   */
#define CAN1_RX0_IRQHandler     Vector90    /**< CAN1 RX0.                  */
#define CAN1_RX1_IRQHandler     Vector94    /**< CAN1 RX1.                  */
#define CAN1_SCE_IRQHandler     Vector98    /**< CAN1 SCE.                  */
#define EXTI9_5_IRQHandler      Vector9C    /**< EXTI Line 9..5.            */
#define TIM1_BRK_IRQHandler     VectorA0    /**< TIM1 Break.                */
#define TIM1_UP_IRQHandler      VectorA4    /**< TIM1 Update.               */
#define TIM1_TRG_COM_IRQHandler VectorA8    /**< TIM1 Trigger and
                                                 Commutation.               */
#define TIM1_CC_IRQHandler      VectorAC    /**< TIM1 Capture Compare.      */
#define TIM2_IRQHandler         VectorB0    /**< TIM2.                      */
#define TIM3_IRQHandler         VectorB4    /**< TIM3.                      */
#define TIM4_IRQHandler         VectorB8    /**< TIM4.                      */
#define I2C1_EV_IRQHandler      VectorBC    /**< I2C1 Event.                */
#define I2C1_ER_IRQHandler      VectorC0    /**< I2C1 Error.                */
#define I2C2_EV_IRQHandler      VectorC4    /**< I2C2 Event.                */
#define I2C2_ER_IRQHandler      VectorC8    /**< I2C1 Error.                */
#define SPI1_IRQHandler         VectorCC    /**< SPI1.                      */
#define SPI2_IRQHandler         VectorD0    /**< SPI2.                      */
#define USART1_IRQHandler       VectorD4    /**< USART1.                    */
#define USART2_IRQHandler       VectorD8    /**< USART2.                    */
#define USART3_IRQHandler       VectorDC    /**< USART3.                    */
#define EXTI15_10_IRQHandler    VectorE0    /**< EXTI Line 15..10.          */
#define RTC_Alarm_IRQHandler    VectorE4    /**< RTC alarms (A and B)
                                                 through EXTI line.         */
#define OTG_FS_WKUP_IRQHandler  VectorE8    /**< USB OTG FS Wakeup through
                                                 EXTI line.                 */
#define TIM8_BRK_IRQHandler     VectorEC    /**< TIM8 Break.                */
#define TIM8_UP_IRQHandler      VectorF0    /**< TIM8 Update.               */
#define TIM8_TRG_COM_IRQHandler VectorF4    /**< TIM8 Trigger and
                                                 Commutation.               */
#define TIM8_CC_IRQHandler      VectorF8    /**< TIM8 Capture Compare.      */
#define DMA1_Stream7_IRQHandler VectorFC    /**< DMA1 Stream 7.             */
#define FSMC_IRQHandler         Vector100   /**< FSMC.                      */
#define SDIO_IRQHandler         Vector104   /**< SDIO.                      */
#define TIM5_IRQHandler         Vector108   /**< TIM5.                      */
#define SPI3_IRQHandler         Vector10C   /**< SPI3.                      */
#define UART4_IRQHandler        Vector110   /**< UART4.                     */
#define UART5_IRQHandler        Vector114   /**< UART5.                     */
#define TIM6_IRQHandler         Vector118   /**< TIM6.                      */
#define TIM7_IRQHandler         Vector11C   /**< TIM7.                      */
#define DMA2_Stream0_IRQHandler Vector120   /**< DMA2 Stream0.              */
#define DMA2_Stream1_IRQHandler Vector124   /**< DMA2 Stream1.              */
#define DMA2_Stream2_IRQHandler Vector128   /**< DMA2 Stream2.              */
#define DMA2_Stream3_IRQHandler Vector12C   /**< DMA2 Stream3.              */
#define DMA2_Stream4_IRQHandler Vector130   /**< DMA2 Stream4.              */
#define ETH_IRQHandler          Vector134   /**< Ethernet.                  */
#define ETH_WKUP_IRQHandler     Vector138   /**< Ethernet Wakeup through
                                                 EXTI line.                 */
#define CAN2_TX_IRQHandler      Vector13C   /**< CAN2 TX.                   */
#define CAN2_RX0_IRQHandler     Vector140   /**< CAN2 RX0.                  */
#define CAN2_RX1_IRQHandler     Vector144   /**< CAN2 RX1.                  */
#define CAN2_SCE_IRQHandler     Vector148   /**< CAN2 SCE.                  */
#define OTG_FS_IRQHandler       Vector14C   /**< USB OTG FS.                */
#define DMA2_Stream5_IRQHandler Vector150   /**< DMA2 Stream5.              */
#define DMA2_Stream6_IRQHandler Vector154   /**< DMA2 Stream6.              */
#define DMA2_Stream7_IRQHandler Vector158   /**< DMA2 Stream7.              */
#define USART6_IRQHandler       Vector15C   /**< USART6.                    */
#define I2C3_EV_IRQHandler      Vector160   /**< I2C3 Event.                */
#define I2C3_ER_IRQHandler      Vector164   /**< I2C3 Error.                */
#define OTG_HS_EP1_OUT_IRQHandler Vector168 /**< USB OTG HS End Point 1 Out.*/
#define OTG_HS_EP1_IN_IRQHandler Vector16C  /**< USB OTG HS End Point 1 In. */
#define OTG_HS_WKUP_IRQHandler  Vector170   /**< USB OTG HS Wakeup through
                                                 EXTI line.                 */
#define OTG_HS_IRQHandler       Vector174   /**< USB OTG HS.                */
#define DCMI_IRQHandler         Vector178   /**< DCMI.                      */
#define CRYP_IRQHandler         Vector17C   /**< CRYP.                      */
#define HASH_RNG_IRQHandler     Vector180   /**< Hash and Rng.              */
#if defined(STM32F4XX) || defined(__DOXYGEN__)
#define FPU_IRQHandler          Vector184   /**< Floating Point Unit.       */
#endif
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* There are differences in vector names in the various sub-families,
   normalizing.*/
#define TIM1_BRK_IRQn       TIM1_BRK_TIM9_IRQn
#define TIM1_UP_IRQn        TIM1_UP_TIM10_IRQn
#define TIM1_TRG_COM_IRQn   TIM1_TRG_COM_TIM11_IRQn
#define TIM8_BRK_IRQn       TIM8_BRK_TIM12_IRQn
#define TIM8_UP_IRQn        TIM8_UP_TIM13_IRQn
#define TIM8_TRG_COM_IRQn   TIM8_TRG_COM_TIM14_IRQn

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing a system clock frequency.
 */
typedef uint32_t halclock_t;

/**
 * @brief   Type of the realtime free counter value.
 */
typedef uint32_t halrtcnt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the current value of the system free running counter.
 * @note    This service is implemented by returning the content of the
 *          DWT_CYCCNT register.
 *
 * @return              The value of the system free running counter of
 *                      type halrtcnt_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_value()         DWT_CYCCNT

/**
 * @brief   Realtime counter frequency.
 * @note    The DWT_CYCCNT register is incremented directly by the system
 *          clock so this function returns STM32_HCLK.
 *
 * @return              The realtime counter frequency of type halclock_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_frequency()     STM32_HCLK

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* STM32 ISR, DMA and RCC helpers.*/
#include "stm32_isr.h"
#include "stm32_dma.h"
#include "stm32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
