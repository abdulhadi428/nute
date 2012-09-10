#include "acc_mma.h"
#include "kl_lib.h"

void Acc_t::Task() {
    if (!IEnabled) return;
    if (!Delay.Elapsed(&CheckTimer, 108)) return;

    if(klGpioIsSetByN(GPIOB, 5)) {  // IRQ occured
        Uart.Printf("Irq\r");
        IClearIrq();
        IsTriggered = true;
        Delay.Reset(&EvtTimer);
    }
#ifdef ACC_ACCELERATIONS_NEEDED
    if(NewData()) Uart.Printf("X: %i; Y: %i; Z: %i\r", Accelerations.xMSB, Accelerations.yMSB, Accelerations.zMSB);
    ReadAccelerations();
#endif
    // Handle event
    if (IsTriggered) {
        if (!IsHandled) {
            EvtTrigger();
            IsHandled = true;
        }
        if (Delay.Elapsed(&EvtTimer, Delay_ms)) {
            // Time to shutdown
            IsTriggered = false;
            IsHandled = false;
            EvtNoTrigger();
        }
    }
}

void Acc_t::Init() {
    IEnabled = false;
    // Init INT pin
    //IPin.Init(GPIOB, 5, GPIO_Mode_IPD);
    klGpioSetupByN(GPIOB, 5, GPIO_Mode_IPD);

//    klPinIrq_t IPin(GPIOB, 5, GPIO_Mode_IPD);
//    IPin.IrqSetup(EXTI_Trigger_Rising);
//    IPin.IrqEnable();

    // ==== Setup initial registers ====
    // Put device to StandBy mode
    IWriteReg(ACC_REG_CONTROL1, 0b10100000);    // ODR = 50Hz, Standby
    // Setup High-Pass filter and acceleration scale
    IWriteReg(ACC_REG_XYZ_DATA_CFG, 0x01);      // No filter, scale = 4g
    // Setup Motion Detection
    IWriteReg(ACC_FF_MT_CFG, 0b11111000);       // Latch enable, detect motion, all three axes
    IWriteReg(ACC_FF_MT_THS, (uint8_t)(ACC_MOTION_TRESHOLD/0.063)); // Threshold = acceleration/0.063. "Detected" = (a > threshold)
    IWriteReg(ACC_FF_MT_COUNT, 0);              // Debounce counter: detect when moving longer than value*20ms (depends on ODR, see below)
    // Control registers
    IWriteReg(ACC_REG_CONTROL2, 0x00);          // Normal mode
    IWriteReg(ACC_REG_CONTROL3, 0b00001010);    // Freefall/motion may wake up system; IRQ output = active high, Push-Pull
    IWriteReg(ACC_REG_CONTROL4, 0b00000100);    // Freefall/motion IRQ enabled
    IWriteReg(ACC_REG_CONTROL5, 0b00000100);    // FreeFall/motion IRQ is routed to INT1 pin
    IWriteReg(ACC_REG_CONTROL1, 0b10100001);    // ASleep=10 => 6.75Hz; DR=100 => 50Hz output data rate (ODR); Mode = Active
}
