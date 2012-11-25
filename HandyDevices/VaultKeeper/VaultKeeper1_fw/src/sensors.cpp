/*
 * sensors.cpp
 *
 *  Created on: 22.06.2012
 *      Author: kreyl
 */

#include "sensors.h"
#include "kl_time.h"
//#include "comline.h"

SnsDataBuf_t SnsBuf;
Sensors_t Sensors;
uint8_t ChIndx;

void Sensors_t::Init() {
    // External power sensor: no ADC, only 1 or 0
    klGpioSetupByN(GPIOB, 12, GPIO_Mode_IN_FLOATING);
    // Variables etc.
    NeedToReport = false;
    ISituationChanged = false;
    AdcGpioInit();
    AdcInit();
    WaterInit();
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) Chnl[i].Init();
    //DbgUART.SendPrintF("ADC_CH_COUNT = %u\r", ADC_CH_COUNT);
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
        //DbgUART.SendPrintF("Ch %u = %u\r", ChIndx, FValue);
        // === Process channels ===
        SnsChnl_t *PCh = &Chnl[ChIndx];
        if (SnsChnlParams[ChIndx].Name == SnsBattery) { // If battery
            if(!IsExtPwrOk()) {             // Power failure
                PCh->Value = FValue * 3.0263158;
                if(PCh->StateLongPerspective == ssOk) {    // Power failure just occured
                    PCh->StateLongPerspective = ssFail;
                    PCh->HasChanged = true; // Fix immediately
                    DbgUART.SendPrintF("Pwr failure; battery = %u\r", PCh->Value);
                }
            }
            else {                          // Power is ok
                PCh->Value = 0;             // indicate that power is ok
                if(PCh->StateLongPerspective != ssOk) {    // Restoration just occured
                    PCh->StateLongPerspective = ssOk;
                    PCh->HasChanged = true; // Fix immediately
                    DbgUART.SendPrintF("Pwr restored\r");
                }
            }
        } // if battery
        // Not battery
        else PCh->ProcessNewValue(FValue);
        IPrepareToNextMeasure();
    }

    // Start measure if not started and if enogh time elapsed to pull pull-up
    if(!MeasureStarted and Delay.Elapsed(&IMeasureTmr, PULLUP_DEADTIME)) StartNextMeasure();

    // ==== Check if problem ====
    if(Delay.Elapsed(&FProblemTmr, WRITE_MIN_DELAY)) {  // do not write too often
        // Iterate all, check if change occured
        for (uint8_t i=0; i<ADC_CH_COUNT; i++) {
            if(Chnl[i].HasChanged) {
                Chnl[i].HasChanged = false;
                ISituationChanged = true;
            }
        }
        // Reporting unit
        if (ISituationChanged) {
            DbgUART.SendPrintF("Need to report\r");
            WriteMeasurements();
            ISituationChanged = false;  // Changes written
            NeedToReport = true;        // Signal for reporting unit
        }
        // Write measurements hourly
        else if (Time.GetHour() != FLastHour) {
            FLastHour = Time.GetHour();
            WriteMeasurements();
            ISituationChanged = false;  // Changes written
        } // if hour passed
    }   // if WRITE_MIN_DELAY
}

void Sensors_t::WriteMeasurements() {
    RowData_t RData;
    // Update time
    Time.GetDateTime(&RData.DateTime);
    // Update sensors
    uint8_t SnsIndx, CurrentState;
    SnsChName_t FName;
    for (uint8_t i=0; i<ADC_CH_COUNT; i++) {
        FName = SnsChnlParams[i].Name;
        if(FName == SnsBattery) RData.Battery = Chnl[i].Value;
        else {
            SnsIndx = i / 2;                        // Two ADC channels per sensor
            CurrentState = RData.SnsArr[SnsIndx];   // For half of byte saving
            if ((FName==Sns1A) or (FName==Sns2A) or (FName==Sns3A) or (FName==Sns4A) or (FName==Sns5A) or (FName==Sns6A)) {
                CurrentState &= 0x0F;               // Erase top bits
                CurrentState += ((uint8_t)Chnl[i].StateLongPerspective) << 4;
            }
            else {
                CurrentState &= 0xF0;               // Erase Lo bits
                CurrentState += ((uint8_t)Chnl[i].StateLongPerspective);
            }
            RData.SnsArr[SnsIndx] = CurrentState;
        } // if not battery
    } // for
    // Water
    RData.WaterValue = ReadWaterValue();
    // Write data to log
    SnsBuf.Write(&RData);
}

