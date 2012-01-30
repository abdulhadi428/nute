/* 
 * File:   cc_rf_settings.h
 * Author: Kreyl Laurelindo
 *
 * Created on 7 Март 2010 г., 12:42
 */

#ifndef _CC_RF_SETTINGS_H
#define	_CC_RF_SETTINGS_H

// Bitrate
//#define CC_BITRATE_2K4
#define CC_BITRATE_10K
//#define CC_BITRATE_250K

// ============================ Common use values ==============================
#define CC_PKT_LENGTH       6   // for this exact application
#define CC_TX_FIFO_SIZE     33
#define CC_RX_FIFO_SIZE     32

// =================================== Power ===================================
//#define CC_PATABLE0_VALUE   0x00    // -55 dBm
#define CC_PATABLE0_VALUE   0x50    // -30 dBm
//#define CC_PATABLE0_VALUE   0x44    // -28 dBm
//#define CC_PATABLE0_VALUE   0x46    // -20 dBm
//#define CC_PATABLE0_VALUE   0xFE    // 0 dBm
//#define CC_PATABLE0_VALUE   0xFF    // +1 dBm


// ================================= Frequency =================================
#define CC_FREQ2_VALUE      0x5D        // Frequency control word, high byte.
#define CC_FREQ1_VALUE      0x93        // Frequency control word, middle byte.
#define CC_FREQ0_VALUE      0xB1        // Frequency control word, low byte.

// =================================== Common ==================================
#define CC_MDMCFG1_VALUE    0b10100010  // FEC=1, Preamble length=010 => 4bytes, Ch spacing=10=2
//#define CC_MDMCFG1_VALUE    0b00100010  // FEC=0, Preamble length=010 => 4bytes, Ch spacing=10=2
//#define CC_MDMCFG1_VALUE    0b11000010  // FEC=1, Preamble length=100 => 8bytes, Ch spacing=10=2

//#define CC_MCSM0_VALUE      0x18        // Calibrate at IDLE->RX,TX
#define CC_MCSM0_VALUE      0x28        // Calibrate at RX->IDLE, TX->IDLE
#define CC_MCSM1_VALUE      0b00001100  // Channel is always clear, RX->RX,   TX->IDLE
//#define CC_MCSM1_VALUE      0b00001111  // Channel is always clear, RX->RX,   TX->RX
//#define CC_MCSM1_VALUE      0b00000000  // Channel is always clear, RX->IDLE, TX->IDLE
//#define CC_MCSM1_VALUE      0b00000011  // Channel is always clear, RX->IDLE, TX->RX

#define CC_FIFOTHR_VALUE    0b00000111  // RXFIFO and TXFIFO thresholds: TX 33, RX 32
#define CC_IOCFG2_VALUE     0x0E        // GDO2 - Carrier sence
#define CC_IOCFG0_VALUE     0x06        // GDO0 - Asserts when sync word has been sent / received, and de-asserts at the end of the packet. In RX, the pin will de-assert
                                        // when the optional address check fails or the RX FIFO overflows.
// *** ! *** 0x07 cannot be used with CC2400=0. Use 0x06 instead. (CC2400=1 makes data whitening impossible).
//define CC_IOCFG0_VALUE     0x07        // GDO0 - Asserts when a packet has been received with CRC OK. De-asserts when the first byte is read from the RX FIFO.

//#define CC_PKTCTRL1_VALUE   0b00000110  // PQT=0, CRC autoflush=0, Append=1, Address check = 10 (check, 0 is broadcast)
#define CC_PKTCTRL1_VALUE   0b00001110  // PQT=0, CRC autoflush=1, Append=1, Address check = 10 (check, 0 is broadcast)
#define CC_PKTCTRL0_VALUE   0b01000100  // WhiteData=1, PKTFormat=normal, CC2400_EN=0, CRC enabled, Fixed Length
//#define CC_PKTCTRL0_VALUE   0b00000100  // WhiteData=0, PKTFormat=normal, CC2400_EN=0, CRC enabled, Fixed Length
#define CC_ADDR_VALUE       0x01        // Device address.
#define CC_PKTLEN_VALUE     CC_PKT_LENGTH  // Packet length.


// ========================= Bitrate-specific ==================================
#ifdef CC_BITRATE_10K
#define CC_FSCTRL1_VALUE    0x06        // Frequency synthesizer control: IF - RF studio
#define CC_FSCTRL0_VALUE    0x00        // Frequency synthesizer control: freq offset

#define CC_MDMCFG4_VALUE    0x78        // Modem configuration: channel bandwidth
#define CC_MDMCFG3_VALUE    0x93        // Modem configuration.
#define CC_MDMCFG2_VALUE    0x03        // Filter, modulation format, Manchester coding, SYNC_MODE=011 => 30/32 sync word bits
#define CC_MDMCFG0_VALUE    0xF8        // Modem configuration.

#define CC_DEVIATN_VALUE    0x44        // Modem deviation setting - RF studio
#define CC_FREND1_VALUE     0x56        // Front end RX configuration - RF studio
#define CC_FREND0_VALUE     0x10        // Front end TX configuration.

#define CC_MCSM2_VALUE      0b00000111

