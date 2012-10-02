#include "i2c_mgr.h"
#include "stm32f10x_i2c.h"
#include "core_cm3.h"

i2cMgr_t i2cMgr;

// ============================ Tasking and handling ===========================
#ifndef I2C_POLL_ONLY
void i2cMgr_t::Task() {
    if (IsError) {
        HwInit();
        return;
    }
    uint8_t IEvt;
    // Bypass zero pointers
    if(Cmd[RIndx] == 0) GetNext();
    else { // not a zero pointer
        //Uart.Printf("R: %u; St: %u\r", RIndx, Cmd[RIndx]->State);
        switch(Cmd[RIndx]->State) {
            case CmdPending:
                //Uart.Printf("==== Pnd %u ====\r", RIndx);
                // Check if Busy and timeout after Stop
                if(IsBusy()) {
                    if(Delay.Elapsed(&Timer, 540)) IsError = true;   // Reset I2C
                    break;
                }
                // Clear flags
                I2C1->SR1 = 0;
                while(IsRxNotEmpty()) (void)I2C1->DR;   // Read DR until it empty
                ClearAddrFlag();
                // Start the job
                if (SendStart() == I2C_OK) {    // Send Start
                    // Send Addr of needed type
                    if ((Cmd[RIndx]->BufToWrite.Length != 0) or Cmd[RIndx]->ByteWrite) {
                        I2C_Send7bitAddress(I2C1, ((Cmd[RIndx]->Address) << 1), I2C_Direction_Transmitter);
                        Cmd[RIndx]->State = CmdWritingAddrTX;
                    }
                    else {  // nothing to write
                        Cmd[RIndx]->State = CmdFailed;
                        StopAndGetNext();
                    }
                } // if start
                else {
                    Uart.Printf("Start fail\r");
                    Cmd[RIndx]->State = CmdFailed;
                    IsError = true;
                }
                break;

            // ==== Writing Address with TX bit ====
            case CmdWritingAddrTX:
                //Uart.Printf("ATx\r");
//                IEvt = I2C_GetLastEvent(I2C1);
//                Uart.Printf("AddrTX evt: %X8\r", IEvt);
                if(IsAddrSent()) {                  // Addr sent, write data
                    ClearAddrFlag();                // EV6
                    if(Cmd[RIndx]->ByteWrite) {     // write byte first
                        I2C_SendData(I2C1, Cmd[RIndx]->Byte);
                        Cmd[RIndx]->State = CmdWritingByte;
                    }
                    else if(Cmd[RIndx]->BufToWrite.Length == 1) {
                        I2C_SendData(I2C1, Cmd[RIndx]->BufToWrite.P[0]);
                        Cmd[RIndx]->State = CmdWritingBufSingle;
                    }
                    else {  // Many bytes to write
                        WriteMany();
                        Cmd[RIndx]->State = CmdWritingBufMany;
                    }
                }
                else if(IsNack()) {
                    //Uart.Printf("NACK 1\r");
                    Cmd[RIndx]->State = CmdFailed;
                    GetNext();
                }
                break;  // Otherwise still sending address

            // ==== Writing one byte ====
            case CmdWritingByte:
                //Uart.Printf("Wb\r");
                //IEvt = I2C_GetLastEvent(I2C1);
                //Uart.Printf("Wb evt: %X8\r", IEvt);
                if (IsTransferCompleted()) {    // EV8_2: Byte sent
                    // Check BufToWrite
                    if(Cmd[RIndx]->BufToWrite.Length == 0) {        // Nothing to write
                        if (Cmd[RIndx]->BufToRead.Length == 0) {    // Nothing to read
                            Cmd[RIndx]->State = CmdSucceded;
                            StopAndGetNext();
                        }
                        else {  // Read needed
                            if (SendStart() == I2C_OK) {            // Send RepeatedStart
                                SendAddrRX();
                                Cmd[RIndx]->State = CmdWritingAddrRX;
                            }
                            else {  // Repeated start failed
                                Cmd[RIndx]->State = CmdFailed;
                                GetNext();
                                Uart.Printf("RS1 fail\r");
                            }
                        } // if read lengt != 0
                    }
                    else if(Cmd[RIndx]->BufToWrite.Length == 1) {   // Write one byte
                        //Uart.Printf("Buf0: %X\r", Cmd[RIndx]->BufToWrite.P[0]);
                        I2C_SendData(I2C1, Cmd[RIndx]->BufToWrite.P[0]);
                        Cmd[RIndx]->State = CmdWritingBufSingle;
                    }
                    else {  // Many bytes to write
                        WriteMany();
                        Cmd[RIndx]->State = CmdWritingBufMany;
                    }
                } // if byte sent
                else if(IsNack()) {
                    Cmd[RIndx]->State = CmdFailed;
                    GetNext();
                    Uart.Printf("NACK 2\r");
                }
                break;  // Otherwise still sending byte

            // ==== Writing single byte of buffer ====
            case CmdWritingBufSingle:
                //Uart.Printf("Wbs\r");
                //IEvt = I2C_GetLastEvent(I2C1);
                //Uart.Printf("Wb evt: %X8\r", IEvt);
                if (IsTransferCompleted()) {    // Byte sent, check if reading needed
                    if (Cmd[RIndx]->BufToRead.Length == 0) {    // Nothing to read
                        Cmd[RIndx]->State = CmdSucceded;
                        StopAndGetNext();
                    }
                    else {  // Read needed
                        if (SendStart() == I2C_OK) {            // Send RepeatedStart
                            SendAddrRX();
                            Cmd[RIndx]->State = CmdWritingAddrRX;
                        }
                        else {  // Repeated start failed
                            Cmd[RIndx]->State = CmdFailed;
                            StopAndGetNext();
                            Uart.Printf("RS2 fail\r");
                        }
                    } // if read lengt != 0
                } // if byte sent
                else if(IsNack()) {
                    Cmd[RIndx]->State = CmdFailed;
                    StopAndGetNext();
                    Uart.Printf("NACK 3\r");
                }
                break;

            // ==== Writing Address with RX bit ====
            case CmdWritingAddrRX:
                //Uart.Printf("WARx\r");
                if(IsAddrSent()) { // Addr sent
                    if (Cmd[RIndx]->BufToRead.Length == 1) {    // Single byte to read
                        I2C_AcknowledgeConfig(I2C1, DISABLE);   // Disable ACK
                        ClearAddrFlag();                        // Clear ADDR
                        I2C_GenerateSTOP(I2C1, ENABLE);         // Send stop after byte rcvd
                        Cmd[RIndx]->State = CmdReadingBufSingle;
                    }
                    else {  // Many bytes to read
                        ClearAddrFlag();                        // Clear ADDR
                        ReadMany();
                        Cmd[RIndx]->State = CmdReadingBufMany;
                    } // many
                } // if Addr set
                else if(IsNack()) {
                    Uart.Printf("NACK 4\r");
                    Cmd[RIndx]->State = CmdFailed;
                    StopAndGetNext();
                }
                break;  // Otherwise still sending address

            // ==== Reading one byte ====
            case CmdReadingBufSingle:
                //Uart.Printf("Rbs\r");
                if(IsRxNotEmpty()) {
                    Cmd[RIndx]->BufToRead.P[0] = I2C_ReceiveData(I2C1);
                    Cmd[RIndx]->State = CmdSucceded;
                    GetNext(); // Stop is already sent
                }
                break;  // Otherwise still reading byte

            // ==== Writing many bytes ====
            case CmdWritingBufMany:
                //Uart.Printf("Wbm\r");
                // Check if DMA transfer ended
                if (DMA_GetFlagStatus(I2C_DMA_FLAG_TC_TX) == SET) { // Bytes sent, check if reading needed
                    if (Cmd[RIndx]->BufToRead.Length != 0) {    // Send RepeatedStart and address RX
                        if (SendStart() == I2C_OK) {            // Send Start
                            SendAddrRX();
                            Cmd[RIndx]->State = CmdWritingAddrRX;
                        }
                        else {  // Repeated start failed
                            Cmd[RIndx]->State = CmdFailed;
                            StopAndGetNext();
                        }
                    }
                    else {  // nothing to read
                        Cmd[RIndx]->State = CmdSucceded;
                        StopAndGetNext();
                    }
                } //
                break;  // Otherwise still sending bytes

            // ==== Reading many bytes ====
            case CmdReadingBufMany:
                //Uart.Printf("RBm\r");
                IEvt = CheckManyReading();
                if (IEvt == I2C_OK) {
                    Cmd[RIndx]->State = CmdSucceded;
                    //Uart.Printf("i %A\r", Cmd[RIndx]->BufToRead.P, Cmd[RIndx]->BufToRead.Length);
                    StopAndGetNext();
                }
                else if (IEvt != I2C_WAITING) { // Some error occured
                    Cmd[RIndx]->State = CmdFailed;
                    StopAndGetNext();
                    Uart.Printf("I2C ReadMany timeout\r");
                }
                break;  // Otherwise still reading bytes

            case CmdFailed:
            case CmdSucceded:
                GetNext();
                break;
        } // switch
    } // else
}

