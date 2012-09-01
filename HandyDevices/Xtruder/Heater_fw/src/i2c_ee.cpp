#include "i2c_ee.h"
#include "kl_lib.h"

ee_t ee;

/*
void ee_t::Init() {
    SingleReg_t RegBuf;
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
*/


uint8_t ee_t::Read(uint16_t AAddr, uint8_t *PBuffer, uint32_t ABufSize) {
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    uint8_t DevSel = 0b01010000 | (uint8_t)(AAddr >> 8);
    return i2cMgr.ReadBufferPoll()

}
