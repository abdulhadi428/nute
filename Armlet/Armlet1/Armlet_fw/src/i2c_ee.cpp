#include "i2c_ee.h"
#include "kl_lib.h"
#include "lcd1200.h"

Pill_t Pill;

// =================================== Pill ====================================
void Pill_t::Task() {
    // Add cmd when needed
    if(DataXch) {
        if(Delay.Elapsed(&ITmr, DATA_DELAY)) {
            i2cMgr.AddCmd(&IDataCmd);
            DataXch = false;    // Data exchange has begun; next time send searching cmd
            return;
        }
    }
    else if(Delay.Elapsed(&ITmr, SEARCH_DELAY)) {
        i2cMgr.AddCmd(&ISearchCmd);
        return;
    }

    switch(State) {
        case esFailure:
            if(ISearchCmd.State == CmdSucceded) {
                //Uart.Printf("%A\r", ISearchCmd.BufToRead.P, 7);
                State = esNew;
                Lcd.Printf(0, 7, "Pill");
            }
            break;

        case esBusy:    // Check DataCmd
            if (IDataCmd.State == CmdSucceded) State = esReady;
            else if(IDataCmd.State == CmdFailed) State = esFailure;
            break;

        case esNew:
        case esReady:
            if(ISearchCmd.State == CmdFailed) {
                State = esFailure;
                Lcd.Printf(0, 7, "    ");
            }
            break;
    } // switch
}

void Pill_t::Init(uint8_t DevAddr) {
    IDevAddr = DevAddr;
    State = esFailure;
    DataXch = false;
    // Setup SearchCmd
    ISearchCmd.Address = IDevAddr;
    ISearchCmd.Byte = 0;
    ISearchCmd.ByteWrite = true;
    ISearchCmd.BufToWrite.Length = 0;
    ISearchCmd.BufToRead.Length = 0;
    i2cMgr.AddCmd(&ISearchCmd);

    // Setup Pill Power pin
    klGpioSetupByN(GPIOB, 2, GPIO_Mode_Out_PP);
    klGpioSetByN(GPIOB, 2);
}


void Pill_t::Read(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount) {
    DataXch = true;
    State = esBusy;
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    IDataCmd.Address = IDevAddr | (uint8_t)(MemoryAddr >> 8);
    IDataCmd.Byte = (uint8_t)(MemoryAddr & 0x00FF); // Read address
    IDataCmd.ByteWrite = true;
    IDataCmd.BufToWrite.Length = 0; // Write only one byte of address
    IDataCmd.BufToRead.P = PBuf;
    IDataCmd.BufToRead.Length = ACount;
}

void Pill_t::Write(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount) {
    State = esBusy;
    DataXch = true;
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    IDataCmd.Address = IDevAddr | (uint8_t)(MemoryAddr >> 8);
    IDataCmd.Byte = (uint8_t)(MemoryAddr & 0x00FF); // Write address
    IDataCmd.ByteWrite = true;
    IDataCmd.BufToWrite.Length = ACount;
    IDataCmd.BufToWrite.P = PBuf;
    IDataCmd.BufToRead.Length = 0;  // do not read
}

// ================================== ee_t =====================================
void ee_t::Init() {
    // Send initial cmd to determine device state
    IDataCmd.Address = INNER_EE_ADDR;
    IDataCmd.Byte = 0;
    IDataCmd.ByteWrite = true;
    IDataCmd.BufToWrite.Length = 0;
    IDataCmd.BufToRead.Length = 0;
    i2cMgr.AddCmd(&IDataCmd);
}

void ee_t::Read(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount) {
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    IDataCmd.Address = INNER_EE_ADDR | (uint8_t)(MemoryAddr >> 8);
    IDataCmd.Byte = (uint8_t)(MemoryAddr & 0x00FF);
    IDataCmd.ByteWrite = true;
    IDataCmd.BufToWrite.Length = 0; // Write only one byte of address
    IDataCmd.BufToRead.P = PBuf;
    IDataCmd.BufToRead.Length = ACount;
    i2cMgr.AddCmd(&IDataCmd);
}

void ee_t::Write(uint16_t MemoryAddr, uint8_t *PBuf, uint32_t ACount) {
    // Construct i2c addr (Device select): 0b1010 (A10)(A9)(A8). Do not shift it left.
    IDataCmd.Address = INNER_EE_ADDR | (uint8_t)(MemoryAddr >> 8);
    IDataCmd.Byte = (uint8_t)(MemoryAddr & 0x00FF); // Write address
    IDataCmd.ByteWrite = true;
    IDataCmd.BufToWrite.Length = ACount;
    IDataCmd.BufToWrite.P = PBuf;
    IDataCmd.BufToRead.Length = 0;  // do not read
    i2cMgr.AddCmd(&IDataCmd);
}