uint8_t i2cMgr_t::CheckAddrRXSending() {
    uint32_t IEvt = I2C_GetLastEvent(I2C1);
    if (IEvt == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) return I2C_OK;
    if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE)              return I2C_ERR_SLAVE_NACK;   // NACK occured, slave doesn't answer
    if (Delay.Elapsed(&Timer, 4))                        return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::WriteMany() {
    // Prepare DMA
    DMA_DeInit(I2C_DMA_CHNL_TX);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &I2C1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) Cmd[RIndx]->BufToWrite.P;
    DMA_InitStructure.DMA_BufferSize         = Cmd[RIndx]->BufToWrite.Length;
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

void i2cMgr_t::ReadMany() {
    // Enable Acknowledgement
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    // Prepare DMA
    DMA_DeInit(I2C_DMA_CHNL_RX);
    DMA_InitTypeDef DMA_InitStructure;
    //DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &I2C1->DR;
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40005410;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) Cmd[RIndx]->BufToRead.P;
    DMA_InitStructure.DMA_BufferSize         = Cmd[RIndx]->BufToRead.Length;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;  // From I2C to memory
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
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
    //if (Delay.Elapsed(&Timer, I2C_TIMEOUT_MS))        return I2C_ERR_TIMEOUT;
    return I2C_WAITING;
}

