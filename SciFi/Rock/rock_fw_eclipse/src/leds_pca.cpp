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
    Delay.Reset(&Timer1);
    Mode = lmEqualAll;
    BlinkOnTime  = 10;
    BlinkOffTime = 10;
    RunDelay = 10;
    RunLedCount = 1;
    BlinkColor = {0, 0, 0};
    RunColor = {0, 0, 0};

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
void Leds_t::SetEqualAll(Color_t AColor) {
    for (uint8_t i=0; i<5; i++) *Colors[i] = AColor;
    i2cMgr.AddCmd(i2cCmd);
    OutputEnable();
    Mode = lmEqualAll;
}
void Leds_t::SetRunning(void) {
    Mode = lmRunning;
    LedID = 0;
    Delay.Bypass(&Timer1, RunDelay);
}
void Leds_t::SetBlinkAll(void) {
    Mode = lmBlinkAll;
    LedIsOn = false;
    Delay.Bypass(&Timer1, BlinkOffTime);
}
void Leds_t::SetRunningWithBlink() {
    OutputEnable();
    Mode = lmRunAndBlink;
    LedID = 0;
    LedIsOn = false;
    Delay.Reset(&Timer2);
}

// ================================ Task =======================================
void Leds_t::Task() {
    uint8_t ILedToFade;
    switch (Mode) {
        case lmRunning:
            if (!Delay.Elapsed(&Timer1, RunDelay)) return;
            *Colors[LedID] = {0, 0, 0}; // Hide previous LED
            if (++LedID == 5) LedID = 0;
            *Colors[LedID] = RunColor;  // Shine next LED
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmBlinkAll:
            if (LedIsOn) {   // Was on, switch off
                if (!Delay.Elapsed(&Timer1, BlinkOnTime)) return;
                for (uint8_t i=0; i<5; i++) *Colors[i] = {0, 0, 0};
            }
            else {
                if (!Delay.Elapsed(&Timer1, BlinkOffTime)) return;
                for (uint8_t i=0; i<5; i++) *Colors[i] = BlinkColor;
            }
            LedIsOn = !LedIsOn;
            i2cMgr.AddCmd(i2cCmd);
            break;

        case lmRunAndBlink: // Blink: Timer2; Run: Timer1
            if (LedIsOn) {   // Was blinking on, check if time to switch off
                if (!Delay.Elapsed(&Timer2, BlinkOnTime)) return;
                for (uint8_t i=0; i<5; i++) *Colors[i] = {0, 0, 0};
                LedIsOn = false;
                i2cMgr.AddCmd(i2cCmd);
            }
            else {  // LED is off
                if (Delay.Elapsed(&Timer2, BlinkOffTime) && (BlinkColor.Red || BlinkColor.Green || BlinkColor.Blue)) {
                    for (uint8_t i=0; i<5; i++) *Colors[i] = BlinkColor;
                    LedIsOn = true;
                    i2cMgr.AddCmd(i2cCmd);
                }
                else {  // Proceed with running
                    if (!(Delay.Elapsed(&Timer1, RunDelay) && RunColor.IsOn())) return;
                    if (++LedID == 5) LedID = 0;
                    *Colors[LedID] = RunColor;  // Shine next LED
                    // Fade previous LED
                    ILedToFade = (LedID >= RunLedCount)? (LedID - RunLedCount) : ((5+LedID) - RunLedCount);
                    *Colors[ILedToFade] = {0, 0, 0};
                    i2cMgr.AddCmd(i2cCmd);
                }
            }
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
