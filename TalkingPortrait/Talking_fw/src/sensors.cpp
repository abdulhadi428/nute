#include "sensors.h"
#include "uart.h"

// Variables
Sns_t Sensor;

// Implementation
void Sns_t::Task() {
    if (!Delay.Elapsed(&Timer, 306)) return;
    // Check sensor
    if (Detected() && !WasDetected) {
        WasDetected = true;
        if(EvtDetected != 0) EvtDetected();
    }
    else if(!Detected() && WasDetected) {
        WasDetected = false;
    }
}

void Sns_t::Init() {
    // ==== Clocks ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    // ==== GPIO ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
