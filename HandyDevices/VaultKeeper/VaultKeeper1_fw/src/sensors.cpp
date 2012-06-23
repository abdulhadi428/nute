/*
 * sensors.cpp
 *
 *  Created on: 22.06.2012
 *      Author: kreyl
 */

#include "sensors.h"
#include "kl_time.h"
#include "stm32f10x_adc.h"
#include "comline.h"

SnsDataBuf_t SnsBuf;
Sensors_t Sensors;

void Sensors_t::Init() {
    // External power sensor
    klGpioSetupByN(GPIOB, 12, GPIO_Mode_IN_FLOATING);

    NewProblemOccured = false;
    IProblemsChanged = false;
    AdcInit();
    ChIndx = ADC_CH_COUNT; // dummy for first time
    StartNextMeasure();
}

void Sensors_t::Task() {
    // ==== Measurement unit ====
    if (MeasureIsCompleted()) {
        Delay.ms(99); // DEBUG
        // Calculate average value
        uint32_t FValue=0;
        for(uint32_t i=0; i<ADC_AVERAGE_COUNT; i++) FValue += ADCValues[i];
        FValue /= ADC_AVERAGE_COUNT;
        // Calculate battery value
        if (ChToMeasure[ChIndx] == SnsBattery) {
            if(!IsExtPwrOk()) { // Pwr problem
                if (CurrentData.Battery == 0) {
                    IProblemsChanged = true; // Report now
                    Com.Printf("No ext pwr\r");
                }
                FValue *= 3.0263158;
                CurrentData.Battery = FValue;
            }
            else // Pwr is ok
                if (!IProblemsChanged) CurrentData.Battery = 0; // Reset only after event handled
        } // if battery
        else {
            SnsState_t ss;
            if      (FValue < 504)  ss = ssShort;
            else if (FValue < 1602) ss = ssWater;
            else if (FValue > 3906) ss = ssOpen;
            else ss = ssOk;
            switch (ss) {
                case ssOk:    Com.Printf("%u Ok\r",  ChIndx); break;
                case ssOpen:  Com.Printf("%u Open\r",  ChIndx); break;
                case ssShort: Com.Printf("%u Short\r", ChIndx); break;
                case ssWater: Com.Printf("%u Water\r", ChIndx); break;
                default: break;
            }
        } // if not battery
        StartNextMeasure();
    }

    // ==== Reporting unit ====
    static uint8_t FLastHour = 27;  // dummy for first time
    static uint32_t FTmr;

    if (IProblemsChanged and Delay.Elapsed(&FTmr, 54000)) { // Restrict write rate
        IProblemsChanged = false;
        WriteMeasurements();
        NewProblemOccured = true;
    }
    else if (Time.GetHour() != FLastHour) {
        FLastHour = Time.GetHour();
        WriteMeasurements();
    } // if hour passed
}

void Sensors_t::WriteMeasurements() {
    // Update time, other values are updated online
    Time.GetDateTime(&CurrentData.DateTime);
    SnsBuf.Write(&CurrentData);
}

// ================================ Leakage ====================================
void Sensors_t::AdcInit() {
    // ==== Clk ====
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // ==== GPIOs ====
    // Inputs
    klGpioSetupByMsk(GPIOA, (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7), GPIO_Mode_AIN);   // SNS 1, 2, 3
    klGpioSetupByMsk(GPIOC, (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5), GPIO_Mode_AIN);   // SNS 4, 5, 6
    klGpioSetupByMsk(GPIOB, GPIO_Pin_0, GPIO_Mode_AIN); // Battery
    // Outputs
    klGpioSetupByMsk(GPIOB, (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11), GPIO_Mode_Out_PP);   // PWR of SNS 1,2,3,4,5
    klGpioSetupByMsk(GPIOC, (GPIO_Pin_6 | GPIO_Pin_7), GPIO_Mode_Out_PP);   // PWR of SNS 6
    // ==== ADC ====
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // Independent, not dual
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
}

void Sensors_t::StartNextMeasure() {
    ADC_DMACmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
    // Calculate channel to measure
    if (++ChIndx >= ADC_CH_COUNT) ChIndx = 0;
    // ==== ADC's DMA ====
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
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // ==== Channel ====
    switch (ChToMeasure[ChIndx]) {
        case Sns1A: klGpioSetByN(GPIOB, 2);  ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  1, ADC_SampleTime_71Cycles5); break;
        case Sns1B: klGpioSetByN(GPIOB, 3);  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  1, ADC_SampleTime_71Cycles5); break;
        case Sns2A: klGpioSetByN(GPIOB, 4);  ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  1, ADC_SampleTime_71Cycles5); break;
        case Sns2B: klGpioSetByN(GPIOB, 5);  ADC_RegularChannelConfig(ADC1, ADC_Channel_5,  1, ADC_SampleTime_71Cycles5); break;
        case Sns3A: klGpioSetByN(GPIOB, 6);  ADC_RegularChannelConfig(ADC1, ADC_Channel_6,  1, ADC_SampleTime_71Cycles5); break;
        case Sns3B: klGpioSetByN(GPIOB, 7);  ADC_RegularChannelConfig(ADC1, ADC_Channel_7,  1, ADC_SampleTime_71Cycles5); break;
        case Sns4A: klGpioSetByN(GPIOB, 8);  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5); break;
        case Sns4B: klGpioSetByN(GPIOB, 9);  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_71Cycles5); break;
        case Sns5A: klGpioSetByN(GPIOB, 10); ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_71Cycles5); break;
        case Sns5B: klGpioSetByN(GPIOB, 11); ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_71Cycles5); break;
        case Sns6A: klGpioSetByN(GPIOC, 6);  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_71Cycles5); break;
        case Sns6B: klGpioSetByN(GPIOC, 7);  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_71Cycles5); break;
        case SnsBattery:                     ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  1, ADC_SampleTime_71Cycles5); break;
    }
    ADC_Cmd(ADC1, ENABLE);                      // Enable ADC1
    // Calibrate ADC
    ADC_ResetCalibration(ADC1);                 // Enable ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1)); // Check the end of ADC1 reset calibration register
    ADC_StartCalibration(ADC1);                 // Start ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));      // Check the end of ADC1 calibration
    ADC_DMACmd(ADC1, ENABLE); // Enable ADC1 DMA
    // Start ADC1 Software Conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// ================================= Water =====================================
void Sensors_t::UpdateWaterValue(void) {
    uint32_t V = ReadWaterValue();
    V++;
    WriteWaterValue(V);
    CurrentData.WaterValue = V;
}

uint32_t Sensors_t::ReadWaterValue(void) {
    uint32_t Lo = BKP_ReadBackupRegister(BKPREG_WATER_LO);
    uint32_t Hi = BKP_ReadBackupRegister(BKPREG_WATER_HI);
    Hi <<= 16;
    return (uint32_t)(Hi + Lo);
}
void Sensors_t::WriteWaterValue(uint32_t AValue) {
    uint32_t Hi = AValue >> 16;
    uint32_t Lo = AValue & 0x0000FFFF;
    BKP_WriteBackupRegister(BKPREG_WATER_LO, (uint16_t)Lo);
    BKP_WriteBackupRegister(BKPREG_WATER_HI, (uint16_t)Hi);
}
