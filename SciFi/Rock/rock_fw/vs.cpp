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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure VS_SI & VS_XCLK as Alternate Function Push Pull
    GPIO_InitStructure.GPIO_Pin  = VS_XCLK | VS_SI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure MISO as Input Floating
    GPIO_InitStructure.GPIO_Pin  = VS_SO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ==== SPI init ====    MSB first, master, SCK idle low
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}

void VS_t::Enable() {

}
void VS_t::Disable() {
    AmplifierOff();
    Rst_Lo();
}

uint8_t VS_t::BusyWait(void) {
    uint32_t Timeout = VS_TIMEOUT;
    while (IsBusy()) {
        Timeout--;
        if (Timeout == 0) return VS_TIMEOUT;
    }
    return VS_OK;
}

// ==== Amplifier ====
void VS_t::AmplifierOn() {

}
void VS_t::AmplifierOff() {

}

// ==== Commands ====
uint8_t VS_t::CmdRead(uint8_t AAddr, uint16_t* AData) {
    uint8_t IReply;
    // Wait until ready
    if (IReply = BusyWait()) return IReply; // Get out in case of timeout
    // Start communication
    XCS_Lo();
    WriteByte(VS_READ_OPCODE);
}

// ==== Low-level SPI transactions
void VS_t::WriteByte(uint8_t AByte) {

}
uint8_t VS_t::ReadByte() {

}


