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
struct VibroChunk_t {
    int8_t Intencity;  // 0 means off, 1...100 means intencity, -1 means end
    uint16_t Time_ms;
} PACKED;
// Send commands to vibro with this function
void Vibro(const VibroChunk_t *PSequence);

// ==== Beep ====

struct BeepChunk_t {
    int8_t VolumePercent;   // 0 means silence, 1...100 means volume, -1 means end
    uint16_t Time_ms;
    uint16_t Freq_Hz;
} PACKED;
#define BEEP_CHUNK_SZ   sizeof(BeepChunk_t)
// Send commands to beep with this function
void Beep(const BeepChunk_t *PSequence);

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
/* Number bytes in inner buffer. Also used to determine packet size.
 * See struct SbPkt_t in southbridge.h for more info about size.
 * Do not set large numbers; this will slow down communication between S & N bridges.
 */
#define PILL_BUF_SZ     4

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
