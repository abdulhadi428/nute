/*
 * pn.h
 *
 *  Created on: 07.09.2011
 *      Author: g.kruglov
 */

#ifndef PN_H_
#define PN_H_

#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "kl_util.h"
#include "kl_lib.h"
#include "pn_defins.h"

typedef enum {csCardOk, csCardOut, csCardError, csCardHalted} CardState_t;

class Card_t {
public:
    CardState_t State;
    uint8_t *Data;
    uint16_t DataLength;
    uint64_t ID;
    bool ReadID(void);
};


enum PN_PktRslt_t {pnPktACK, pnPktErrFrame, pnPktOK, pnPktFail, pnPktCRC};

class PN_t {
private:
    uint32_t Timer;
    uint8_t Buf[264];   // 264 obtained from datasheet
    uint32_t Length;    // Payload data length
    void HardwareInit(void);
    void ResetLo(void) { GPIO_ResetBits(GPIOA, GPIO_Pin_2); }
    void ResetHi(void) { GPIO_SetBits(GPIOA, GPIO_Pin_2); }
    void NssLo(void)   { GPIO_ResetBits(GPIOA, GPIO_Pin_3); Delay.us(135); } // Delay allows PN to wakeup, do not remove it
    void NssHi(void)   { GPIO_SetBits(GPIOA, GPIO_Pin_3); }
    bool ReplyIsReady(void) { return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET); } // Low level means that reply is ready
    uint8_t WriteReadByte(uint8_t AByte);
    void WritePkt(void);
    PN_PktRslt_t WriteAndWaitReply(void);
    PN_PktRslt_t WriteAndWaitData(void);
    PN_PktRslt_t ReadPkt(void);
    // General
    PN_PktRslt_t Cmd(uint8_t ACmd, uint32_t ALength, ...);
    void FieldOn(void)  { Cmd(PN_CMD_RF_CONFIGURATION, 2, 0x01, 0x01); } // Cfg=0x01=RF field, value=0x01=FieldOn
    void FieldOff(void) { Cmd(PN_CMD_RF_CONFIGURATION, 2, 0x01, 0x00); } // Cfg=0x01=RF field, value=0x00=FieldOff
    // Initiator
    uint8_t InSelect(uint8_t ATagID);
    uint8_t InDeselect(uint8_t ATagID);
    uint8_t InATR(uint8_t ATagID);
    bool CardIsAppeared(void);
    bool CheckIfCardStillNear(void);
public:
    void Init(void);
    void Task(void);
    void SendData(uint8_t *ABuf, uint16_t ALen);
    bool MifareRead(uint8_t *ABuf, uint32_t AAddr); // Reads 16 bytes of Mifare data into ABuf
    // Events
    ftVoid_Void Evt_CardAppeared, Evt_CardDisappeared;
};

extern PN_t PN;
extern Card_t Card;

#endif /* PN_H_ */