void i2cMgr_t::StopAndGetNext() {
    I2C_GenerateSTOP(I2C1, ENABLE);
    GetNext();
    Delay.Reset(&Timer);
}
#endif

// ================================ Init & Add Cmd =============================
void i2cMgr_t::HwInit() {
	Uart.Printf("I2C HwInit\r");
	Uart.FlushTx();
    I2C_DeInit(I2C1);
    // ==== First, check if bus is not in BUSY state ====
    klPin_t Scl, Sda;
    Scl.Init(GPIOB, 6, GPIO_Mode_Out_OD);
    Sda.Init(GPIOB, 7, GPIO_Mode_IN_FLOATING);
    Scl = 1;    // Clk Idle High
    Sda = 1;    // Try to set 1 on SDA
    if(Sda == 0) {
    	Uart.Printf("SDA 0\r");
    	Uart.FlushTx();
    	while(1) {  // Look at result
			// Send clock pulse
			Scl = 0; Delay.ms(1);
			Scl = 1; Delay.ms(1);
			if(Sda == 1) {  // looks like released bus, send stop (SDA Lo-Hi when SCL High)
				Scl = 0; Delay.ms(1);
				Sda.Init(GPIOB, 7, GPIO_Mode_Out_OD);
				Sda = 0; Delay.ms(1);
				Scl = 1; Delay.ms(1);
				Sda = 1; Delay.ms(1);
				break;
			}
		} // while
    	Uart.Printf("SDA ok\r");
    }
    // ==== Proceed with init ====
    // Init GPIO - SCL & SDA
    klGpioSetupByMsk(GPIOB, GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AF_OD);
    // I2C
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);    // I2C clock
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_CLOCK_FREQ;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
    I2C_GetLastEvent(I2C1); // Clear flags
    // ==== Init DMA ====
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#ifndef I2C_POLL_ONLY
    IsError = false;
#endif
}

void i2cMgr_t::Init() {
	HwInit();
#ifndef I2C_POLL_ONLY
    // ==== Init manager ====
    RIndx = 0;
    WIndx = 0;
    // Reset stage of all commands
    for (uint8_t i=0; i<I2C_CMD_QUEUE_LENGTH; i++) Cmd[i] = 0; // Reset all pointers
#endif
}

#ifndef I2C_POLL_ONLY
void i2cMgr_t::AddCmd(I2C_Cmd_t *PCmd) {
    //Uart.Printf("WIndx: %u\r", WIndx);
    PCmd->State = CmdPending;
    Cmd[WIndx] = PCmd; // Add PCmd to buffer
    //Uart.Printf("Cmd Addr: %X\r", PCmd->Address);
    // Process command queue
    if (++WIndx >= I2C_CMD_QUEUE_LENGTH) WIndx = 0;
}

void i2cMgr_t::GetNext() {
    Cmd[RIndx] = 0; // Zero current pointer to disallow repeatable Cmd execution
    RIndx++;
    if (RIndx >= I2C_CMD_QUEUE_LENGTH) RIndx = 0;
}

