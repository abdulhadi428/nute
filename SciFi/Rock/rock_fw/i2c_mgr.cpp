#include "i2c_mgr.h"
#include "stm32f10x_dma.h"

#include "uart.h"
#include "delay_util.h"


i2cMgr_t i2cMgr;

void i2cMgr_t::Init() {
    // ==== Init I2C ====
    // Init clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);    // I2C clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   // Peripheral clock
    // Init GPIO - SCL & SDA
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // I2C
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x01;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);

    // ==== Init manager ====
    IsBusy = false;
    CmdToWrite = &Commands[0];
    CmdToRead  = &Commands[0];
    // Reset stage of all commands
    for (uint8_t i=0; i<I2C_CMD_QUEUE_LENGTH; i++) Commands[i].State = CmdSucceded;

    // ==== Init DMA ====
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//    DMA_DeInit(I2C_DMA_CHNL);
//    // ==== Init NVIC ====
//    NVIC_InitTypeDef NVIC_InitStructure;
//    // Enable DMA1 channel3 IRQ Channel
//    NVIC_InitStructure.NVIC_IRQChannel = I2C_DMA_IRQ;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//    // Enable DMA1 Channel Transfer Complete interrupt
//    DMA_ITConfig(I2C_DMA_CHNL, DMA_IT_TC, ENABLE);
}

void i2cMgr_t::AddCmd(I2C_Cmd_t ACmd) {
    ACmd.State = CmdPending;
    *CmdToWrite = ACmd;     // Copy command to buffer
    // Process command queue
    if (!IsBusy) CmdToRead = CmdToWrite;
    CmdToWrite++;
    if ((CmdToWrite - &Commands[0]) >= I2C_CMD_QUEUE_LENGTH) CmdToWrite = &Commands[0];
    // Process command
    if (!IsBusy) ProcessCmd();
}

void i2cMgr_t::WriteBufferNoDMA(uint8_t AAddr, uint8_t* ABuffer, uint8_t ABufferSize) {
    Uart.PrintString("I2C wb\r");
    uint32_t IEvt;
    uint8_t b;
    // Send Start and Address as transmitter
    if (SendStart()) return;
    if (SendAddrTX(AAddr)) return;
    // Address was acknowledged, start transmission
    for (uint8_t i=0; i<ABufferSize; i++) {
        b = *ABuffer++;
        Uart.PrintAsHex(b);
        Uart.Print(' ');
        I2C_SendData(I2C1, b);
        do {
            IEvt = I2C_GetLastEvent(I2C1);
            if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
                Uart.PrintString("\rI2C Slave NACK\r");
                I2C_GenerateSTOP(I2C1, ENABLE);
                return;
            }
        } while(IEvt != I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    } // for
    I2C_GenerateSTOP(I2C1, ENABLE);
    Uart.PrintString("\rI2C write completed\r");
}

void i2cMgr_t::ProcessCmd() {
    CmdToRead->State = CmdFailed;
    if (SendStart()) return;
    if (SendAddrTX(CmdToRead->Address)) return;
    // Address was sent and was acknowledged, write or read data
    if (CmdToRead->DataToWrite.Length != 0) {
        PrepareToWrite();
        return;
    }
    else if (CmdToRead->DataToRead.Length != 0) {
        PrepareToRead();
        return;
    }
    // If no data is to be read or written, send STOP
    StopAndGetNext();
}

