/*
 * RS485Class.h
 *
 *  Created on: 25.11.2012
 *      Author: Stefan
 */

#ifndef RS485CLASS_H_
#define RS485CLASS_H_

#include <stdint.h>


class RS485_Class {
private:

public:
    uint16_t        i;
    void Init(void);
    void Task(void);
    void SendData (void);
    void GetData (void);
};
#endif /* RS485CLASS_H_ */
