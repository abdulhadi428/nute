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

// Inner use
#define EVTMASK_PN_NEW_PKT      EVENT_MASK(27)

#endif /* EVT_MASK_H_ */
