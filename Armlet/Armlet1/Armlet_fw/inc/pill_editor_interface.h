/*
 * pill_editor_interface.h
 *
 *  Created on: 25.09.2012
 *      Author: kreyl
 */

#ifndef PILL_EDITOR_INTERFACE_H_
#define PILL_EDITOR_INTERFACE_H_

#include "i2c_ee.h"

#define PE_BLINK_DELAY     360  // ms

enum PEIntState_t {pstNoPill, pstReading, pstWriting, pstIdle};
enum PESelection_t {selType, selChrg};
enum PEMenuState_t {msIdle, msTypeEdit, msChargeEdit};

class PEIntf_t {
private:
    uint32_t Tmr;
    PEIntState_t IState;
    PillData_t IPillData;
    void PrintType(bool ShowNotClear);
    void PrintCharge(bool ShowNotClear);
    void Clear();
    void MenuHandler();
    PESelection_t SelId;
    PEMenuState_t MState;
    void Select(PESelection_t NewSelId);
    bool IsDisplayed;
public:
    void Init();
    void Task();
};

extern PEIntf_t PEInterface;

#endif /* PILL_EDITOR_INTERFACE_H_ */
