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
            CurrentDelay = 81;
            break;

        case lmFadeAllAwayAndStop:
            CurrentDelay = 45;
            break;

        default:
            CurrentDelay = 10;
            break;
    } // switch
    Delay.Bypass(&Timer, IDelay1);
}
void Leds_t::EqualAll(uint8_t AValue) {
    SetAll(AValue);
    i2cMgr.AddCmd(i2cCmd);
    OutputEnable();
    IMode = lmEqualAll;
}
void Leds_t::SetRunning(uint16_t ADelay, uint8_t ALedCount, Color_t AColor) {
    CurrentDelay = ADelay;
    IColor = AColor;
    LedID = 0;
    IMode = lmRunning;
    Delay.Bypass(&Timer, IDelay1);
}
void Leds_t::SetBlinkAll(uint16_t OnTime, uint16_t OffTime, Color_t AColor) {
    IDelay1 = OnTime;
    IDelay2 = OffTime;
    CurrentDelay = IDelay2;
    IColor = AColor;
    IMode = lmBlinkAll;
    LedID = 0;
    Delay.Bypass(&Timer, IDelay2);
}

// ================================ Task =======================================
void Leds_t::Task() {
    if (!Delay.Elapsed(&Timer, CurrentDelay)) return;
    uint8_t b, i;
    switch (IMode) {
        case lmFadeInAll:
            // Increase PWM
            b = FPkt.PWM[0];
            b++;
            SetAll(b);
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmRunning:
            FPkt.Colors[LedID] = {0, 0, 0}; // Hide previous LED
            if (++LedID == 5) LedID = 0;
            FPkt.Colors[LedID] = IColor;    // Shine next LED
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmRunningRGB:
            FPkt.Colors[LedID] = {0, 0, 0}; // Hide previous LED
            // Switch to next LED
            if (++LedID == 5) {
                LedID = 0;
                // Switch color
                if      (IColor.Red   == COLOR_MAX) { IColor.Red   = 0; IColor.Green = COLOR_MAX; }
                else if (IColor.Green == COLOR_MAX) { IColor.Green = 0; IColor.Blue  = COLOR_MAX; }
                else                                { IColor.Blue  = 0; IColor.Red   = COLOR_MAX; }
            }
            FPkt.Colors[LedID] = IColor;
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmFadeAllAwayAndStop:
            b = 0;
            // Iterate all LEDs
            for (i=0; i<15; i++) {
                if (FPkt.PWM[i] != 0) {
                    b = 1;
                    FPkt.PWM[i]--;
                }
            } // for
            if (b == 0) IMode = lmEqualAll;   // None is shining
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmBlinkAll:
            if (LedID == 0) {   // Was off, switch on
                for (uint8_t i=0; i<5; i++) FPkt.Colors[i] = IColor;
                CurrentDelay = IDelay1; // ON time
            }
            else {
                for (uint8_t i=0; i<5; i++) FPkt.Colors[i] = {0, 0, 0};
                CurrentDelay = IDelay2; // OFF time
            }
            LedID = !LedID;
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
