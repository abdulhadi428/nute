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
uint8_t ChIndx;

void Sensors_t::Init() {
    // External power sensor: no ADC, only 1-0
    klGpioSetupByN(GPIOB, 12, GPIO_Mode_IN_FLOATING);
    // Variables etc.
    NewProblemOccured = false;
    IProblemsChanged = false;
    AdcGpioInit();
    AdcInit();
    WaterInit();
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) Chnl[i].Init();
    //Com.Printf("ADC_CH_COUNT = %u\r", ADC_CH_COUNT);
    ChIndx = ADC_CH_COUNT-1;    // To start from 0
    IPrepareToNextMeasure();
}

void Sensors_t::Task() {
    static uint8_t FLastHour = 27;  // dummy for first time
    static uint32_t FProblemTmr;
    // ==== Measurement unit ====
    if (MeasureIsCompleted()) {
        // Calculate average value
        uint32_t FValue=0;
        for(uint32_t i=0; i<ADC_AVERAGE_COUNT; i++) FValue += ADCValues[i];
        FValue /= ADC_AVERAGE_COUNT;
        //Com.Printf("Ch %u = %u\r", ChIndx, FValue);
        // === Process channels ===
        SnsChnl_t *PCh = &Chnl[ChIndx];
        if (SnsChnlParams[ChIndx].Name == SnsBattery) {
            if(!IsExtPwrOk()) {             // Check if power failure
                FValue *= 3.0263158;
                PCh->Value = FValue;
                if(PCh->State == ssOk) {    // Just occured
                    PCh->State = ssFail;
                    PCh->ProblemIsNew = true;
                    Delay.Bypass(&PCh->Timer, NEW_PROBLEM_TIMEOUT); // Report now
                }
            }
            else {
                PCh->Value = 0;
                PCh->State = ssOk;
                PCh->ProblemIsNew = false;
            }
        } // if battery
        else PCh->ProcessNewValue(FValue);
        IPrepareToNextMeasure();
    }

    if(!MeasureStarted and Delay.Elapsed(&IMeasureTmr, PULLUP_DEADTIME)) StartNextMeasure();

    // ==== Check if problem ====
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) {
        if(Chnl[i].NewProblemOccured()) IProblemsChanged = true;
    }

    // ==== Reporting unit ====
    if (IProblemsChanged and Delay.Elapsed(&FProblemTmr, WRITE_MIN_DELAY)) { // Restrict write rate
        Com.Printf("New problem\r");
        IProblemsChanged = false;
        WriteMeasurements();
        NewProblemOccured = true;   // Signal for reporting unit
    }
    else if (Time.GetHour() != FLastHour) {
        FLastHour = Time.GetHour();
        WriteMeasurements();
    } // if hour passed
}

void Sensors_t::WriteMeasurements() {
    RowData_t RData;
    // Update time
    Time.GetDateTime(&RData.DateTime);
    // Update sensors
    uint8_t SnsIndx, CurrentState;
    SnsChName_t FName;
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) {
        if(SnsChnlParams[i].Name == SnsBattery) RData.Battery = Chnl[i].Value;
        else {
            SnsIndx = i >> 1;
            CurrentState = RData.SnsArr[SnsIndx];
            FName = SnsChnlParams[i].Name;
            if ((FName==Sns1A) or (FName==Sns2A) or (FName==Sns3A) or (FName==Sns4A) or (FName==Sns5A) or (FName==Sns6A)) {
                CurrentState &= 0x0F;   // Erase top bits
                CurrentState += ((uint8_t)Chnl[i].State) << 4;
            }
            else {
                CurrentState &= 0xF0;   // Erase Lo bits
                CurrentState += ((uint8_t)Chnl[i].State);
            }
            RData.SnsArr[SnsIndx] = CurrentState;
            Chnl[i].ProblemIsNew = false;   // do not report again if channel timeout is currently ticking
        } // if not battery
    } // for
    // Water
    RData.WaterValue = ReadWaterValue();
    // Write data to log
    SnsBuf.Write(&RData);
}

