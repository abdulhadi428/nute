#include "i2c_mgr.h"
#include "stm32f10x_i2c.h"
#include "core_cm3.h"

i2cMgr_t i2cMgr;

// ============================ Tasking and handling ===========================
#ifndef I2C_POLL_ONLY
void i2cMgr_t::Task() {
    if (IsError) {
        Init();
        return;
    }
    uint8_t IEvt;
    switch (CmdToRead->State) {
        case CmdPending:
            if (SendStart() == I2C_OK) {    // Send Start
                // Send Addr of needed type
                if (CmdToRead->DataToWrite.Length != 0) {
                    SendAddrTX();
                    CmdToRead->State = CmdWritingAddrTX;
                }
                else {  // nothing to write
                    CmdToRead->State = CmdSucceded;
                    StopAndGetNext();
                }
            } // if start
            else {
                Uart.Printf("I2C Start failed\r");
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
            }
            break;

        // ****** Writing Address with TX bit *******
        case CmdWritingAddrTX:
            IEvt = CheckAddrTXSending();        // Check if ok
            if (IEvt == I2C_OK) {               // Addr sent, write data
                if (CmdToRead->DataToWrite.Length == 1) {   // Single byte to write
                    WriteOneByte();
                    CmdToRead->State = CmdWritingOne;
                }
                else {  // Many bytes to write
                    WriteMany();
                    CmdToRead->State = CmdWritingMany;
                }
            }
            else if (IEvt != I2C_WAITING) { // Some error occured
                if (IEvt == I2C_ERR_TIMEOUT) Uart.Printf("I2C Addr TX timeout\r");
                else if (IEvt == I2C_ERR_SLAVE_NACK) Uart.Printf("I2C NACK\r");
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
            }
            break;  // Otherwise still sending address

        case CmdWritingOne: // ****** Writing one byte *******
            IEvt = CheckOneByteWriting();
            if (IEvt == I2C_OK) {               // Byte sent, check if reading needed
                if (CmdToRead->DataToRead.Length != 0) {    // Send RepeatedStart
                    if (SendStart() == I2C_OK) {
                        SendAddrRX();
                        CmdToRead->State = CmdWritingAddrRX;
                    }
                    else {  // Repeated start failed
                        CmdToRead->State = CmdFailed;
                        StopAndGetNext();
                        Uart.Printf("I2C OneByte RS failed\r");
                    }
                }
                else {  // nothing to read
                    CmdToRead->State = CmdSucceded;
                    StopAndGetNext();
                    Uart.Printf("I2C OneByte no read\r");
                }
            } //
            else if (IEvt != I2C_WAITING) { // Some error occured
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
                Uart.Printf("I2C OneByte timeout\r");
            }
            break;  // Otherwise still sending byte

        // ****** Writing Address with RX bit *******
        case CmdWritingAddrRX:
            IEvt = CheckAddrRXSending();        // Check if ok
            if (IEvt == I2C_OK) {               // Addr sent, read data
                if (CmdToRead->DataToRead.Length == 1) {   // Single byte to read
                    ReadOneByte();
                    CmdToRead->State = CmdReadingOne;
                }
                else {  // Many bytes to read
                    ReadMany();
                    CmdToRead->State = CmdReadingMany;
                }
            }
            else if (IEvt != I2C_WAITING) { // Some error occured
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
                Uart.Printf("I2C Addr RX timeout\r");
            }
            break;  // Otherwise still sending address

        case CmdReadingOne: // ****** Reading one byte *******
            IEvt = CheckOneByteReading();
            if (IEvt == I2C_OK) {               // Byte read
                CmdToRead->State = CmdSucceded;
                GetNext();  // No need to send stop as it sent automatically
            }
            else if (IEvt != I2C_WAITING) { // Some error occured
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
            }
            break;  // Otherwise still reading byte

        case CmdWritingMany: // ****** Writing many bytes *******
            IEvt = CheckManyWriting();
            if (IEvt == I2C_OK) {               // Bytes sent, check if reading needed
                if (CmdToRead->DataToRead.Length != 0) {    // Send RepeatedStart and address RX
                    if (SendStart() == I2C_OK) {            // Send Start
                        SendAddrRX();
                        CmdToRead->State = CmdWritingAddrRX;
                    }
                    else {  // Repeated start failed
                        CmdToRead->State = CmdFailed;
                        StopAndGetNext();
                    }
                }
                else {  // nothing to read
                    CmdToRead->State = CmdSucceded;
                    StopAndGetNext();
                }
            } //
            else if (IEvt != I2C_WAITING) { // Some error occured
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
            }
            break;  // Otherwise still sending bytes

        case CmdReadingMany: // ****** Reading many bytes *******
            IEvt = CheckManyReading();
            if (IEvt == I2C_OK) {               // Byte read
                CmdToRead->State = CmdSucceded;
                StopAndGetNext();
            }
            else if (IEvt != I2C_WAITING) { // Some error occured
                CmdToRead->State = CmdFailed;
                StopAndGetNext();
                Uart.Printf("I2C ReadMany timeout\r");
            }
            break;  // Otherwise still reading bytes

        case CmdFailed:
        case CmdSucceded:
            GetNext();
            break;
    } // switch
}

