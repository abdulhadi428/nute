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
    //Uart.Printf("klNFC\r");
    PinSetupOut(GPIOC, 8, poPushPull, pudNone);
    PinSetupOut(GPIOC, 9, poPushPull, pudNone);
    PinSet(GPIOC, 9);

  //  uint32_t Tmr;
    //uint16_t v=0;
    while(1) {
//        if(Delay.Elapsed(&Tmr, 999)) {
            //PinToggle(GPIOC, 8);
            //Uart.Printf("hojo\r");
//            DAC_SetChannel1Data(DAC_Align_12b_R, v++);
//            if(v>= 4000) v=0;
  //      }
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
    //Uart.Init(115200);
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
    TIM_TimeBaseStructure.TIM_Period = 309;
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Output Compare PWM Mode configuration */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* low edge by default */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 9;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    // ==== ADC ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit(ADC1);

//    ADC1->CR = 0;   // Disable ADC
//    ADC1->CR = ADC_CR_ADCAL;
    ADC_InitTypeDef ADC_InitStructure;
    /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_1_5Cycles);
    //ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_13_5Cycles);
  //  ADC_GetCalibrationFactor(ADC1);


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

//int32_t Sin1[ORDR] = {4071,   891, -3876, -1739, 3496};
//int32_t Cos1[ORDR] = { 448, -3998, -1323,  3708, 2135};
int32_t Sin1[] = {-4000, -4000, 4000, 4000};
int32_t Cos1[] = {-4000, 4000, 4000, -4000};
#define ORDR   countof(Sin1)
int32_t xn[ORDR];


int32_t ix1, iy1;
int32_t xv[5], yv[5];
int32_t cx1, cy1, lx1, ly1;
uint32_t RGain = 255;

void ADC1_COMP_IRQHandler() {
    if(ADC1->ISR & ADC_IT_EOC) {
        PinSet(GPIOC, 9);
        int32_t x0 = ADC1->DR;
        int32_t y0;
//        x0 <<= 4;
        y0 = RGain * x0;
        y0 >>= 8;


        // ==== Remove DC component ====
        y0 = 4095 * (x0 - cx1) + 4000 * cy1;
        y0 >>= 12;
        cy1 = y0;
        cx1 = x0;

        // ==== Detector is here ====
        if(y0<0) y0 = -y0;

        // ==== AGC ====
        //uint32_t y = (uint32_t)y0;
//        y0 *= RGain;
//        y0 >>= 8;
//        int32_t ly0 = 100*lx1 + 4060*ly1;
//        ly0 >>= 12;
//        lx1 = y0;
//        ly1 = ly0;
//        y0 = ly0;

        int32_t Err = y0 - 2000;
        if(Err < 0) {
            if(RGain < 3000) {
                RGain += 1;
                PinSet(GPIOC, 8);
            }
        }
        else {
            if(RGain > 16) {
                RGain -= 16;
                PinClear(GPIOC, 8);
            }
        }

        // ==== 4-th order Butterworth: 14kHz, fs=51780 ====
        yv[4] = 493 * (xv[0] + xv[4]) +
                1970 * (xv[1] + xv[3]) +
                2955 * xv[2]  +
                (-80 * yv[0]) +
                (-279 * yv[1]) +
                (-2122 * yv[2]) +
                (-1304 * yv[3]);
        yv[4] >>= 12;
        xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
        xv[4] = y0;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4];
        y0 = yv[4];
        if(y0<0) y0 = 0;


//        y0 += 2048;
        DAC->DHR12R1 = y0;
        ADC1->ISR = ADC_IT_EOC;
        PinClear(GPIOC, 9);
    }
}

/*
         // AGC
#define IA1     6
#define IB1     4090
        int32_t ix0 = y0;
        int32_t iy0 = IA1 * ix1 + IB1 * iy1;
        iy0 >>= 12;
        ix1 = ix0;
        iy1 = iy0;

        iy0 >>= 7;
        if(iy0 > 1000) y0 >>= 7;
        else y0 >>= 4;
//        if(y0 < 700) PinClear(GPIOC, 8);
//        else if(y0 > 1300) PinSet(GPIOC, 8);

 ////FIR filters
        int32_t f0=0;
        int32_t f1=0;
        xn[3] = xn[2];
        f0 += xn[3] * Sin1[3];
        f1 += xn[3] * Cos1[3];

        xn[2] = xn[1];
        f0 += xn[2] * Sin1[2];
        f1 += xn[2] * Cos1[2];

        xn[1] = xn[0];
        f0 += xn[1] * Sin1[1];
        f1 += xn[1] * Cos1[1];

        xn[0] = x0;
        f0 += xn[0] * Sin1[0];
        f1 += xn[0] * Cos1[0];

        f0 >>= 15;
        f1 >>= 15;
        y0 = f0*f0 + f1*f1;


 */
