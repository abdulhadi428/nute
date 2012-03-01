/*
 * main.h
 *
 *  Created on: 18.02.2012
 *      Author: Владимир
 */

#ifndef MAIN_H_
#define MAIN_H_

class Switchers_t {
    klPin_t SFront, SBack;
    uint32_t STimer;
private:
    bool FrontIsOn(void) { return (SFront == false); }
    bool BackIsOn(void)  { return (SBack  == false); }
public:
    void Init(void);
    void Task(void);
    void UpdateState(void);
};


#endif /* MAIN_H_ */
