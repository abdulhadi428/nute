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
enum KeyStatus_t {keyReleased=0, keyPressed=1};
extern KeyStatus_t Key[KEY_COUNT];

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
    void Transmit(uint8_t PwrPercent, uint16_t Data);
};
extern IR_t IR;

// ==== Pills ====
#define PILL_COUNT_MAX  8   // max count of simultaneously connected pills
#define PILL_BUF_SZ     6   // Number bytes in inner buffer. Also used to determine packet size. Do not set large numbers; this will slow down communication between S & N bridges.
enum PillStatus_t {psIdle=0, psBusy=1, psNotConnected=2, psError=3};
struct Pill_t {
    PillStatus_t Status;
    uint8_t Address;
    uint8_t Data[PILL_BUF_SZ];
    void Read(uint16_t MemAddress, uint32_t DataSize);  // Data will be placed in PReadBuf
    void Write(uint16_t MemAddress, uint32_t DataSize, uint8_t *PData);
};
extern Pill_t Pill[PILL_COUNT_MAX];

#endif /* PERIPHERAL_H_ */
