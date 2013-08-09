/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

// Status
enum Status_t {
    stIdle, stCharging, stChargeCompleted, stAccFail,
    stAardSrc,  stAardForce,  stAardExec,  stAardHold,
    stIgniSrc,  stIgniForce,  stIgniExec,  stIgniHold,
    stKvenSrc,  stKvenForce,  stKvenExec,  stKvenHold,
    stIrdenSrc, stIrdenForce, stIrdenExec, stIrdenHold,
    stGelioSrc, stGelioForce, stGelioExec, stGelioHold,
    stSignReset
};

class App_t {
private:

public:
    bool Enabled;
    Status_t Status;
    void Init();
};

extern App_t App;

#endif /* APPLICATION_H_ */