// =============================== Channel =====================================
void SnsChnl_t::ProcessNewValue(uint32_t AValue) {
    //DbgUART.SendPrintF("Sns%u %u\r", IIndx, AValue);
    //if(SnsChnlParams[ChIndx].Name == Sns3A) DbgUART.SendPrintF("Sns3A %u\r", AValue);
    SnsState_t ssNow;
    if      (AValue < SHORT_ADC_VALUE) ssNow = ssShort;
    else if (AValue < WATER_ADC_VALUE) ssNow = ssWater;
    else if (AValue > OPEN_ADC_VALUE)  ssNow = ssOpen;
    else ssNow = ssOk;

    if (ssNow != StateLast) {   // State has changed since last time
        StateLast = ssNow;
        DbgUART.SendPrintF("Sns%u: %u\r", ChIndx, ssNow);
        Delay.Reset(&Timer);    // Do not take fast changes into account
    }
    else {  // State has not changed
        if(Delay.Elapsed(&Timer, NEW_PROBLEM_TIMEOUT)) {    // After timeout, check if changed in long perspective
            if(StateLongPerspective != StateLast) {
                StateLongPerspective = StateLast;
                HasChanged = true;
                DbgUART.SendPrintF("Sns%u: change detected\r", ChIndx);
            }
        }
    }
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
    if(SnsChnlParams[ChIndx].POutPort != 0) // zero pointer protection
        klGpioClearByMsk(SnsChnlParams[ChIndx].POutPort, SnsChnlParams[ChIndx].OutMask);
    // Calculate channel to measure
    if (++ChIndx >= ADC_CH_COUNT) ChIndx = 0;
    //DbgUART.SendPrintF("ch = %u\r", ChIndx);
    // Switch on pull-up
    if(SnsChnlParams[ChIndx].POutPort != 0) // zero pointer protection
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
    // Switch on water sns pull-up (SNS6A PWR)
    klGpioSetupByN(GPIOC, 6,  GPIO_Mode_Out_PP);
    klGpioSetByN(GPIOC, 6);
    // Switch off second pull-up and second input
    klGpioSetupByMsk(GPIOC, GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_Out_PP);
    klGpioClearByMsk(GPIOC, GPIO_Pin_5 | GPIO_Pin_7);
    // Switch on water sns' pin
    WaterPin.Init(GPIOC, 4, GPIO_Mode_IN_FLOATING);
    WaterPin.IrqSetup(EXTI_Trigger_Falling);
    WaterPin.IrqEnable();
}

void Sensors_t::UpdateWaterValue(void) {
    uint32_t V = ReadWaterValue();
    V++;
    WriteWaterValue(V);
    DbgUART.SendPrintF("W: %u\r", V);
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

void Sensors_t::WaterIrqHandler() {
    static uint32_t FTmr;
    static bool FCounted = false;
    bool FTimeoutOk = Delay.Elapsed(&FTmr, MIN_PULSE_LENGTH);
    if(!FCounted and FTimeoutOk) {      // Not counted, enough time passed => first fall after long open circuit, count
        UpdateWaterValue();
        FCounted = true;
        WaterPin.DisableFalling();
        WaterPin.EnableRising();
    }
    else if(FCounted and !FTimeoutOk) { // Counted, but not enough time passed => rise right after fall, ignore
        Delay.Reset(&FTmr);
    }
    else if(FCounted and FTimeoutOk) { // Counted, and enough time passed => first rise after long short circuit, reset FCounted
        FCounted = false;
        WaterPin.DisableRising();
        WaterPin.EnableFalling();
    }
    else if(!FCounted and !FTimeoutOk) { // Not counted, and not enough time passed => second fall after short
        Delay.Reset(&FTmr);
    }

}

void EXTI4_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
        Sensors.WaterIrqHandler();
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
