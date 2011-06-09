#include "vs.h"

VS_t Vs;

void VS_t::Init() {
    // ******** Hardware init section *******
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // Configure VS_XCS, VS_XDCS, VS_RST as Push-Pull output
    GPIO_InitStructure.GPIO_Pin  = VS_XCS | VS_XDCS | VS_RST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    this->Rst_Lo();
    this->XCS_Hi();
    this->XDCS_Hi();
    // Configure VS_DREQ as floating input
    GPIO_InitStructure.GPIO_Pin  = VS_DREQ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure MOSI & SCK as Alternate Function Push Pull
    GPIO_InitStructure.GPIO_Pin  = CC_SCLK | CC_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(CC_GPIO, &GPIO_InitStructure);
    // Configure MISO as Input Floating
    GPIO_InitStructure.GPIO_Pin  = CC_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(CC_GPIO, &GPIO_InitStructure);

}
