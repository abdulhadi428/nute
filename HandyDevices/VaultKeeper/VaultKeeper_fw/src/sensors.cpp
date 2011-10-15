/*
 * water_sensor.cpp
 *
 *  Created on: 14.10.2011
 *      Author: Kreyl
 */

#include "sensors.h"
#include "uart.h"

WaterSns_t WaterSensor;

void WaterSns_t::Init() {
    // ==== Clocks ====
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    // ==== NVIC ====  Configure and enable ADC interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // ==== ADC ====
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // Independent, not dual
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    // ADC1 regular channels configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
    // Enable ADC2 EOC interrupt
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));
    // Start ADC1 calibration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
}

void WaterSns_t::Task() {
    if(Delay.Elapsed(&SearchTimer, WATER_SEARCH_PERIOD)) {
        // Search for water
        UART_PrintString("StC\r");
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    }
}

// ============================= Interrupt ====================================
void ADC1_2_IRQHandler(void) {
    WaterSensor.ADCValue = ADC_GetConversionValue(ADC1);
    UART_StrUint("EOC: ", WaterSensor.ADCValue);

}
