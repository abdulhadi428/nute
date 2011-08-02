/*
 * File:   cc2500.h of NeldeCalma
 * Author: Laurelindo
 *
 * Created on 11 Декабрь 2009 г., 2:08
 */

#ifndef _CC2500_H
#define	_CC2500_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "cc2500defins.h"
#include "cc2500_rf_settings.h"

// ==================== Application-specific defins ============================
// Channels to use
#define CC_CHANNEL      0

// ================================== Defins ===================================
// Ports
#define CC_DDR  DDRD
#define CC_PORT PORTD
#define CC_PIN  PIND

#define CC_GDO0 PD2
#define CC_CS   PD7
#define CC_MOSI PD1
#define CC_MISO PD0
#define CC_SCLK PD4

// =========================== Pseudo functions ================================
#define CC_SCLK_LO()        CC_PORT &= ~(1<<CC_SCLK)
#define CC_CS_HI()          CC_PORT |=  (1<<CC_CS)
#define CC_CS_LO()          CC_PORT &= ~(1<<CC_CS)
#define CC_GDO0_IRQ_ENABLE( )   EIMSK |=  (1<<INT0)
#define CC_GDO0_IRQ_DISABLE( )  EIMSK &= ~(1<<INT0)

#define CC_GDO0_IS_HI()     bit_is_set(CC_PIN, CC_GDO0)
#define CC_MISO_IS_HI()     bit_is_set(CC_PIN, CC_MISO)

// =============================== Variables ===================================
#define CC_PKT_DATA_LENGTH  4
struct CC_Packet_t {
    uint8_t R, G, B, Uv;
    uint8_t RSSI;
    uint8_t LQI;
};

class CC_t {
private:
    uint16_t Timer;
    uint8_t State;
    uint8_t ReadWriteByte(uint8_t AByte);
    // Inner methods
    void WriteBurst(uint8_t ARegAddr, uint8_t *PData, uint8_t ALength);
    void WriteTX (uint8_t *PData, uint8_t ALength) { WriteBurst(CC_FIFO, PData, ALength); }
    void ReadRX  (uint8_t *PData, uint8_t ALength);
    void RfConfig(void);
    uint8_t ReadRegister (uint8_t ARegAddr);
    void WriteRegister (uint8_t ARegAddr, uint8_t AData);
    void WriteStrobe (uint8_t AStrobe);
public:
    bool NewPacketReceived;
    bool IsSleeping;
    CC_Packet_t RX_Pkt, TX_Pkt;
    void Init(void);
    void Task(void);
    void SetChannel(uint8_t AChannel);
    void SetAddress(uint8_t AAdress) { WriteRegister(CC_ADDR, AAdress); }
    // Strobes
    void Reset(void)       { WriteStrobe(CC_SRES);  }
    void EnterTx(void)     { WriteStrobe(CC_STX);   }
    void EnterRx(void)     { WriteStrobe(CC_SRX);   }
    void EnterIdle(void)   { WriteStrobe(CC_SIDLE); }
    void PowerDown(void)   { WriteStrobe(CC_SPWD);  }
    void Calibrate(void)   { WriteStrobe(CC_SCAL);  }
    void FlushRxFifo(void) { WriteStrobe(CC_SFRX);  }
    void FlushTxFifo(void) { WriteStrobe(CC_SFTX);  }
    void GetState(void)    { WriteStrobe(CC_SNOP);  }
};

extern CC_t CC;


#endif