void i2cMgr_t::SendAddrTX() {
    //Uart.Printf("AddrTx: %X\r", CmdToRead->Address);
    I2C_Send7bitAddress(I2C1, ((CmdToRead->Address) << 1), I2C_Direction_Transmitter);
    Delay.Reset(&Timer);
}
uint8_t i2cMgr_t::CheckAddrTXSending() {
    uint32_t IEvt = I2C_GetLastEvent(I2C1);
    if (IEvt == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) return I2C_OK;
    if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE)                 return I2C_ERR_SLAVE_NACK;   // NACK occured, slave doesn't answer
    if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS))              return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::SendAddrRX() {
    I2C_Send7bitAddress(I2C1, ((CmdToRead->Address) << 1), I2C_Direction_Receiver);
    Delay.Reset(&Timer);
}
uint8_t i2cMgr_t::CheckAddrRXSending() {
    uint32_t IEvt = I2C_GetLastEvent(I2C1);
    if (IEvt == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) return I2C_OK;
    if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE)              return I2C_ERR_SLAVE_NACK;   // NACK occured, slave doesn't answer
    if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS))           return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::WriteOneByte() {
    uint8_t b = (CmdToRead->DataToWrite.UseInnerBuf)? CmdToRead->DataToWrite.InnerBuf[0] : CmdToRead->DataToWrite.Buf[0];
    //Uart.Printf("W1b: %X\r", b);
    I2C_SendData(I2C1, b);
    Delay.Reset(&Timer);
}
uint8_t i2cMgr_t::CheckOneByteWriting() {
    uint32_t IEvt = I2C_GetLastEvent(I2C1);
    if (IEvt == I2C_EVENT_MASTER_BYTE_TRANSMITTED) return I2C_OK;
    if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE)        return I2C_ERR_SLAVE_NACK;   // NACK occured, slave doesn't answer
    if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS))     return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::ReadOneByte() {
    // Disable Acknowledgement, as only one byte is to be received
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    // Send STOP Condition and then read byte
    I2C_GenerateSTOP(I2C1, ENABLE);
    Delay.Reset(&Timer);
}
uint8_t i2cMgr_t::CheckOneByteReading() {
    uint32_t IEvt = I2C_GetLastEvent(I2C1);
    if (IEvt & I2C_FLAG_RXNE) {
        uint8_t b = I2C_ReceiveData(I2C1);
        // Write data if buffer is not NULL
        if(CmdToRead->DataToRead.Buf != 0) CmdToRead->DataToRead.Buf[0] = b;
        return I2C_OK;
    }
    if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS)) return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::WriteMany() {
    // Prepare DMA
    DMA_DeInit(I2C_DMA_CHNL_TX);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &I2C1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) &CmdToRead->DataToWrite.Buf[0];
    DMA_InitStructure.DMA_BufferSize         = CmdToRead->DataToWrite.Length;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;  // From memory to I2C
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(I2C_DMA_CHNL_TX, &DMA_InitStructure);
    // Start transmission
    DMA_Cmd(I2C_DMA_CHNL_TX, ENABLE);   // Enable DMA channel
    I2C_DMACmd(I2C1, ENABLE);           // Enable DMA
    //Uart.PrintString("ST\r\r");
    Delay.Reset(&Timer);
}
uint8_t i2cMgr_t::CheckManyWriting() {
    // Check if DMA transfer ended
    if (DMA_GetFlagStatus(I2C_DMA_FLAG_TC_TX) == SET)
        if (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == RESET)
            return I2C_OK;

    if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS)) return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::ReadMany() {
    // Enable Acknowledgement
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    // Prepare DMA
    DMA_DeInit(I2C_DMA_CHNL_RX);
    DMA_InitTypeDef DMA_InitStructure;
    //DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &I2C1->DR;
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40005410;
    // Decide where to read data to
    if(CmdToRead->DataToRead.Buf == 0) {    // no need in this data
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CmdToRead->DataToRead.InnerBuf[0];   // dummy place
        DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Disable;   // Do not move pointer
    }
    else {
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CmdToRead->DataToRead.Buf[0];
        DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Enable;
    }
    DMA_InitStructure.DMA_BufferSize         = CmdToRead->DataToRead.Length;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;  // From I2C to memory
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(I2C_DMA_CHNL_RX, &DMA_InitStructure);
    //Inform the DMA that the next End Of Transfer Signal will be the last one, need to send NACK after last byte
    I2C_DMALastTransferCmd(I2C1, ENABLE);
    // Start transmission
    I2C_DMACmd(I2C1, ENABLE);           // Enable DMA
    DMA_Cmd(I2C_DMA_CHNL_RX, ENABLE);   // Enable DMA channel

    Delay.Reset(&Timer);
}
uint8_t i2cMgr_t::CheckManyReading() {
    // Check if DMA transfer ended
    if (DMA_GetFlagStatus(I2C_DMA_FLAG_TC_RX) == SET) return I2C_OK;
    if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS))        return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::GetNext() {
    // Report Cmd state if needed
    if(CmdToRead->StateReport != 0) *CmdToRead->StateReport = CmdToRead->State;
    CmdToRead++;
    if ((CmdToRead - &Commands[0]) >= I2C_CMD_QUEUE_LENGTH) CmdToRead = &Commands[0];
}
void i2cMgr_t::StopAndGetNext() {
    I2C_GenerateSTOP(I2C1, ENABLE);
    GetNext();
}
#endif

