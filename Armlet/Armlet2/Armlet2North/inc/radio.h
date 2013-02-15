/*
 * radio.h
 *
 *  Created on: Feb 15, 2013
 *      Author: g.kruglov
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "ch.h"

struct Contact_t {
    uint16_t Addr;
    uint8_t Channel;
    systime_t Timestamp;
} __attribute__ ((__packed__));
#define CONTACT_SZ  sizeof(Contact_t)

#define CONTACT_COUNT   99

enum RadioState_t {rIdle, rService};

class Radio_t {
private:
    Contact_t Contacts[CONTACT_COUNT];
    void ClearContacts() { for(uint32_t i=0; i<CONTACT_COUNT; i++) Contacts[i].Addr = 0; }

public:
    void Init();
};

extern Radio_t Radio;

#endif /* RADIO_H_ */
