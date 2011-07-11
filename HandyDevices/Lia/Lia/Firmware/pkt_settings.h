/* 
 * File:   pkt_settings.h
 * Author: Kreyl Laurelindo
 *
 * Created on 16 Ноябрь 2010 г., 23:40
 *
 * Here lie settings and types of packet used, and any IDs used, too.
 */

#ifndef PKT_SETTINGS_H
#define	PKT_SETTINGS_H

// ============================= Defines =======================================
#define CC_PKT_LENGTH       4
#define CC_PKT_DATA_LENGTH  CC_PKT_LENGTH-2

#define PKT_ID_CALL     0xCA

// ============================== Types ========================================
struct CC_Packet_t {
    uint8_t Address;
    uint8_t CommandID;
    uint8_t Data[CC_PKT_DATA_LENGTH];
    uint8_t RSSI;
    uint8_t LQI;
};



#endif	/* PKT_SETTINGS_H */

