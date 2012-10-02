#include "acc_mma.h"
#include "kl_lib.h"

int16_t AccProcess(uint8_t Msb, uint8_t Lsb) {
	// Check if negative
	bool IsNegative = false;
	if(Msb > 0x7F) {
		// Change sign
		Msb = ~Msb + 1;
		Lsb = ~Lsb + 1;
		IsNegative = true;
	}
	uint16_t w1 = (uint16_t)Msb, w2 = (uint16_t)Lsb;
	w1 <<= 8;
	w1 |= w2;
	w1 >>= 4;
	// Now w1 contains positive representation of acceleration
	int16_t iw = w1;
	if(IsNegative) iw = -iw;
	return iw;
}

void Acc_t::Task() {
	//static uint32_t Tmr;
    //if (!Delay.Elapsed(&Tmr, 450)) return;

	if(ICmd.State == CmdSucceded) {
		if(Accelerations.Status & 0x08) {	// Check if new info
			INewData = true;
			//Uart.Printf("%X; %X; %X\r", Accelerations.xMSB, Accelerations.yMSB, Accelerations.zMSB);
			x = AccProcess(Accelerations.xMSB, Accelerations.xLSB);
			y = AccProcess(Accelerations.yMSB, Accelerations.yLSB);
			z = AccProcess(Accelerations.zMSB, Accelerations.zLSB);
			//Uart.Printf("X: %d; Y: %d; Z: %d\r", x, y, z);
		}
	}
	// Send new cmd if needed
	if((ICmd.State == CmdSucceded) or (ICmd.State == CmdFailed)) i2cMgr.AddCmd(&ICmd);
}

void Acc_t::Init() {
    ICmd.Address = ACC_I2C_ADDR;
    // ==== Setup initial registers ====
    // Put device to StandBy mode
    IWriteReg(ACC_REG_CONTROL1, 0b10100000);    // ODR = 50Hz, Standby
    // Setup High-Pass filter and acceleration scale
    //IWriteReg(ACC_REG_XYZ_DATA_CFG, 0x01);      // No filter, scale = 4g
    IWriteReg(ACC_REG_XYZ_DATA_CFG, 0x02);      // No filter, scale = 8g
    // Control registers
    IWriteReg(ACC_REG_CONTROL2, 0x00);          // Normal mode
//    IWriteReg(ACC_REG_CONTROL3, 0b00001010);    // Freefall/motion may wake up system; IRQ output = active high, Push-Pull
    IWriteReg(ACC_REG_CONTROL3, 0b00000010);    // IRQ output = active high, Push-Pull
    //IWriteReg(ACC_REG_CONTROL4, 0b00000100);    // Freefall/motion IRQ enabled
    IWriteReg(ACC_REG_CONTROL4, 0b00000000);    // IRQs disabled
    IWriteReg(ACC_REG_CONTROL5, 0b00000100);    // FreeFall/motion IRQ is routed to INT1 pin
    //IWriteReg(ACC_REG_CONTROL1, 0b10100001);    // ASleep=10 => 6.75Hz; DR=100 => 50Hz output data rate (ODR); Mode = Active
    IWriteReg(ACC_REG_CONTROL1, 0b10011001);    // ASleep=10 => 6.75Hz; DR=011 => 100Hz output data rate (ODR); Mode = Active

    // Prepare ICmd
    ICmd.BufToWrite.Length = 0;	// No buffer to write
    ICmd.Byte = ACC_REG_STATUS;
    ICmd.ByteWrite = true;
    ICmd.BufToRead.P = (uint8_t*)&Accelerations;
    ICmd.BufToRead.Length = ACCELERATIONS_SIZE;
    i2cMgr.AddCmd(&ICmd);
}

uint8_t Acc_t::IWriteReg(uint8_t AAddr, uint8_t AValue) {
	uint8_t Buf[2];
	Buf[0] = AAddr;
	Buf[1] = AValue;
	// Setup cmd
	ICmd.BufToWrite.P = Buf;
	ICmd.BufToWrite.Length = 2;
	ICmd.ByteWrite = false;		// Do not write byte
	ICmd.BufToRead.Length = 0;	// Do not read
	return i2cMgr.CmdPoll(ICmd);
}