// ================================ Init & Add Cmd =============================
void i2cMgr_t::Init() {
    // Init GPIO - SCL & SDA
    klGpioSetupByMsk(GPIOB, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AF_OD);
    // I2C
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);    // I2C clock
    I2C_DeInit(I2C1);
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_CLOCK_FREQ;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
#ifndef I2C_POLL_ONLY
    // ==== Init manager ====
    CmdToWrite = &Commands[0];
    CmdToRead  = &Commands[0];
    // Reset stage of all commands
    for (uint8_t i=0; i<I2C_CMD_QUEUE_LENGTH; i++) Commands[i].State = CmdSucceded;
    // ==== Init DMA ====
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    IsError = false;
#endif
}

#ifndef I2C_POLL_ONLY
void i2cMgr_t::AddCmd(I2C_Cmd_t ACmd) {
    ACmd.State = CmdPending;
    *CmdToWrite = ACmd;     // Copy command to buffer
    //klPrintf("Cmd: %X l=%u\r", ACmd.DataToWrite.Buf[0], ACmd.DataToWrite.Length);
    // Process command queue
    CmdToWrite++;
    if ((CmdToWrite - &Commands[0]) >= I2C_CMD_QUEUE_LENGTH) CmdToWrite = &Commands[0];
}

void i2cMgr_t::AddCmdRegistersRead(uint8_t ADevAddr, uint8_t ARegAddr, uint8_t *PBuf, uint8_t ALength, CmdState_t *PStateReport) {
    // Write new Cmd to buffer
    CmdToWrite->Address = ADevAddr;
    CmdToWrite->DataToWrite.UseInnerBuf = true;   // No outer writebuffer to point to
    CmdToWrite->DataToWrite.Length = 1;
    CmdToWrite->DataToWrite.InnerBuf[0] = ARegAddr;
    CmdToWrite->DataToRead.Length = ALength;
    CmdToWrite->DataToRead.Buf = PBuf;
    CmdToWrite->State = CmdPending;
    CmdToWrite->StateReport = PStateReport;
    CmdToWrite++;
    if ((CmdToWrite - &Commands[0]) >= I2C_CMD_QUEUE_LENGTH) CmdToWrite = &Commands[0];
}

