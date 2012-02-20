/*
 * kl_gpio.cpp
 *
 *  Created on: 18.02.2012
 *      Author: kreyl
 */

#include "kl_gpio.h"

klPin_t::klPin_t(GPIO_TypeDef *PGpioPort, uint16_t APinNumber, GPIOMode_TypeDef AMode) {
    IPinNumber = APinNumber;
    IPinMask = (uint16_t)(1<<APinNumber);
    IGPIO = PGpioPort;

    int IClock = 0;
    if (PGpioPort == GPIOA) IClock = RCC_APB2Periph_GPIOA;
    else if (PGpioPort == GPIOB) IClock = RCC_APB2Periph_GPIOB;
    // Clock
    if ((AMode == GPIO_Mode_AF_OD) || (AMode == GPIO_Mode_AF_PP)) IClock |= RCC_APB2Periph_AFIO;
    RCC_APB2PeriphClockCmd(IClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = IPinMask;
    GPIO_InitStructure.GPIO_Mode = AMode;
    GPIO_Init(IGPIO, &GPIO_InitStructure);
}

// ======== IRQ pin =========
void klPinIrq_t::IrqSetup(EXTITrigger_TypeDef ATriggerType) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // Get IRQ channel
    if      ((IPinNumber >= 0)  && (IPinNumber <= 4))   IChannel = IPinNumber;
    else if ((IPinNumber >= 5)  && (IPinNumber <= 9))   IChannel = EXTI9_5_IRQn;
    else if ((IPinNumber >= 10) && (IPinNumber <= 15))  IChannel = EXTI15_10_IRQn;
    // EXTI line config
    if (IGPIO == GPIOA) GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, IPinNumber);
    else if (IGPIO == GPIOB) GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, IPinNumber);
    // Configure EXTI3 line
    EXTI_InitTypeDef   EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = IPinMask;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = ATriggerType;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void klPinIrq_t::IrqEnable() {
    // Enable and set EXTI Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = IChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void klPinIrq_t::IrqDisable() {
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}
