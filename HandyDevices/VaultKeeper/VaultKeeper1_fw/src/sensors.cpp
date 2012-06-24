/*
 * sensors.cpp
 *
 *  Created on: 22.06.2012
 *      Author: kreyl
 */

#include "sensors.h"
#include "kl_time.h"
#include "comline.h"

SnsDataBuf_t SnsBuf;
Sensors_t Sensors;

void Sensors_t::Init() {
    // External power sensor: no ADC, only 1-0
    klGpioSetupByN(GPIOB, 12, GPIO_Mode_IN_FLOATING);

    NewProblemOccured = false;
    IProblemsChanged = false;
    AdcGpioInit();
    AdcInit();
    WaterInit();
    ChIndx = 0;
    StartNextMeasure();
}

#define WRITE_MIN_DELAY     54000   // ms
#define EVENT_MIN_DELAY     4005    // ms
void Sensors_t::Task() {
    static uint8_t FLastHour = 27;  // dummy for first time
    static uint32_t FTmr;
    // ==== Measurement unit ====
    if (MeasureIsCompleted()) {
        //Delay.ms(99); // DEBUG
        // Calculate average value
        uint32_t FValue=0;
        for(uint32_t i=0; i<ADC_AVERAGE_COUNT; i++) FValue += ADCValues[i];
        FValue /= ADC_AVERAGE_COUNT;
        //Com.Printf("Ch %u = %u\r", ChIndx, FValue);
        // Process channels
        if (SnsChnl[ChIndx].Name == SnsBattery) {
            if(!IsExtPwrOk()) { // Pwr problem
                if (CurrentData.Battery == 0) {
                    IProblemsChanged = true; // Report now
                    Delay.Bypass(&FTmr, (WRITE_MIN_DELAY-EVENT_MIN_DELAY)); // Report after 4000 mS
                    Com.Printf("No ext pwr\r");
                }
                FValue *= 3.0263158;
                CurrentData.Battery = FValue;
            }
            else // Pwr is ok
                if (!IProblemsChanged) CurrentData.Battery = 0; // Reset only after event handled
        } // if battery
        else {
            //if(SnsChnl[ChIndx].Name == Sns3A) Com.Printf("Sns3A %u\r", FValue);
            //if(ChToMeasure[ChIndx] == Sns4B) Com.Printf("Sns4B %u\r", FValue);
            SnsState_t ssNow, ssOld = ssOk;
            if      (FValue < 477)  ssNow = ssShort;
            else if (FValue < 1305) ssNow = ssWater;
            else if (FValue > 3501) ssNow = ssOpen;
            else ssNow = ssOk;

            // ==== Check if change occured ====
            uint8_t CurrentState;
            uint8_t SnsIndx = ChIndx >> 1;
            CurrentState = CurrentData.SnsArr[SnsIndx];
            //Com.Printf("Sns%u Old: %X\r", SnsIndx, CurrentState);
            switch (SnsChnl[ChIndx].Name) {
                case Sns1A:
                case Sns2A:
                case Sns3A:
                case Sns4A:
                case Sns5A:
                case Sns6A:
                    ssOld = (SnsState_t)(CurrentState >> 4);    // A at hi 4 bits
                    // Write new value
                    CurrentState &= 0x0F;   // Erase top bits
                    CurrentState += ((uint8_t)ssNow) << 4;
                    break;

                case Sns1B:
                case Sns2B:
                case Sns3B:
                case Sns4B:
                case Sns5B:
                case Sns6B:
                    ssOld = (SnsState_t)(CurrentState & 0x0F);    // B at lo 4 bits
                    // Write new value
                    CurrentState &= 0xF0;   // Erase Lo bits
                    CurrentState += ((uint8_t)ssNow);
                    break;
                default: break;
            }
            if((ssOld == ssOk) and (ssNow != ssOk)) {       // Problem occured
                IProblemsChanged = true;
                Delay.Bypass(&FTmr, (WRITE_MIN_DELAY-EVENT_MIN_DELAY)); // Report after 4000 mS
                Com.Printf("Sns%u New: %X\r", SnsIndx, CurrentState);
                CurrentData.SnsArr[SnsIndx] = CurrentState; // Write new problem
            }
            else if((ssOld != ssOk) and (ssNow == ssOk)) {  // Problem vanished
                // Write ok only if was processed
                if (!IProblemsChanged) CurrentData.SnsArr[SnsIndx] = CurrentState;
            }
            // Display
//            switch (ss) {
//                case ssOk:    Com.Printf("%u Ok\r",  ChIndx); break;
//                case ssOpen:  Com.Printf("%u Open\r",  ChIndx); break;
//                case ssShort: Com.Printf("%u Short\r", ChIndx); break;
//                case ssWater: Com.Printf("%u Water\r", ChIndx); break;
//                default: break;
//            }
        } // if not battery
        StartNextMeasure();
    }

    // ==== Reporting unit ====
    if (IProblemsChanged and Delay.Elapsed(&FTmr, WRITE_MIN_DELAY)) { // Restrict write rate
        Com.Printf("New problem\r");
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

// ================================== ADC ======================================
void Sensors_t::AdcGpioInit() {
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) {
        klGpioSetupByMsk(SnsChnl[i].PInPort,  SnsChnl[i].InMask,  GPIO_Mode_AIN);     // Input
        if(SnsChnl[i].POutPort != 0)
            klGpioSetupByMsk(SnsChnl[i].POutPort, SnsChnl[i].OutMask, GPIO_Mode_Out_PP);  // Output
    }
}

void Sensors_t::AdcInit() {
    // ==== Clk ====
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
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
    // Switch off pull-up
    if(SnsChnl[ChIndx].POutPort != 0)
        klGpioClearByMsk(SnsChnl[ChIndx].POutPort, SnsChnl[ChIndx].OutMask);
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
    if(SnsChnl[ChIndx].POutPort != 0)
        klGpioSetByMsk(SnsChnl[ChIndx].POutPort, SnsChnl[ChIndx].OutMask);  // Switch on pull-up
    ADC_RegularChannelConfig(ADC1, SnsChnl[ChIndx].AdcChannel, 1, ADC_SampleTime_71Cycles5);
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
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
void Sensors_t::WaterInit() {
    // GPIO
    klGpioSetupByN(GPIOC, 6,  GPIO_Mode_Out_PP);
    klGpioSetByN(GPIOC, 6);                             // switch on our pull-up
    klGpioSetupByN(GPIOC, 4, GPIO_Mode_IN_FLOATING);    // input

}

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
