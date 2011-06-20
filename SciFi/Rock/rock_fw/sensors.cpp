#include "sensors.h"
#include "uart.h"

Sns_t ESns;
SnsState_t SnsState;
SnsHeat_t SnsHeat;

void Sns_t::Task() {
    if (!Delay.Elapsed(&Timer, 198)) return;
    //UART_StrUint("0: ", BatteryADC);
    //UART_StrUint("1: ", LumiADC);

    // Check sensors
    SnsState.KeyTouched[0]  = Touched(0);
    SnsState.KeyTouched[1]  = Touched(1);
    SnsState.KeyTouched[2]  = Touched(2);
    SnsState.MagnetNear     = MagnetNear();
    SnsState.VoltageApplied = VoltageApplied();
    // Accelerometer
    //Acc.ReadAccelerations();

    // Heat sensor
    //SnsHeat

    // Rise event if something changed
    if (SensorsStateChanged()) EVENT_SensorsStateChanged();
}

void Sns_t::Init() {
    // Outer modules init
    //Acc.Init();
    SnsHeat.Init();
    // ==== Clocks ====
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1,  ENABLE);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Battery and light
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // Touch, magnet, voltage sensors
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // Sensors power
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    PowerOn();
    // ==== Timer ====
    // Time base configuration
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Up-counter needed, nothing special
    TIM_TimeBaseStructure.TIM_Period = ADC_REQUEST_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000) - 1; // Input clock divisor
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);       // Timer TRGO selection
    // ==== DMA ====
    DMA_DeInit(DMA1_Channel1);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCValues[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;
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
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_Init(ADC1, &ADC_InitStructure);
    // ADC1 regular channels configuration
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);

    ADC_DiscModeChannelCountConfig(ADC1, 1);    // Regular discontinuous mode channel number configuration
    ADC_DiscModeCmd(ADC1, ENABLE);              // Enable regular discontinuous mode
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);      // Enable ADC1 external trigger conversion
    //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);                   // Enable ADC1 DMA
    ADC_Cmd(ADC1, ENABLE);                      // Enable ADC1
    // Calibrate ADC
    ADC_ResetCalibration(ADC1);                 // Enable ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1)); // Check the end of ADC1 reset calibration register
    ADC_StartCalibration(ADC1);                 // Start ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));      // Check the end of ADC1 calibration

    TIM_Cmd(TIM3, ENABLE);                      // Enable timer

    // Interrupt config: needed to workaround hardware bug
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

bool Sns_t::SensorsStateChanged() {
    bool Changed = false;
    if (SnsState.KeyTouched[0]  != OldState.KeyTouched[0] ) Changed = true;
    if (SnsState.KeyTouched[1]  != OldState.KeyTouched[1] ) Changed = true;
    if (SnsState.KeyTouched[2]  != OldState.KeyTouched[2] ) Changed = true;
    if (SnsState.MagnetNear     != OldState.MagnetNear    ) Changed = true;
    if (SnsState.VoltageApplied != OldState.VoltageApplied) Changed = true;
    // TODO: accelerations
    if (Changed) OldState = SnsState;
    return Changed;
}

void SnsVerbose(void) {
    if(SnsState.KeyTouched[0] ) UART_PrintString("Key0 touched\r");
    if(SnsState.KeyTouched[1] ) UART_PrintString("Key1 touched\r");
    if(SnsState.KeyTouched[2] ) UART_PrintString("Key2 touched\r");
    if(SnsState.MagnetNear    ) UART_PrintString("MagnetNear\r");
    if(SnsState.VoltageApplied) UART_PrintString("VoltageApplied\r");
    UART_NewLine();
}

// Sensors
bool Sns_t::Touched(uint8_t Indx) {
    if      (Indx==0) return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
    else if (Indx==1) return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
    else              return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
}

// ========================= STTS751 heat sensor ===============================
void SnsHeat_t::Init() {
//    // Prepare i2cCmd for accelerations reading
//    RegAddrToRead = ;
//    i2cCmd.Address = HEATSNS_I2C_ADDR;
//    i2cCmd.DataToWrite.Buf = &RegAddrToRead;
//    i2cCmd.DataToWrite.Length = 1;
//    i2cCmd.DataToRead.Buf = AccArray;
//    i2cCmd.DataToRead.Length = ACCELERATIONS_SIZE;
//    i2cCmd.Callback = 0;
    // ==== Setup initial registers ====
    SingleReg_t RegBuf;
    // Write configuration register
    RegBuf.RegAddr = HEATSNS_REG_CONFIG;
    RegBuf.RegValue = 0b10000000; // EVENT disabled, convert continuously, 10 bit
    i2cMgr.WriteBufferPoll(HEATSNS_I2C_ADDR, (uint8_t *)&RegBuf, I2C_SINGLEREG_SIZE);
}