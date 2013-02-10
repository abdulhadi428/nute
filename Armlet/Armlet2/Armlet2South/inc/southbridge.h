/*
 * bridge.h
 *
 *  Created on: 06.02.2013
 *      Author: kreyl
 */

#ifndef BRIDGE_H_
#define BRIDGE_H_

#define SB_BAUDRATE     115200

#include "ch.h"
#include "kl_lib_f0.h"

#define SB_DMA_STREAM   STM32_DMA1_STREAM2
#define SB_UART_RX_IRQ  STM32_USART1_HANDLER

#define SB_UART_RX_REG  USART1->RDR

// ==================================== Commands ===============================
#define CMD_NONE                0x00
#define CMD_ADDITIONAL_DATA     0x01
// North To South Bridge commands
#define NTS_VIBRO               0x10
#define NTS_BEEP                0x20
#define NTS_IR_TRANSMIT         0x30
#define NTS_PILL_READ           0x40
#define NTS_PILL_WRITE          0x41

// South To North Bridge commands
#define STN_ATR                 0x70
#define STN_KEY_STATUS          0x80
#define STN_PWR_STATUS          0x90
#define STN_IR_IDLE             0xA0
#define STN_IR_RECEPTION        0xB0
#define STN_PILL_STATUS         0xC0
#define STN_PILL_DATA           0xC1

// =================================== Types ===================================
class Bridge_t {
private:
public:
    void Init();
};
extern Bridge_t Bridge;

#endif /* BRIDGE_H_ */
