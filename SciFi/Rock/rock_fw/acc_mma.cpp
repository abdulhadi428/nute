#include "acc_mma.h"

Acc_t Acc;

void Acc_t::Task() {
    if (!Delay.Elapsed(&Timer, ACC_PERIOD)) return;
}

void Acc_t::Init() {
    // Init INT pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = ACC_INT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ======== Variables =========
    Delay.Reset(&Timer);
    // Prepare i2cCmd for accelerations reading
    RegAddrToRead = ACC_REG_STATUS;
    i2cCmd.Address = ACC_I2C_ADDR;
    i2cCmd.DataToWrite.Buf = &RegAddrToRead;
    i2cCmd.DataToWrite.Length = 1;
    i2cCmd.DataToRead.Buf = AccArray;
    i2cCmd.DataToRead.Length = ACCELERATIONS_SIZE;
    i2cCmd.Callback = 0;
    // ==== Setup initial registers ====
    SingleReg_t RegBuf;
    // Setup High-Pass filter and acceleration scale
    RegBuf.RegAddr = ACC_REG_XYZ_DATA_CFG;
    RegBuf.RegValue = 0x01; // No filter, scale = 4g
    i2cMgr.WriteBufferPoll(ACC_I2C_ADDR, (uint8_t *)&RegBuf, I2C_SINGLEREG_SIZE);
    // Control registers
    uint8_t CtrlRegs[6];
    CtrlRegs[0] = ACC_REG_CONTROL1; // CtrReg[0] is register address
    CtrlRegs[1] = 0x21;     // DR=100 => 50Hz data rete; Mode = Active
    CtrlRegs[2] = 0x00;     // Normal mode
    CtrlRegs[3] = 0x02;     // No IRQ; IRQ output active high
    CtrlRegs[4] = 0x00;     // All interrupts disabled
    CtrlRegs[5] = 0x04;     // FreeFall IRQ is routed to INT1 pin
    i2cMgr.WriteBufferPoll(ACC_I2C_ADDR, &CtrlRegs[0], 6);
}