// =============================== Channel =====================================
void SnsChnl_t::ProcessNewValue(uint32_t AValue) {
    //Com.Printf("Sns%u %u\r", IIndx, AValue);
    //if(SnsChnlParams[ChIndx].Name == Sns3A) Com.Printf("Sns3A %u\r", AValue);
    SnsState_t ssNow;
    if      (AValue < SHORT_ADC_VALUE) ssNow = ssShort;
    else if (AValue < WATER_ADC_VALUE) ssNow = ssWater;
    else if (AValue > OPEN_ADC_VALUE)  ssNow = ssOpen;
    else ssNow = ssOk;

    if ((ssNow != ssOk) and (State == ssOk)) {     // Problem appeared
        ProblemIsNew = true;
        Com.Printf("Sns%u: NewProb %u\r", ChIndx, ssNow);
        Delay.Reset(&Timer);                       // Change state only if time passed
    }
    else if((ssNow == ssOk) and (State != ssOk)) { // Problem disappeared
        ProblemIsNew = false;
        Com.Printf("Sns%u: No Prob\r", ChIndx);
    }
    State = ssNow;
}

bool SnsChnl_t::NewProblemOccured() {
    if ((State != ssOk) and ProblemIsNew and Delay.Elapsed(&Timer, NEW_PROBLEM_TIMEOUT)) {
        ProblemIsNew = false;
        return true;
    }
    else return false;
}

// ================================== ADC ======================================
void Sensors_t::AdcGpioInit() {
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) {
        klGpioSetupByMsk(SnsChnlParams[i].PInPort,  SnsChnlParams[i].InMask,  GPIO_Mode_AIN);     // Input
        if(SnsChnlParams[i].POutPort != 0)
            klGpioSetupByMsk(SnsChnlParams[i].POutPort, SnsChnlParams[i].OutMask, GPIO_Mode_Out_PP);  // Output
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

void Sensors_t::IPrepareToNextMeasure() {
    // Switch off DMA
    ADC_DMACmd(ADC1, DISABLE);
    DMA_DeInit(DMA1_Channel1);
    // Switch off pull-up
    if(SnsChnlParams[ChIndx].POutPort != 0)
        klGpioClearByMsk(SnsChnlParams[ChIndx].POutPort, SnsChnlParams[ChIndx].OutMask);
    // Calculate channel to measure
    if (++ChIndx >= ADC_CH_COUNT) ChIndx = 0;
    //Com.Printf("ch = %u\r", ChIndx);
    // Switch on pull-up
    if(SnsChnlParams[ChIndx].POutPort != 0)
        klGpioSetByMsk(SnsChnlParams[ChIndx].POutPort, SnsChnlParams[ChIndx].OutMask);
    // Reset deadtime delay
    Delay.Reset(&IMeasureTmr);
    MeasureStarted = false;
}

void Sensors_t::StartNextMeasure() {
    MeasureStarted = true;
    ADC_Cmd(ADC1, DISABLE);
    // ==== ADC's DMA ====
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
    ADC_RegularChannelConfig(ADC1, SnsChnlParams[ChIndx].AdcChannel, 1, ADC_SampleTime_71Cycles5);
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
    klGpioSetupByMsk(GPIOC, GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_Out_PP);
    klGpioClearByMsk(GPIOC, GPIO_Pin_5 | GPIO_Pin_7);
    WaterPin.Init(GPIOC, 4, GPIO_Mode_IN_FLOATING);
    WaterPin.IrqSetup(EXTI_Trigger_Falling);
    WaterPin.IrqEnable();
}

void Sensors_t::UpdateWaterValue(void) {
    uint32_t V = ReadWaterValue();
    V++;
    WriteWaterValue(V);
    Com.Printf("W: %u\r", V);
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

void EXTI4_IRQHandler(void) {
    static uint32_t FTmr;
    if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
        if(Delay.Elapsed(&FTmr, MIN_PULSE_LENGTH)) {
            Sensors.UpdateWaterValue();
        }
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
