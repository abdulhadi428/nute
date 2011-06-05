#include "leds_pca.h"

#include "uart.h"
#include "delay_util.h"

Leds_t Leds;

void Leds_t::Init() {
    Uart.PrintString("PCA Init\r");
    // Init OE pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = LED_OE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    this->OutputDisable();           // TODO: check if needed to init
    //this->OutputEnable();
    // Setup i2cCmd
    i2cCmd.Address = LED_I2C_ADDR;
    i2cCmd.DataToRead.Length = 0;   // Nothing to read
    i2cCmd.DataToWrite.Buf = (uint8_t *)&FPkt;
    i2cCmd.DataToWrite.Length = LEDS_PKT_SIZE;
    // Prepare initialization packet
    FPkt.ControlReg = 0b10000000;   // Autoincrement enable, first register is 0x00
    FPkt.Mode1 = 0b10000000;    // Non-sleep mode, do not respond to extra adresses
    //FPkt.Mode2 = 0b00000010;    // Group control = dimming, output not inverted, change on STOP, Open-drain, high-impedance when OE=1
    FPkt.Mode2 = 0b00000110;    // Group control = dimming, output not inverted, change on STOP, Push-Pull, high-impedance when OE=1
    for (uint8_t i=0; i<=15; i++) FPkt.PWM[i] = i<<3;   // DEBUG
    FPkt.GrpPWM = 0xFF;         // Max common brightness
    FPkt.GrpFreq = 0;           // Don't care as DBLINK bit is 0
    FPkt.LEDOut[0] = 0xFF;      // LED driver has individual brightness and group dimming/blinking and can be controlled through its PWMx register and the GRPPWM registers
    FPkt.LEDOut[1] = 0xFF;
    FPkt.LEDOut[2] = 0xFF;
    FPkt.LEDOut[3] = 0xFF;
    // ==== Init device ====
    while (1) {
        i2cMgr.WriteBufferNoDMA(LED_I2C_ADDR, (uint8_t *)&FPkt, LEDS_PKT_SIZE);
        Delay.ms(250);
    }

}
