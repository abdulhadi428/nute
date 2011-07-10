#include "sensors.h"
#include "uart.h"
#include "media.h"
#include "rock_action.h"
#include "rock_tables.h"

// Variables
Sns_t ESns;
SnsState_t SnsState;
IRSirc_t EIRSirc;

// Implementation

void Sns_t::Task() {
    if (!Enabled) return;
    if (!Delay.Elapsed(&Timer, 306)) return;
    // Check sensors
    SnsState.KeyTouched[0] = Touched(0);
    SnsState.KeyTouched[1] = Touched(1);
    SnsState.KeyTouched[2] = Touched(2);
    SnsState.MagnetNear    = MagnetNear();
    // Raise event if something changed
    if (SensorsStateChanged()) EVENT_SensorsStateChanged();
}

void Sns_t::Init() {
    // ==== Clocks ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Touch sensors
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // magnet sensor
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // Sensors power
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    PowerOn();
	// ==== Variables ====
	Enabled = true;
}

bool Sns_t::SensorsStateChanged() {
    bool Changed = false;
    if (SnsState.KeyTouched[0]  != OldState.KeyTouched[0] ) Changed = true;
    if (SnsState.KeyTouched[1]  != OldState.KeyTouched[1] ) Changed = true;
    if (SnsState.KeyTouched[2]  != OldState.KeyTouched[2] ) Changed = true;
    if (SnsState.MagnetNear     != OldState.MagnetNear    ) Changed = true;
    if (Changed) OldState = SnsState;
    return Changed;
}

// ================================ Sensors ====================================
bool Sns_t::Touched(uint8_t Indx) {
    if      (Indx==0) return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
    else if (Indx==1) return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
    else              return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // ==== Timer ====
    // Time base configuration: pulse per uS
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Up-counter needed, nothing special
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000000) - 1; // Input clock divisor
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
    if (!ESns.Enabled) return;
    if (!NewPacket) return;
    //UART_StrHex16("Pkt: ", Pkt);
    // Analyze packet
//    uint8_t IDamage = Pkt & 0x000F;
//    uint8_t ICmd    = (Pkt >> 4) & 0x0003;
    uint8_t IID     = (Pkt >> 6) & 0x007F;
    //UART_StrHex16("ID: ", IID);
    // Check pkt
    FieldType_t IField = ftNone;
    if      (IID == fidHP) IField = ftHP;
    else if (IID == fidHM) IField = ftHM;
    else if (IID == fidEP) IField = ftEP;
    else if (IID == fidEM) IField = ftEM;
    else if (IID == fidCP) IField = ftCP;
    else if (IID == fidCM) IField = ftCM;
    ResetPkt();
    ERock.Charge(IField);
}

// Capture complete IRQ handler
void IRSirc_t::IRQHandler() {
    uint16_t tc3 = TIM_GetCapture3(TIM3);
    uint16_t tc4 = TIM_GetCapture4(TIM3);
    uint16_t PulseWidth = (tc4 >= tc3)? (tc4-tc3) : ((0xFFFF - tc3) + tc4);
//    UART_PrintUint(PulseWidth);
//    UART_NewLine();
    // Check what we received
    if (IsHeader(PulseWidth)) { // New packet begins
//        UART_PrintUint(PulseWidth);
//        UART_NewLine();
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
        if (ESns.Enabled) EIRSirc.IRQHandler();
    }
}
