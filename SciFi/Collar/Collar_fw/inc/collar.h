/*
 * collar.h
 *
 *  Created on: 06.06.2012
 *      Author: kreyl
 */

#ifndef COLLAR_H_
#define COLLAR_H_

// All this is needed for radioexchange
#define COLSTATE_OK             0x00
#define COLSTATE_BOOM           0x01
#define COLSTATE_DELAY_BY_AREA  0x02
#define COLSTATE_DELAY_BY_CMD   0x03

#define COL_KEY_PRESSED         0x10

#define COLSTATE_CMD_NOTHING    0
#define COLSTATE_CMD_BOOM       1
#define COLSTATE_CMD_DELAY      2
#define COLSTATE_CMD_BE_OK      3

extern uint8_t CollarState;
extern uint8_t CollarStateCmd;

#endif /* COLLAR_H_ */