#endif
// =========================== Polling functions ===============================
// Wait for Idle Bus
uint8_t i2cMgr_t::BusyWait() {
    uint32_t FTimeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
        if((FTimeout--) == 0) {
            Uart.Printf("I2C BusyWait timeout\r");
            return I2C_ERR_TIMEOUT;
        }
    return I2C_OK;
}
uint8_t i2cMgr_t::SendStart(void) {
    I2C_GenerateSTART(I2C1, ENABLE);
    uint32_t FTimeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if ((FTimeout--) == 0) {
            Uart.Printf("I2C can't enter master mode\r");
            return I2C_ERR_NOMASTER;
        }
    }
    return I2C_OK;
}
uint8_t i2cMgr_t::SendAddrTXPoll(uint8_t AAddr) {
    uint32_t IEvt;
    I2C_Send7bitAddress(I2C1, (AAddr << 1), I2C_Direction_Transmitter);
    uint32_t FTimeout = I2C_TIMEOUT;
    do {
        IEvt = I2C_GetLastEvent(I2C1);
        if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
            Uart.Printf("I2C Slave Addr TX NACK\r");
            return I2C_ERR_SLAVE_NACK;
        }
        if ((FTimeout--) == 0) {
            Uart.Printf("I2C Slave Addr Timeout\r");
            return I2C_ERR_TIMEOUT;
        }
    } while (IEvt != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    //Uart.Printf("I2C Slave Addr TX ACK\r");
    return I2C_OK;   // all right
}
uint8_t i2cMgr_t::SendAddrRXPoll(uint8_t AAddr) {
    uint32_t IEvt;
    I2C_Send7bitAddress(I2C1, (AAddr << 1), I2C_Direction_Receiver);
    uint32_t FTimeout = I2C_TIMEOUT;
    do {
        IEvt = I2C_GetLastEvent(I2C1);
        if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
            Uart.Printf("I2C Slave Addr RX NACK\r");
            return I2C_ERR_SLAVE_NACK;
        }
        if ((FTimeout--) == 0) {
            Uart.Printf("I2C Slave Addr Timeout\r");
            return I2C_ERR_TIMEOUT;
        }
    } while (IEvt != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    //Uart.Printf("I2C Slave Addr RX ACK\r");
    return I2C_OK;   // all right
}

uint8_t i2cMgr_t::WriteBytePoll(uint8_t AByte) {
    uint32_t IEvt;
    //Uart.Printf("%X\r", AByte);
    I2C_SendData(I2C1, AByte);
    // Wait for ok, or for NACK or Timeout
    uint32_t ITimeout = I2C_TIMEOUT;
    do {
        IEvt = I2C_GetLastEvent(I2C1);
        if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
            Uart.Printf("\rI2C Slave NACK\r");
            return I2C_ERR_SLAVE_NACK;
        }
        if(ITimeout-- == 0) {
            Uart.Printf("\rI2C Slave Timeout\r");
            return I2C_ERR_TIMEOUT;
        }
    } while(IEvt != I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    return I2C_OK;
}

