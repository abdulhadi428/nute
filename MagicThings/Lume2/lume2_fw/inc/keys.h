/*
 * keys.h
 *
 *  Created on: 26.11.2011
 *      Author: Kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "kl_lib.h"
#include <inttypes.h>
#include "interface.h"

#define KEY_DELAY                   49      // ms


class Key_t {
private:
    GPIO_TypeDef *PGpioPort;
    uint16_t PinMask;

    bool prev_pressed;
    bool is_pressed;
	uint32_t unhandled_presses;
public:
	Key_t(GPIO_TypeDef *PGpioPort, uint16_t PinMask)
		: PGpioPort(PGpioPort),
		  PinMask(PinMask),
		  prev_pressed(false),
		  is_pressed(false),
		  unhandled_presses(0) {}

	bool IsPressed() { return is_pressed; }

	bool WasJustPressed() {
		if (unhandled_presses == 0) return false;
		unhandled_presses--;
		return true;
	}

	friend class Keys_t;
};


class Keys_t {
private:
    uint32_t timer;
    static Key_t *keys[];
public:
    void Init(void);
    void Task(void);

    static Key_t Up;
    static Key_t Down;
    static Key_t Enter;

    bool AnyKeyWasJustPressed();
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
