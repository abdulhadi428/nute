#include "leds_pca.h"

#include "uart.h"

Leds_t Leds;

void Leds_t::Init() {
    UART_PrintString("PCA Init\r");
    // Init OE pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = LED_OE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    this->OutputDisable();
    // ==== Init variables ====
    Delay.Reset(&Timer);
    IMode = lmEqualAll;
    LedID = 0;
    // Setup i2cCmd
    i2cCmd.Address = LED_I2C_ADDR;
    i2cCmd.DataToRead.Length = 0;   // Nothing to read
    i2cCmd.DataToWrite.Buf = (uint8_t *)&FPkt;
    i2cCmd.DataToWrite.Length = LEDS_PKT_SIZE;
    i2cCmd.Callback = 0;
    // Prepare initialization packet
    FPkt.ControlReg = 0b10000000;   // Autoincrement enable, first register is 0x00
    FPkt.Mode1 = 0b10000000;    // Non-sleep mode, do not respond to extra adresses
    //FPkt.Mode2 = 0b00000010;    // Group control = dimming, output not inverted, change on STOP, Open-drain, high-impedance when OE=1
    FPkt.Mode2 = 0b00000110;    // Group control = dimming, output not inverted, change on STOP, Push-Pull, high-impedance when OE=1
    SetAll(0);                  // Set all PWM to 0
    FPkt.GrpPWM = 0xFF;         // Max common brightness
    FPkt.GrpFreq = 0;           // Don't care as DBLINK bit is 0
    FPkt.LEDOut[0] = 0xFF;      // LED driver has individual brightness and group dimming/blinking and can be controlled through its PWMx register and the GRPPWM registers
    FPkt.LEDOut[1] = 0xFF;
    FPkt.LEDOut[2] = 0xFF;
    FPkt.LEDOut[3] = 0x7F;      // LEDOut15 = 01 => fully on => 0 at output
    // ==== Init device ====
    i2cMgr.WriteBufferPoll(LED_I2C_ADDR, (uint8_t *)&FPkt, LEDS_PKT_SIZE);
}

// =============================== Light effects ===============================
void Leds_t::SetMode(LedModes_t AMode) {
    // Switch all off
    SetAll(0);
    i2cMgr.AddCmd(i2cCmd);
    OutputEnable();
    IMode = AMode;
    LedID = 0;
    // Setup depending on mode
    switch (AMode) {
        case lmRunningRGB:
            IColor.Red = COLOR_MAX;
            IColor.Green = 0;
            IColor.Blue = 0;
            lDelay = 81;
            break;

        default:
            lDelay = 10;
            break;
    } // switch

    Delay.Reset(&Timer);
}
void Leds_t::EqualAll(uint8_t AValue) {
    SetAll(AValue);
    i2cMgr.AddCmd(i2cCmd);
    OutputEnable();
    IMode = lmEqualAll;
}

// ================================ Task =======================================
void Leds_t::Task() {
    if (!Delay.Elapsed(&Timer, lDelay)) return;
    uint8_t b;
    switch (IMode) {
        case lmFadeInAll:
            // Increase PWM
            b = FPkt.PWM[0];
            b++;
            SetAll(b);
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmRunningRGB:
            // Fade current LED
            FPkt.Colors[LedID].Red   = 0;
            FPkt.Colors[LedID].Green = 0;
            FPkt.Colors[LedID].Blue  = 0;
            // Switch to next LED
            LedID++;
            if (LedID == 5) {
                LedID = 0;
                // Switch color
                if      (IColor.Red   == COLOR_MAX) { IColor.Red   = 0; IColor.Green = COLOR_MAX; }
                else if (IColor.Green == COLOR_MAX) { IColor.Green = 0; IColor.Blue  = COLOR_MAX; }
                else                                { IColor.Blue  = 0; IColor.Red   = COLOR_MAX; }
            }
            FPkt.Colors[LedID] = IColor;
            i2cMgr.AddCmd(i2cCmd);
            break;

        default: break;
    } // switch
}

// ==== LCD backlight ====
void Leds_t::BacklightOn() {
    FPkt.LEDOut[3] = 0x3F;
    i2cMgr.AddCmd(i2cCmd);
}
void Leds_t::BacklightOff() {
    FPkt.LEDOut[3] = 0x7F;
    i2cMgr.AddCmd(i2cCmd);
}