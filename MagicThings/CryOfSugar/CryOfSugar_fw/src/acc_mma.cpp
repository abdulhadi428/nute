#include "acc_mma.h"
#include "kl_lib.h"
#include "delay_util.h"

void Acc_t::Task() {
    if (!IEnabled) return;
    if (!Delay.Elapsed(&CheckTimer, 108)) return;

    // Calc total acc
    uint32_t a = Accelerations.xMSB * Accelerations.xMSB;
    a += Accelerations.yMSB * Accelerations.yMSB;
    a += Accelerations.zMSB * Accelerations.zMSB;

    if ((a >= ThresholdTop) || (a < ThresholdBottom)) {
        IsTriggered = true;
        Delay.Reset(&EvtTimer);
    }

    // Handle event
    if (IsTriggered) {
        if (!IsHandled) {
            if (EvtTrigger != 0) EvtTrigger();
            IsHandled = true;
        }
        if (Delay.Elapsed(&EvtTimer, Delay_ms)) {
            // Time to shutdown
            IsTriggered = false;
            IsHandled = false;
            if (EvtNoTrigger != 0) EvtNoTrigger();
        }
    }

    // Ask new accelerations
    ReadAccelerations();
}

void Acc_t::Init() {
    IEnabled = false;
    // Init INT pin
//    klPinIrq_t IPin(GPIOB, 5, GPIO_Mode_IPD);
//    IPin.IrqSetup(EXTI_Trigger_Rising);
//    IPin.IrqEnable();
    // ======== Variables =========
    // Prepare i2cCmd for accelerations reading
    RegAddrToRead = ACC_REG_STATUS;
    i2cCmd.Address = ACC_I2C_ADDR;
    i2cCmd.DataToWrite.Buf = &RegAddrToRead;
    i2cCmd.DataToWrite.Length = 1;
    i2cCmd.DataToRead.Buf = (uint8_t*)&Accelerations;
    i2cCmd.DataToRead.Length = ACCELERATIONS_SIZE;
    i2cCmd.Callback = 0;
    // ==== Setup initial registers ====
    SingleReg_t RegBuf;
    // Setup High-Pass filter and acceleration scale
    RegBuf.RegAddr = ACC_REG_XYZ_DATA_CFG;
    RegBuf.RegValue = 0x01; // No filter, scale = 4g
    i2cMgr.WriteBufferPoll(ACC_I2C_ADDR, (uint8_t *)&RegBuf, I2C_SINGLEREG_SIZE);
//    // Setup Motion Detection
//    RegBuf.RegAddr = ACC_FF_MT_CFG;
//    RegBuf.RegValue = 0b11111000; // Latch enable, detect motion, all three axes
//    i2cMgr.WriteBufferPoll(ACC_I2C_ADDR, (uint8_t *)&RegBuf, I2C_SINGLEREG_SIZE);
//    RegBuf.RegAddr = ACC_FF_MT_THS;
//    RegBuf.RegValue = 19;   // Threshold
//    i2cMgr.WriteBufferPoll(ACC_I2C_ADDR, (uint8_t *)&RegBuf, I2C_SINGLEREG_SIZE);
    // Control registers
    uint8_t CtrlRegs[6];
    CtrlRegs[0] = ACC_REG_CONTROL1; // CtrReg[0] is register address
    CtrlRegs[1] = 0x21;     // DR=100 => 50Hz data rate; Mode = Active
    CtrlRegs[2] = 0x00;     // Normal mode
    CtrlRegs[3] = 0x02;     // No IRQ; IRQ output active high
    CtrlRegs[4] = 0x00;     // All interrupts disabled
    CtrlRegs[5] = 0x04;     // FreeFall IRQ is routed to INT1 pin
    i2cMgr.WriteBufferPoll(ACC_I2C_ADDR, &CtrlRegs[0], 6);
}
