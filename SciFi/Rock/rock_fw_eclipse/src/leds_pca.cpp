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
    Mode = lmEqualAll;
    LedID = 0;
    // Map colors on PWM channels
    Colors[0] = (Color_t*)(&FPkt.PWM[9]);
    Colors[1] = (Color_t*)(&FPkt.PWM[12]);
    Colors[2] = (Color_t*)(&FPkt.PWM[6]);
    Colors[3] = (Color_t*)(&FPkt.PWM[0]);
    Colors[4] = (Color_t*)(&FPkt.PWM[3]);

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
void Leds_t::SetEqualAll(uint8_t AValue) {
    SetAll(AValue);
    i2cMgr.AddCmd(i2cCmd);
    OutputEnable();
    Mode = lmEqualAll;
}
void Leds_t::SetRunning(void) {
    Mode = lmRunning;
    LedID = 0;
    CurrentDelay = RunDelay;
    Delay.Bypass(&Timer, RunDelay);
}
void Leds_t::SetBlinkAll(void) {
    Mode = lmBlinkAll;
    LedIsOn = false;
    CurrentDelay = BlinkOffTime;
    Delay.Bypass(&Timer, CurrentDelay);
}
void Leds_t::SetRunningWithBlink() {
    OutputEnable();
    Mode = lmRunAndBlink;
    LedID = 0;
    LedIsOn = false;
    CurrentDelay = RunDelay;
    Delay.Reset(&Timer2);
    Delay.Bypass(&Timer, CurrentDelay);
}

// ================================ Task =======================================
void Leds_t::Task() {
    if (!Delay.Elapsed(&Timer, CurrentDelay)) return;
    uint8_t ILedToFade;
    switch (Mode) {
        case lmRunning:
            *Colors[LedID] = {0, 0, 0}; // Hide previous LED
            if (++LedID == 5) LedID = 0;
            *Colors[LedID] = RunColor;  // Shine next LED
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmBlinkAll:
            if (LedIsOn) {   // Was on, switch off
                for (uint8_t i=0; i<5; i++) *Colors[i] = {0, 0, 0};
                CurrentDelay = BlinkOffTime; // OFF time
            }
            else {
                for (uint8_t i=0; i<5; i++) *Colors[i] = BlinkColor;
                CurrentDelay = BlinkOnTime; // ON time
            }
            LedIsOn = !LedIsOn;
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmRunAndBlink:
            if (LedIsOn) {   // Was blinking on, check if time to switch off
                if (Delay.Elapsed(&Timer2, BlinkOnTime)) {
                    for (uint8_t i=0; i<5; i++) *Colors[i] = {0, 0, 0};
                    LedIsOn = false;
                }
            }
            else {  // LED is off
                if (Delay.Elapsed(&Timer2, BlinkOffTime) && (BlinkColor.Red || BlinkColor.Green || BlinkColor.Blue)) {
                    for (uint8_t i=0; i<5; i++) *Colors[i] = BlinkColor;
                    LedIsOn = true;
                }
                else {  // Proceed with running
                    if (++LedID == 5) LedID = 0;
                    *Colors[LedID] = RunColor;  // Shine next LED
                    // Fade previous LED
                    ILedToFade = (LedID >= RunLedCount)? (LedID - RunLedCount) : ((5+LedID) - RunLedCount);
                    *Colors[ILedToFade] = {0, 0, 0};
                }
            }
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
