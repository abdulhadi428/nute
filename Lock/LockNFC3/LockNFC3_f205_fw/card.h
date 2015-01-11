/*
 * card.h
 *
 *  Created on: 11 џэт. 2015 у.
 *      Author: Kreyl
 */

#ifndef CARD_H_
#define CARD_H_

enum CardState_t {csCardOk, csCardOut, csCardError, csCardHalted};

class Card_t {
private:

public:
    CardState_t State = csCardOut;
    uint8_t *Data;
    uint32_t DataSz;

};

extern Card_t Card;

#endif /* CARD_H_ */
