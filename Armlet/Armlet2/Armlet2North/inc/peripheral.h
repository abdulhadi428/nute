/*
 * peripheral.h
 *
 *  Created on: 18.01.2013
 *      Author: kreyl
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include <stdint.h>
#include "kl_lib_f2xx.h"

// ==== Keys ====
#define KEY_COUNT   9
extern bool KeyPressed[KEY_COUNT];

// ==== Power ====
struct PwrStatus_t {
    bool ExternalPwrOn;
    bool IsCharging;
    uint16_t Voltage_mV;
} PACKED;
extern PwrStatus_t PwrStatus;

// ==== Vibro ====
enum VibroAction_t {vaStop=0, vaFlinch=1};
struct VibroCmd_t {
    VibroAction_t Action;
    uint32_t Time_ms;
} PACKED;
// Send commands to vibro with this function
void VibroCmd(uint32_t CmdCount, VibroCmd_t *PCmds);

// ==== Beep ====
enum BeepAction_t {baSilence=0, baBeep=1};
struct BeepCmd_t {
    BeepAction_t Action;
    uint32_t Time_ms;
    uint16_t Freq_Hz;
    uint8_t VolumePercent;
} PACKED;
// Send commands to beep with this function
void BeepCmd(uint32_t CmdCount, BeepCmd_t *PCmds);

// ==== Infrared ====
enum IRStatus_t {irIdle=0, irTransmitting=1, irReceiving=2};
#define IR_RXBUF_SZ     27
#define IR_TXBUF_SZ     27
struct IR_t {
    IRStatus_t Status;
    uint8_t RxBuf[IR_RXBUF_SZ];
    uint32_t RxSize;
    void Transmit(uint8_t PwrPercent, uint32_t DataSz, uint8_t *PData);
};
extern IR_t IR;

// ==== Pills ====
#define PILL_COUNT_MAX  8   // max count of simultaneously connected pills
enum PillStatus_t {psIdle=0, psBusy=1, psNotConnected=2, psError=3};
struct Pill_t {
    PillStatus_t Status;
    uint8_t Address;
    uint8_t *PReadBuf;
    void Read(uint32_t MemAddress, uint32_t DataSize);  // Data will be placed in PReadBuf
    void Write(uint32_t MemAddress, uint32_t DataSize, uint8_t *PData);
};
extern Pill_t Pill[PILL_COUNT_MAX];

// ==== SouthBridge class ====
#define SB_GPIO     GPIOC
#define SB_RST      5
#define SB_IN       6
#define SB_OUT      7
#define SB_UART     USART6
#define SB_BAUDRATE 100000
enum SbStatus_t {sbsOn, sbsOff, sbsError};

class SouthBridge_t {
private:

public:
    //Thread *itp;    // need for IRQ
    SbStatus_t Status;
    uint32_t FwVersion;
    void Init();
    void On();
    void Shutdown();
};
extern SouthBridge_t SouthBridge;

// North To South Bridge commands
#define NTS_KEY_GET_STATUS      10
#define NTS_PWR_GET_STATUS      20
#define NTS_VIBRO_FLINCH        30
#define NTS_BEEP                40
#define NTS_IR_TRANSMIT         50
#define NTS_PILL_STATUS         60
#define NTS_PILL_READ           61
#define NTS_PILL_WRITE          62

// South To North Bridge commands
#define STN_ATR                 100
#define STN_KEY_STATUS          110
#define STN_PWR_STATUS          120
#define STN_IR_STATUS           150
#define STN_IR_RECEPTION        151
#define STN_PILL_INFO           160


#endif /* PERIPHERAL_H_ */
