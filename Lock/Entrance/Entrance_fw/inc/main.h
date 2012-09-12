/*
 * main.h
 *
 *  Created on: 28.01.2012
 *      Author: kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#define NOSIGNAL_DELAY  999

// Brightness value matching MIN RSSI
#define MIN_BRT                 1
// Brightness value matching MAX RSSI
#define MAX_BRT                 250

#define MIN_ADDRESS     1
#define MAX_ADDRESS     20
#define TRY_COUNT       1
#define ALIEN_COUNT     (MAX_ADDRESS - MIN_ADDRESS + 1)

struct Alien_t {
    uint32_t Timer;
    bool New, Exists;
    int32_t RSSI;
};

#define AVG_SZ      99  // Size of average
class Signal_t {
private:
    Alien_t Alien[ALIEN_COUNT];
    int32_t AvgBuf[AVG_SZ], AvgIndx;
public:
    // Brightness coefs
    int32_t BrtA, BrtB;
    // General
    bool AllThemCalled;
    void Init(void);
    void Task(void);
    void Remember(uint8_t AAddress, int32_t RawRSSI);
};


#endif /* MAIN_H_ */
