#include "sensors.h"
#include "uart.h"
#include "media.h"
#include "rock_action.h"

// Variables
Sns_t ESns;
SnsState_t SnsState;
IRSirc_t EIRSirc;

// Implementation

void Sns_t::Task() {
    if (!Delay.Elapsed(&Timer, 198)) return;
    //UART_StrUint("Battery: ", BatteryADC);
    // Check sensors
    SnsState.KeyTouched[0] = Touched(0);
    SnsState.KeyTouched[1] = Touched(1);
    SnsState.KeyTouched[2] = Touched(2);
    SnsState.MagnetNear    = MagnetNear();
    // Accelerometer
    //Acc.ReadAccelerations();
    // Rise event if something changed
    if (SensorsStateChanged()) EVENT_SensorsStateChanged();
}

void Sns_t::Init() {
    // Outer modules init
    Acc.Init();
    EIRSirc.Init();
    // ==== Clocks ====
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM8, ENABLE);
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1,  ENABLE);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Battery and light
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // Touch & magnet sensors
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13;
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
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Up-counter needed, nothing special
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_Period = ADC_REQUEST_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000) - 1; // Input clock divisor
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);	// Timer TRGO selection

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
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_Init(ADC1, &ADC_InitStructure);
    // Connect ADC trigger to TRGO of Timer8
    GPIO_PinRemapConfig(GPIO_Remap_ADC1_ETRGREG, ENABLE);
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

	TIM_Cmd(TIM8, ENABLE);						// TIM8 counter enable
}

bool Sns_t::SensorsStateChanged() {
    bool Changed = false;
    if (SnsState.KeyTouched[0]  != OldState.KeyTouched[0] ) Changed = true;
    if (SnsState.KeyTouched[1]  != OldState.KeyTouched[1] ) Changed = true;
    if (SnsState.KeyTouched[2]  != OldState.KeyTouched[2] ) Changed = true;
    if (SnsState.MagnetNear     != OldState.MagnetNear    ) Changed = true;
    // TODO: accelerations
    if (Changed) OldState = SnsState;
    return Changed;
}

void SnsVerbose(void) {
    if(SnsState.KeyTouched[0]) {
        UART_PrintString("Key0 touched\r");
        if (ESnd.State == sndStopped) ESnd.Play("touch0.wav");
    }
    if(SnsState.KeyTouched[1]) {
        UART_PrintString("Key1 touched\r");
        if (ESnd.State == sndStopped) ESnd.Play("touch1.wav");
    }
    if(SnsState.KeyTouched[2]) {
        UART_PrintString("Key2 touched\r");
        if (ESnd.State == sndStopped) ESnd.Play("touch2.wav");
    }
    if(SnsState.MagnetNear) {
        UART_PrintString("MagnetNear\r");
        if (ESnd.State == sndStopped) ESnd.Play("magnet.wav");
    }
    UART_NewLine();
}

// ================================ Sensors ====================================
bool Sns_t::Touched(uint8_t Indx) {
    if      (Indx==0) return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
    else if (Indx==1) return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
    else              return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
}

void Sns_t::MeasureBattery() {

}

// ================================ Infrared ===================================
void IRSirc_t::Init() {
    // ==== Clocks ====
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // ==== NVIC config ==== Enable the TIM3 global Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // ==== Timer ====
    // Time base configuration
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Up-counter needed, nothing special
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 7; // Input clock divisor
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    // Capture channels
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    // Falling edge capture
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    // Rising edge capture
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    // TIM enable counter
    TIM_Cmd(TIM3, ENABLE);
    // Enable the CC3 Interrupt Request
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
    // ==== Variables ====
    ResetPkt();
}
void IRSirc_t::Task() {
    if (!NewPacket) return;
    // Analyze packet
//    uint8_t IDamage = Pkt & 0x000F;
//    uint8_t ICmd    = (Pkt >> 4) & 0x0003;
    uint8_t IID     = (Pkt >> 6) & 0x007F;
    // Check pkt
    FieldType_t IField = ftNone;
    if      (IID == 53) IField = ftHP;
    else if (IID == 54) IField = ftHM;
    else if (IID == 55) IField = ftEP;
    else if (IID == 56) IField = ftEM;
    else if (IID == 57) IField = ftCP;
    else if (IID == 58) IField = ftCM;
    ResetPkt();
    ERock.Charge(IField);
}

// Capture complete IRQ handler
void IRSirc_t::IRQHandler() {
    uint16_t tc3 = TIM_GetCapture3(TIM3);
    uint16_t tc4 = TIM_GetCapture4(TIM3);
    uint16_t PulseWidth = (tc4 >= tc3)? (tc4-tc3) : ((0xFFFF - tc3) + tc4);
    //UART_PrintUint(PulseWidth);
    //UART_NewLine();
    // Check what we received
    if (IsHeader(PulseWidth)) { // New packet begins
        ResetPkt();
        BitCounter = 1;
    }
    // Receive bits only if header is received, and no overflow
    else if ((BitCounter >= 1) && (BitCounter < (IRS_PKT_LENGTH + 1))) {
        Pkt <<= 1;                      // Shift packet
        if (IsOne(PulseWidth)) Pkt++;
        else if (!IsZero(PulseWidth)) { // if not one and not zero
            ResetPkt();
            return;
        }
        BitCounter++;
        NewPacket = (BitCounter == (IRS_PKT_LENGTH + 1));
    }
}
// ==== Timer3 IRQ ====
void TIM3_IRQHandler(void) {
    if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
        EIRSirc.IRQHandler();
    }
}
