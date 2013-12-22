/*
 * evt_mask.h
 *
 *  Created on: Apr 12, 2013
 *      Author: g.kruglov
 */

#ifndef EVT_MASK_H_
#define EVT_MASK_H_

// Event masks
#define EVTMSK_NOTHING          0

#define EVTMSK_KEY_START        EVENT_MASK(0)
#define EVTMSK_KEY_TIME_UP      EVENT_MASK(1)
#define EVTMSK_KEY_TIME_DOWN    EVENT_MASK(2)
#define EVTMSK_KEY_CURRENT_UP   EVENT_MASK(3)
#define EVTMSK_KEY_CURRENT_DOWN EVENT_MASK(4)
#define EVTMSK_KEY_START_LONG   EVENT_MASK(5)

#define EVTMASK_NEWSECOND       EVENT_MASK(6)

#endif /* EVT_MASK_H_ */
