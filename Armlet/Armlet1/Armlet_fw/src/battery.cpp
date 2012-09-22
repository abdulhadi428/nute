/*
 * adc.cpp
 *
 *  Created on: 27.11.2011
 *      Author: Kreyl
 */

#include "battery.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "lcd1200.h"
#include "lcd_images.h"

Battery_t Battery;

void Battery_t::Init() {
    // Charging signals
    klGpioSetupByMsk(GPIOA, GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15, GPIO_Mode_IPU);
    // Clock
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // GPIO
    klGpioSetupByN(GPIOA, 1, GPIO_Mode_AIN);
    // ==== DMA ====
    DMA_DeInit(DMA1_Channel1);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCValues[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_AVERAGE_COUNT;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    // ==== ADC ====
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // Independent, not dual
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    // ADC1 regular channels configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5);
    ADC_DMACmd(ADC1, ENABLE);                   // Enable ADC1 DMA
    ADC_Cmd(ADC1, ENABLE);                      // Enable ADC1
    // Calibrate ADC
    ADC_ResetCalibration(ADC1);                 // Enable ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1)); // Check the end of ADC1 reset calibration register
    ADC_StartCalibration(ADC1);                 // Start ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));      // Check the end of ADC1 calibration
    // Start ADC1 Software Conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void Battery_t::Task() {
    // Measure
    if(Delay.Elapsed(&Timer, 999)) {
        if(IsCharging()) State = bsCharging;
        else {
            // Calculate average of existing values
            uint32_t IValue=0;
            for(uint32_t i=0; i<ADC_AVERAGE_COUNT; i++) IValue += ADCValues[i];
            IValue /= ADC_AVERAGE_COUNT;
            Uart.Printf("U = %u\r", IValue);
            // ==== Calculate battery state ====
            // Discharging slope
            if     ((State == bsFull)  and (IValue < BATTERY_HALF))     State = bsHalf;
            else if((State == bsHalf)  and (IValue < BATTERY_EMPTY))    State = bsEmpty;
            else if((State == bsEmpty) and (IValue < BATTERY_ZERO))     State = bsZero;
            // Charging slope
            else if((State == bsZero)  and (IValue > BATTERY_ZERO+20))  State = bsEmpty;
            else if((State == bsEmpty) and (IValue > BATTERY_EMPTY+20)) State = bsHalf;
            else if((State == bsHalf)  and (IValue > BATTERY_HALF+20))  State = bsFull;
            else if(State  == bsCharging)                               State = bsZero;
        }
    }

    // Display state
    static uint32_t BlinkTmr;
    static bool IsShown = false;
    if(Delay.Elapsed(&BlinkTmr, 540)) {
        switch (State) {
            case bsCharging: Lcd.DrawImage(80, 0, icon_BatteryCharging); break;
            case bsFull:     Lcd.DrawImage(80, 0, icon_BatteryFull);  break;
            case bsHalf:     Lcd.DrawImage(80, 0, icon_BatteryHalf);  break;
            case bsEmpty:    Lcd.DrawImage(80, 0, icon_BatteryEmpty); break;
            case bsZero:
                if(IsShown) Lcd.DrawImage(80, 0, icon_BatteryEmpty);
                else Lcd.DrawImage(80, 0, icon_BatteryNothing);
                IsShown = !IsShown;
                break;
        }
    }
}
