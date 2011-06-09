/*
 * File:   vs.h
 * Author: Kreyl
 *
 * Created on June 9, 2011, 11:46 AM
 */

#ifndef VS_H
#define	VS_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"

// ==== Defines ====
#define VS_XCS          GPIO_Pin_0
#define VS_XDCS         GPIO_Pin_4
#define VS_RST          GPIO_Pin_15
#define VS_DREQ         GPIO_Pin_1
#define VS_XCLK         GPIO_Pin_5
#define VS_SO           GPIO_Pin_6
#define VS_SI           GPIO_Pin_7

// Constants
#define VS_TIMEOUT  0xFFFF

// Command codes
#define VS_READ_OPCODE  0b00000011
#define VS_WRITE_OPCODE 0b00000010

// ==== Error codes ====
#define VS_OK       0
#define VS_BUSY     1
#define VS_TIMEOUT  2

// Types
typedef struct {
    uint8_t Instruction;
    uint8_t Address;
    uint16_t Data;
} VSCmd_t;

class VS_t {
private:
    void Rst_Lo(void) { GPIOA->BRR  = VS_RST; }
    void Rst_Hi(void) { GPIOA->BSRR = VS_RST; }
    void XCS_Lo(void) { GPIOA->BRR  = VS_XCS; }
    void XCS_Hi(void) { GPIOA->BSRR = VS_XCS; }
    void XDCS_Lo(void) { GPIOA->BRR  = VS_XDCS; }
    void XDCS_Hi(void) { GPIOA->BSRR = VS_XDCS; }

    bool IsBusy(void) { return (GPIO_ReadInputDataBit(GPIOA, VS_DREQ) == Bit_RESET);  }
    uint8_t BusyWait(void);

    void Enable(void);
    void Disable(void);

    void AmplifierOn(void);
    void AmplifierOff(void);

    void WriteByte(uint8_t AByte);
    uint8_t ReadByte(void);
public:
    void Init(void);
    uint8_t CmdRead(uint8_t AAddr, uint16_t *AData);
};

extern VS_t Vs;

#endif	/* VS_H */

