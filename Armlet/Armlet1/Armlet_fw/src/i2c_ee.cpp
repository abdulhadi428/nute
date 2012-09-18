#include "i2c_ee.h"
#include "kl_lib.h"

ee_t ee;

uint8_t ee_t::Read(uint8_t DevAddr, uint16_t MemoryAddr, uint32_t ACount) {
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    ICmd.Address = DevAddr | (uint8_t)(MemoryAddr >> 8);
    IBuf[0] = (uint8_t)(MemoryAddr & 0x00FF);	// Byte address
    ICmd.DataToWrite.Length = 1;
    ICmd.DataToRead.Length = ACount;
    return i2cMgr.CmdPoll(ICmd);
}

uint8_t ee_t::Write(uint8_t DevAddr, uint16_t MemoryAddr, uint32_t ACount) {
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    ICmd.Address = DevAddr | (uint8_t)(MemoryAddr >> 8);
    IBuf[0] = (uint8_t)(MemoryAddr & 0x00FF);	// Byte address
    ICmd.DataToWrite.Length = 1 + ACount;	// Write address and data
    ICmd.DataToRead.Length = 0;				// Nothing to read
    return i2cMgr.CmdPoll(ICmd);
}
