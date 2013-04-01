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

// ================================== Power ====================================
struct PwrStatus_t {
    bool ExternalPwrOn;
    bool IsCharging;
    uint16_t Voltage_mV;
} PACKED;
extern PwrStatus_t PwrStatus;

// ==================================== Vibro ==================================
struct VibroChunk_t {
    int8_t Intencity;  // 0 means off, 1...100 means intencity, -1 means end
    uint16_t Time_ms;
} PACKED;
#define VIBRO_CHUNK_SZ   sizeof(VibroChunk_t)
class Vibrator_t {
private:
    PwmPin_t IPin;
    VirtualTimer ITmr;
public:
    void Vibrate(const VibroChunk_t *PSequence); // Vibrate with this function
    void Init();
};
extern Vibrator_t Vibro;

// ==================================== Beep ===================================
struct BeepChunk_t {
    int8_t VolumePercent;   // 0 means silence, 1...100 means volume, -1 means end
    uint16_t Time_ms;
    uint16_t Freq_Hz;
} PACKED;
#define BEEP_CHUNK_SZ   sizeof(BeepChunk_t)
class Beeper_t {
private:
    PwmPin_t IPin;
    VirtualTimer ITmr;
public:
    void Beep(const BeepChunk_t *PSequence); // Beep with this function
    void Init();
};
extern Beeper_t Beeper;

// ================================= Pin control ===============================
#define PIN_GPIO    GPIOD
#define PIN_NUMBER  14
class Pin_t {
private:
    VirtualTimer ITmr;
public:
    void Init() { PinSetupOut(PIN_GPIO, PIN_NUMBER, omPushPull); }
    void High() { PinSet(PIN_GPIO, PIN_NUMBER); }
    void Low()  { PinClear(PIN_GPIO, PIN_NUMBER); }
    void Pulse(uint32_t ms);
};

extern Pin_t Pin;

// =================================== Pills ===================================
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
