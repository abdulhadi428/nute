/*
 * main.h
 *
 *  Created on: 28.01.2012
 *      Author: kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#define NOSIGNAL_DELAY  999

// ==== Values of RSSI to convert to brightness ====
// All values in dBm.
// Set between -108 and 0. -108 means "Use all". 0 is impossible.
#define MIN_RSSI_TO_USE         (-60)
// Brightness value matching MIN RSSI
#define MIN_BRT                 1
// At this value of RSSI brightness will be full
#define FULL_BRIGHTNESS_RSSI    (-40)
// Brightness value matching MAX RSSI
#define MAX_BRT                 250
// Coeffs of brightness conversion
#define BRT_A   ((int32_t)((MAX_BRT - MIN_BRT) / (FULL_BRIGHTNESS_RSSI - MIN_RSSI_TO_USE)))
#define BRT_B   ((int32_t)(MIN_BRT - BRT_A * MIN_RSSI_TO_USE))

#define MIN_ADDRESS     1
#define MAX_ADDRESS     20
#define ALIEN_COUNT     (MAX_ADDRESS - MIN_ADDRESS + 1)

struct Alien_t {
    uint32_t Timer;
    bool New, Exists;
    int32_t RSSI;
};

class Signal_t {
private:
    Alien_t Alien[ALIEN_COUNT];
public:
    void Init(void);
    void Task(void);
    void Remember(uint8_t AAddress, int32_t RawRSSI);
};


#endif /* MAIN_H_ */
