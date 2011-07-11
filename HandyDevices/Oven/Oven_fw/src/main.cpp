/*
 * File:   main.cpp
 * Author: Kreyl
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "stm32f10x.h"
#include "delay_util.h"
#include "uart.h"
#include "lcd110x.h"
#include "images.h"
#include "adc.h"

// =============================== Prototypes ==================================
void GeneralInit(void);

#define HEATER_PERIOD   306 // ms
class Heater_t {
private:
    uint32_t Timer;
    void On(void)  { GPIOB->BSRR = GPIO_Pin_8; }
    void Off(void) { GPIOB->BRR  = GPIO_Pin_8; }
public:
    uint16_t NeededADC;
    void Task(void);
    void Init(void);
} Heater;


// ============================= Implementation ================================
int main(void) {
    UART_Init();
    Delay.ms(100);
    UART_PrintString("Can offer only oven\r");

    GeneralInit();
    Lcd.DrawImage(0,0, ImageLogo, NotInverted);

    uint32_t FTimr;
    while (!Delay.Elapsed(&FTimr, 999));

    Heater.NeededADC = 4000;
    // ==== Main cycle ====
    while(1) {
//        if(Delay.Elapsed(&FTimr, 306)) {
//            UART_StrUint("ADC: ", Adc.AverageValues());
//        }
        Heater.Task();
    } // while(1)
    return 0;
}

void GeneralInit(void) {
    // Disable JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // Configure two bits for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    Delay.Init();
    Lcd.Init();
    Adc.Init();
    Heater.Init();

    // ==== Backlight ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIOB->BSRR = GPIO_Pin_0;   // Switch backlight on
}

// ================================ Heater =====================================
void Heater_t::Task() {
    if (!Delay.Elapsed(&Timer, HEATER_PERIOD)) return;
    // Switch heater on or off depending on current temperature
    uint16_t AdcValue = Adc.AverageValues();
    UART_StrUint("ADC: ", AdcValue);
    if (AdcValue > NeededADC) this->Off();
    else this->On();
}

void Heater_t::Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