#define CC_FOCCFG_VALUE     0x16        // Frequency Offset Compensation Configuration - RF studio
#define CC_BSCFG_VALUE      0x6C        // Bit synchronization Configuration - RF studio
#define CC_AGCCTRL2_VALUE   0x43        // }
#define CC_AGCCTRL1_VALUE   0x40        // }
#define CC_AGCCTRL0_VALUE   0x91        // } AGC control: RF studio

#define CC_FSCAL3_VALUE     0xA9        // }
#define CC_FSCAL2_VALUE     0x0A        // }
#define CC_FSCAL1_VALUE     0x00        // }
#define CC_FSCAL0_VALUE     0x11        // } Frequency synthesizer calibration: RF studio

#define CC_FSTEST_VALUE     0x59        // Frequency synthesizer calibration: RF studio
#define CC_TEST2_VALUE      0x88        // Various test settings: RF studio
#define CC_TEST1_VALUE      0x31        // Various test settings: RF studio
#define CC_TEST0_VALUE      0x0B        // Various test settings: RF studio

#endif
// ================================ 2K4 ========================================
#ifdef CC_BITRATE_2K4
#define CC_FSCTRL1_VALUE    0x08        // Frequency synthesizer control: IF - RF studio
#define CC_FSCTRL0_VALUE    0x00        // Frequency synthesizer control: freq offset

#define CC_MDMCFG4_VALUE    0x86        // Modem configuration: channel bandwidth
#define CC_MDMCFG3_VALUE    0x83        // Modem configuration.
#define CC_MDMCFG2_VALUE    0x03        // Filter, modulation format, Manchester coding, SYNC_MODE=011 => 30/32 sync word bits
#define CC_MDMCFG0_VALUE    0xF8        // Modem configuration.

#define CC_DEVIATN_VALUE    0x44        // Modem deviation setting - RF studio
#define CC_FREND1_VALUE     0x56        // Front end RX configuration - RF studio
#define CC_FREND0_VALUE     0x10        // Front end TX configuration.

#define CC_MCSM2_VALUE      0b00000111

#define CC_FOCCFG_VALUE     0x16        // Frequency Offset Compensation Configuration - RF studio
#define CC_BSCFG_VALUE      0x6C        // Bit synchronization Configuration - RF studio
#define CC_AGCCTRL2_VALUE   0x03        // }
#define CC_AGCCTRL1_VALUE   0x40        // }
#define CC_AGCCTRL0_VALUE   0x91        // } AGC control: RF studio

#define CC_FSCAL3_VALUE     0xA9        // }
#define CC_FSCAL2_VALUE     0x0A        // }
#define CC_FSCAL1_VALUE     0x00        // }
#define CC_FSCAL0_VALUE     0x11        // } Frequency synthesizer calibration: RF studio

#define CC_FSTEST_VALUE     0x59        // Frequency synthesizer calibration: RF studio
#define CC_TEST2_VALUE      0x88        // Various test settings: RF studio
#define CC_TEST1_VALUE      0x31        // Various test settings: RF studio
#define CC_TEST0_VALUE      0x0B        // Various test settings: RF studio

#endif
// ============================== 250K =========================================
#ifdef CC_BITRATE_250K
#define CC_FSCTRL1_VALUE    0x0A        // Frequency synthesizer control: IF - RF studio
#define CC_FSCTRL0_VALUE    0x00        // Frequency synthesizer control: freq offset

#define CC_MDMCFG4_VALUE    0x2D        // Modem configuration: channel bandwidth
#define CC_MDMCFG3_VALUE    0x3B        // Modem configuration.
#define CC_MDMCFG2_VALUE    0x73        // Filter, modulation format, Manchester coding, SYNC_MODE=011 => 30/32 sync word bits
#define CC_MDMCFG0_VALUE    0xF8        // Modem configuration.

#define CC_DEVIATN_VALUE    0x00        // Modem deviation setting - RF studio
#define CC_FREND1_VALUE     0xB6        // Front end RX configuration - RF studio
#define CC_FREND0_VALUE     0x10        // Front end TX configuration.

#define CC_MCSM2_VALUE      0b00000111

#define CC_FOCCFG_VALUE     0x1D        // Frequency Offset Compensation Configuration - RF studio
#define CC_BSCFG_VALUE      0x1C        // Bit synchronization Configuration - RF studio
#define CC_AGCCTRL2_VALUE   0xC7        // }
#define CC_AGCCTRL1_VALUE   0x00        // }
#define CC_AGCCTRL0_VALUE   0xB0        // } AGC control: RF studio

#define CC_FSCAL3_VALUE     0xEA        // }
#define CC_FSCAL2_VALUE     0x0A        // }
#define CC_FSCAL1_VALUE     0x00        // }
#define CC_FSCAL0_VALUE     0x11        // } Frequency synthesizer calibration: RF studio

#define CC_FSTEST_VALUE     0x59        // Frequency synthesizer calibration: RF studio
#define CC_TEST2_VALUE      0x88        // Various test settings: RF studio
#define CC_TEST1_VALUE      0x31        // Various test settings: RF studio
#define CC_TEST0_VALUE      0x0B        // Various test settings: RF studio


#endif

// ********************

// Rare use settings
#define CC_SYNC1_VALUE      0xD3    // }
#define CC_SYNC0_VALUE      0x91    // } SYNC word to transmit

#define CC_CHANNR_VALUE     0x00        // Channel number.


#endif	/* _CC_RF_SETTINGS_H */