uint8_t i2cMgr_t::CmdPoll(I2C_Cmd_t ACmd) {
	//Uart.Printf("i2c Cmd Poll\r");
    if(ACmd.StateReport != 0) *ACmd.StateReport = CmdFailed;
	if(BusyWait()) return I2C_ERR_TIMEOUT;
	if(ACmd.DataToWrite.Length == 0) return I2C_OK;
	uint8_t rslt=I2C_ERR_TIMEOUT;
    uint32_t IEvt, ITimeout;
    uint8_t i, b;
	// ==== First stage ====
    //Uart.Printf("i2c write: %A\r", ACmd.DataToWrite.Buf, ACmd.DataToWrite.Length);
	if ((rslt = SendStart()) == I2C_OK) {
		rslt = SendAddrTXPoll(ACmd.Address);
		for(i=0; (i < ACmd.DataToWrite.Length) and (rslt == I2C_OK); i++) {
		    b = ACmd.DataToWrite.Buf[i];
			//Uart.Printf("%X\r", b);
		    rslt = WriteBytePoll(b);
		} // while

		// ==== Second stage ====
		if((ACmd.DataToRead.Length != 0) and (rslt == I2C_OK)) {
			//Uart.Printf("i2c read %u\r", ACmd.DataToRead.Length);
			if((rslt = SendStart()) == I2C_OK) {
				if((rslt = SendAddrRXPoll(ACmd.Address)) == I2C_OK) {
					// Act accordingly to number of bytes to receive
					if(ACmd.DataToRead.Length == 1) {		// 1 byte to read
						I2C_AcknowledgeConfig(I2C1, DISABLE);
						I2C_GenerateSTOP(I2C1, ENABLE);
						// Wait for the byte to be received
						ITimeout = I2C_TIMEOUT;
						while(1) {	// Check if received
							IEvt = I2C_GetLastEvent(I2C1);
							if(IEvt == I2C_EVENT_MASTER_BYTE_RECEIVED) {
								ACmd.DataToRead.Buf[0] = I2C_ReceiveData(I2C1);
								break;
							}
							if(ITimeout-- == 0) {
								rslt = I2C_ERR_TIMEOUT;
								break;
							}
						} // while
					} // if 1
					else if(ACmd.DataToRead.Length == 2) {	// 2 bytes to read
						I2C_AcknowledgeConfig(I2C1, DISABLE);
						// Wait for the two bytes to be received
						ITimeout = I2C_TIMEOUT;
						while(1) {
							IEvt = I2C_GetLastEvent(I2C1);
							if(IEvt & I2C_FLAG_BTF) {
								I2C_GenerateSTOP(I2C1, ENABLE);
								ACmd.DataToRead.Buf[0] = I2C_ReceiveData(I2C1);
								ACmd.DataToRead.Buf[1] = I2C_ReceiveData(I2C1);
								break;
							}
							if(ITimeout-- == 0) {
								rslt = I2C_ERR_TIMEOUT;
								break;
							}
						} // while
					}
					else {	// many bytes to read
						I2C_AcknowledgeConfig(I2C1, ENABLE);
						i = 0;
						while((i < ACmd.DataToRead.Length) and (rslt == I2C_OK)) {
							ITimeout = I2C_TIMEOUT;
							while(1) {
								IEvt = I2C_GetLastEvent(I2C1);
								if(i == ACmd.DataToRead.Length-3) {	// Catch BTF
									if(IEvt & I2C_FLAG_BTF) {
										__disable_irq();
										I2C_AcknowledgeConfig(I2C1, DISABLE);
										ACmd.DataToRead.Buf[i] = I2C_ReceiveData(I2C1);
										I2C_GenerateSTOP(I2C1, ENABLE);
										i++;
										ACmd.DataToRead.Buf[i] = I2C_ReceiveData(I2C1);
										__enable_irq();
										break;
									}
								}
								else if(IEvt & I2C_FLAG_RXNE) {
									ACmd.DataToRead.Buf[i] = I2C_ReceiveData(I2C1);
									break;
								}

								if(ITimeout-- == 0) {
									rslt = I2C_ERR_TIMEOUT;
									break;
								}
							} // while 1
							i++;
						} // while
					} // many bytes to read
				} // Send addr
			} // send repstart
		} // if second stage

		else {	// only write operation (or failure), generate Stop
			//Uart.Printf("st\r");
			I2C_GenerateSTOP(I2C1, ENABLE);
		}
	} // if start
    if((rslt == I2C_OK) and (ACmd.StateReport != 0)) *ACmd.StateReport = CmdSucceded;
    //if(rslt == I2C_OK) Uart.Printf("Done\r");
	return rslt;
}

uint8_t i2cMgr_t::WriteRegPoll(uint8_t AI2CAddr, uint8_t ARegAddr, uint8_t AValue) {
    //Uart.Printf("i2c WriteReg Poll\r");
    if(BusyWait()) return I2C_ERR_TIMEOUT;
    uint8_t rslt;
    if ((rslt = SendStart()) == I2C_OK) {
        if((rslt = SendAddrTXPoll(AI2CAddr)) == I2C_OK) {
            // Send reg addr
            if((rslt = WriteBytePoll(ARegAddr)) == I2C_OK) {
                // Send data
                rslt = WriteBytePoll(AValue);
            }
        }
        I2C_GenerateSTOP(I2C1, ENABLE);
    }
    //if(rslt == I2C_OK) Uart.Printf("Done\r");
    return rslt;
}
