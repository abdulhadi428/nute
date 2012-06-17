/*
 * sound.h
 *
 *  Created on: 25.01.2012
 *      Author: kreyl
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "kl_lib.h"

//#define BEEP_DISABLE    // Uncomment this in case of emergency

// Boom
#define BEEP_BOOM_LEN   9999
// Delay
#define BEEP_SND_LEN    45
const uint32_t BeepLenArr[] = {1200,1144,1091,1041,993,947,903,861,821,783,747,712,679,648,618,589,562,536,511,487,465,443,423,403,384,367,350,333,318,303,289,276,263,251,239,228,218,208,198,189,180,172,164,156,149,142,135,129,123,117,112,107,102,97,93,88,84,80,77,73,70,66,63,60,58,55,52,50,48,45,43,41,39,38,36,34,33,31,30,28,27};
#define BEEP_ARR_SZ     countof(BeepLenArr)

class Beep_t {
private:
    klPwmChannel_t IPwm;
public:
    uint32_t Timer, SilenceLen;
    int32_t N;
    uint16_t Freq;
    uint8_t Volume;
    bool IsOn;
    void On(void);
    void Off(void) { IPwm.Disable(); IsOn = false;}
    void Init(void);
};

extern Beep_t Beep;


#endif /* SOUND_H_ */