void i2cMgr_t::PrepareToWrite() {
    CmdToRead->State = CmdWriting;
    IsBusy = true;
    // If more than one byte to send, use DMA
    if (CmdToRead->DataToWrite.Length > 1) {
        // Prepare DMA
        DMA_DeInit(I2C_DMA_CHNL);
        DMA_InitTypeDef DMA_InitStructure;
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &I2C1->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) &CmdToRead->DataToWrite.Buf[0];
        DMA_InitStructure.DMA_BufferSize         = CmdToRead->DataToWrite.Length;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;  // From memory to I2C
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(I2C_DMA_CHNL, &DMA_InitStructure);
        // Start transmission
        I2C_DMACmd(I2C1, ENABLE);       // Enable DMA
        DMA_Cmd(I2C_DMA_CHNL, ENABLE);  // Enable DMA channel
    }
    else {
        I2C_SendData(I2C1, CmdToRead->DataToWrite.Buf[0]);
        uint32_t FTimeout = I2C_TIMEOUT;
        while (I2C_GetFlagStatus(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == RESET) {
            if ((FTimeout--) == 0) {
                IsBusy = false;
                Uart.PrintString("I2C can't transmit single byte\r");
                return;
            }
        } // while
        StopAndGetNext();
    } // if 1 byte
}
void i2cMgr_t::PrepareToRead() {
    CmdToRead->State = CmdFailed;
    IsBusy = true;
    if (SendStart()) return;                    // Send Repeated Start
    if (SendAddrRX(CmdToRead->Address)) return; // Send device address
    // Repeated Start and Addr are sent successfully, start reading
    CmdToRead->State = CmdReading;
    // If more than one byte to receive, use DMA
    if (CmdToRead->DataToRead.Length > 1) {
        // Prepare DMA
        DMA_DeInit(I2C_DMA_CHNL);
        DMA_InitTypeDef DMA_InitStructure;
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &I2C1->DR;
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t) &CmdToRead->DataToRead.Buf[0];
        DMA_InitStructure.DMA_BufferSize         = CmdToRead->DataToRead.Length;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;  // From I2C to memory
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_Init(I2C_DMA_CHNL, &DMA_InitStructure);
        //Inform the DMA that the next End Of Transfer Signal will be the last one, need to send NACK after last byte
        I2C_DMALastTransferCmd(I2C1, ENABLE);
        // Start transmission
        I2C_DMACmd(I2C1, ENABLE);       // Enable DMA
        DMA_Cmd(I2C_DMA_CHNL, ENABLE);  // Enable DMA channel
    }
    else {
        // Disable Acknowledgement
        I2C_AcknowledgeConfig(I2C1, DISABLE);
        I2C_GenerateSTOP(I2C1, ENABLE);
        // Wait for the byte to be received
        uint32_t FTimeout = I2C_TIMEOUT;
        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET) {
            if ((FTimeout--) == 0) {
                IsBusy = false;
                Uart.PrintString("I2C can't receive single byte\r");
                return;
            }
        } // while
        StopAndGetNext();
    } // if 1 byte
}

void i2cMgr_t::StopAndGetNext() {
    I2C_GenerateSTOP(I2C1, ENABLE);
    IsBusy = false;
    // Get next command
    CmdToRead++;
    if ((CmdToRead - &Commands[0]) >= I2C_CMD_QUEUE_LENGTH) CmdToRead = &Commands[0];
    // Check if processing needed
    if (CmdToRead->State == CmdPending) ProcessCmd();
}


// ================================ Low level ==================================
uint8_t i2cMgr_t::SendStart(void) {
    // Wait for Idle Bus
    uint32_t FTimeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) if((FTimeout--) == 0) return 1;

    I2C_GenerateSTART(I2C1, ENABLE);
    FTimeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        if ((FTimeout--) == 0) {
            IsBusy = false;
            Uart.PrintString("I2C can't enter master mode\r");
            return 2;
        }
    }
    return 0;
}
uint8_t i2cMgr_t::SendAddrTX(uint8_t AAddr) {
    uint32_t IEvt;
    I2C_Send7bitAddress(I2C1, (AAddr << 1), I2C_Direction_Transmitter);
    uint32_t FTimeout = I2C_TIMEOUT;
    do {
        IEvt = I2C_GetLastEvent(I2C1);
        if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
            IsBusy = false;
            Uart.PrintString("I2C Slave Addr NACK\r");
            return 1;
        }
        if ((FTimeout--) == 0) {
            IsBusy = false;
            Uart.PrintString("I2C Slave Addr Timeout\r");
            return 2;
        }
    } while (IEvt != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    return 0;   // all right
}
uint8_t i2cMgr_t::SendAddrRX(uint8_t AAddr) {
    uint32_t IEvt;
    I2C_Send7bitAddress(I2C1, AAddr, I2C_Direction_Receiver);
    uint32_t FTimeout = I2C_TIMEOUT;
    do {
        IEvt = I2C_GetLastEvent(I2C1);
        if (IEvt & I2C_EVENT_SLAVE_ACK_FAILURE) {   // NACK occured, slave doesn't answer
            IsBusy = false;
            Uart.PrintString("I2C Slave Addr NACK\r");
            return 1;
        }
        if ((FTimeout--) == 0) {
            IsBusy = false;
            Uart.PrintString("I2C Slave Addr Timeout\r");
            return 2;
        }
    } while (IEvt != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    return 0;   // all right
}

// ============================ Interrupts =====================================
// DMA interrupt of end of transmission

void DMA1_Channel5_IRQHandler(void) {
    // Test on DMA1 Channel5 Transfer Complete interrupt
    if (DMA_GetITStatus(DMA1_IT_TC5)) {
        // Clear DMA1 Channel5 Half Transfer, Transfer Complete and Global interrupt pending bits
        DMA_ClearITPendingBit(DMA1_IT_GL5);
        DMA_DeInit(I2C_DMA_CHNL); // Disable DMA
        // Check what to do
        if (i2cMgr.CmdToRead->State == CmdWriting) {
            // Check if read needed
            if (i2cMgr.CmdToRead->DataToRead.Length != 0) {
                i2cMgr.PrepareToRead();
                return;
            }
        }
        i2cMgr.CmdToRead->State = CmdSucceded;
        i2cMgr.StopAndGetNext();
    } // if IT_TC3
}
