/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "kl_lib_f0.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_dac.h"
#include "main.h"

class klNfc_t {
private:
    void DACInit();
public:
    void Init();

} klNfc;


void Init();

int main(void) {
    Init();
    Uart.Printf("klNFC\r");
    PinSetupOut(GPIOC, 8, poPushPull, pudNone);
    PinSetupOut(GPIOC, 9, poPushPull, pudNone);
    PinSet(GPIOC, 8);

    uint32_t Tmr;
    //uint16_t v=0;
    while(1) {
        if(Delay.Elapsed(&Tmr, 999)) {
            PinToggle(GPIOC, 8);
            //Uart.Printf("hojo\r");
//            DAC_SetChannel1Data(DAC_Align_12b_R, v++);
//            if(v>= 4000) v=0;
        }
//        if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET) {
//            PinToggle(GPIOC, 9);
//            /* Get ADC1 converted data */
//            //AdcV = ADC_GetConversionValue(ADC1);
//            //Uart.Printf("%u\r", AdcV);
//        }
    }
}

void Init() {
    Delay.Init();
    Uart.Init(115200);
    klNfc.Init();
}

// ================================= klNFC =====================================
void klNfc_t::Init() {
    DACInit();
    // Gpio
    PinSetupAnalog(GPIOA, 1, pudNone);
//    PinSetupAlterFunc(GPIOA, 11, poPushPull, pudNone, AF2);
    // ==== Timer ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_DeInit(TIM1);
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef        TIM_OCInitStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_OCStructInit(&TIM_OCInitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 0xFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Output Compare PWM Mode configuration */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* low edge by default */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0x01;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    // ==== ADC ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit(ADC1);
    ADC_InitTypeDef ADC_InitStructure;
    /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Convert the ADC1 Channel 1 with 239.5 Cycles as sampling time */
    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);

    /* Enable End Of Conversion interupt */
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    /* Configure and enable ADC1 interrupt */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_COMP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ADC Calibration */
    //ADC_GetCalibrationFactor(ADC1);

    /* Enable ADCperipheral[PerIdx] */
    ADC_Cmd(ADC1, ENABLE);

    /* Wait the ADCEN falg */
    //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));

    /* ADC1 regular Software Start Conv */
    ADC_StartOfConversion(ADC1);
}

void klNfc_t::DACInit() {
    PinSetupAnalog(GPIOA, 4, pudNone);  // PA4 is DAC1 out

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    DAC_InitTypeDef DAC_InitStructure;
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1, ENABLE);
}

void ADC1_COMP_IRQHandler() {
    if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
        PinToggle(GPIOC, 9);
        uint32_t x0 = ADC_GetConversionValue(ADC1);
        DAC_SetChannel1Data(DAC_Align_12b_R, x0);
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}