#endif
// =========================== Polling functions ===============================
// Wait for Idle Bus
uint8_t i2cMgr_t::BusyWait() {
	uint32_t FTmr;
	Delay.Reset(&FTmr);
	uint16_t ISR2;
	do {
		ISR2 = I2C1->SR2;
		if(Delay.Elapsed(&FTmr, 4)) {
            Uart.Printf("I2C Busy: %04X\r", ISR2);
            return I2C_ERR_TIMEOUT;
		}
	} while(ISR2 & 0x0002);
    return I2C_OK;
}
uint8_t i2cMgr_t::SendStart(void) {
    I2C_GenerateSTART(I2C1, ENABLE);
    uint32_t FTmr;
    Delay.Reset(&FTmr);
    uint16_t ISR1;
    do {
        ISR1 = I2C1->SR1;
        if(Delay.Elapsed(&FTmr, 4)) {
            Uart.Printf("S evt: %04X\r", ISR1);
            return I2C_ERR_NOMASTER;
        }
    } while (!(ISR1 &0x0001));	// Start condition generated
    return I2C_OK;
}
uint8_t i2cMgr_t::SendAddrTXPoll(uint8_t AAddr) {
    //Uart.Printf("I2C TX Addr: %X\r", AAddr);
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

uint8_t i2cMgr_t::CmdPoll(I2C_Cmd_t ACmd) {
	//Uart.Printf("i2c Cmd Poll\r");
	while(BusyWait()) HwInit();
	if(ACmd.BufToWrite.Length == 0) return I2C_OK;
	// ==== First stage ====
    Uart.Printf("i2c write: %A\r", ACmd.BufToWrite.P, ACmd.BufToWrite.Length);
    while(SendStart() != I2C_OK) HwInit();
	uint8_t rslt=I2C_OK;
    uint32_t IEvt, ITimeout;
    uint8_t i, b;
	rslt = SendAddrTXPoll(ACmd.Address);
	i = 0;
	while((i < ACmd.BufToWrite.Length) and (rslt == I2C_OK)) {
		b = ACmd.BufToWrite.P[i];
		//Uart.Printf("%X\r", b);
		I2C_SendData(I2C1, b);
		ITimeout = I2C_TIMEOUT;
		do {	// Check if transmitted
			IEvt = I2C_GetLastEvent(I2C1);
			if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
				Uart.Printf("\rI2C Slave NACK\r");
				rslt = I2C_ERR_SLAVE_NACK;
				break;
			}
			if(ITimeout-- == 0) {
				rslt = I2C_ERR_TIMEOUT;
				break;
			}
		} while(IEvt != I2C_EVENT_MASTER_BYTE_TRANSMITTED);
		i++;
	} // while
	if(rslt != I2C_OK) return rslt;

	// ==== Second stage ====
	if(ACmd.BufToRead.Length == 0) {
		I2C_GenerateSTOP(I2C1, ENABLE);
		return I2C_OK;
	}
	else {
		//Uart.Printf("i2c read %u\r", ACmd.DataToRead.Length);
		if((rslt = SendStart()) == I2C_OK) {
			if((rslt = SendAddrRXPoll(ACmd.Address)) == I2C_OK) {
				// Act accordingly to number of bytes to receive
				if(ACmd.BufToRead.Length == 1) {		// 1 byte to read
					I2C_AcknowledgeConfig(I2C1, DISABLE);
					I2C_GenerateSTOP(I2C1, ENABLE);
					// Wait for the byte to be received
					ITimeout = I2C_TIMEOUT;
					while(1) {	// Check if received
						if(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == SET) {
							ACmd.BufToRead.P[0] = I2C_ReceiveData(I2C1);
							break;
						}
						if(ITimeout-- == 0) {
							rslt = I2C_ERR_TIMEOUT;
							break;
						}
					} // while
				} // if 1
				else if(ACmd.BufToRead.Length == 2) {	// 2 bytes to read
					I2C_AcknowledgeConfig(I2C1, DISABLE);
					// Wait for the two bytes to be received
					ITimeout = I2C_TIMEOUT;
					while(1) {
						IEvt = I2C_GetLastEvent(I2C1);
						if(IEvt & I2C_FLAG_BTF) {
							I2C_GenerateSTOP(I2C1, ENABLE);
							ACmd.BufToRead.P[0] = I2C_ReceiveData(I2C1);
							ACmd.BufToRead.P[1] = I2C_ReceiveData(I2C1);
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
					while((i < ACmd.BufToRead.Length) and (rslt == I2C_OK)) {
						ITimeout = I2C_TIMEOUT;
						while(1) {
							IEvt = I2C_GetLastEvent(I2C1);
							if(i == ACmd.BufToRead.Length-3) {	// Catch BTF
								if(IEvt & I2C_FLAG_BTF) {
									__disable_irq();
									I2C_AcknowledgeConfig(I2C1, DISABLE);
									ACmd.BufToRead.P[i] = I2C_ReceiveData(I2C1);
									I2C_GenerateSTOP(I2C1, ENABLE);
									i++;
									ACmd.BufToRead.P[i] = I2C_ReceiveData(I2C1);
									__enable_irq();
									break;
								}
							}
							else if(IEvt & I2C_FLAG_RXNE) {
								ACmd.BufToRead.P[i] = I2C_ReceiveData(I2C1);
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

    //if(rslt == I2C_OK) Uart.Printf("Done\r");
	return rslt;
}
