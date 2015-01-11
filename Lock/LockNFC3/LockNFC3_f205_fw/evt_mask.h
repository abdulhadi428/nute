/*
 * evt_mask.h
 *
 *  Created on: Apr 12, 2013
 *      Author: g.kruglov
 */

#ifndef EVT_MASK_H_
#define EVT_MASK_H_

// Event masks
#define EVTMSK_UART_RX_POLL     EVENT_MASK(1)
#define EVTMSK_USB_READY        EVENT_MASK(2)
#define EVTMASK_PLAY_ENDS       EVENT_MASK(3)
#define EVTMASK_CARD_APPEARS    EVENT_MASK(4)
#define EVTMASK_CARD_DISAPPEARS EVENT_MASK(5)
#define EVTMSK_KEYS             EVENT_MASK(6)
// Indication
#define EVTMASK_DOOR_OPEN       EVENT_MASK(7)
#define EVTMASK_DOOR_SHUT       EVENT_MASK(8)
#define EVTMASK_BAD_KEY         EVENT_MASK(9)

// Inner use
#define EVTMASK_PN_NEW_PKT      EVENT_MASK(27)
#define EVTMASK_PN_TX_COMPLETED EVENT_MASK(28)
#define EVTMASK_PN_RX_COMPLETED EVENT_MASK(29)

#endif /* EVT_MASK_H_ */
