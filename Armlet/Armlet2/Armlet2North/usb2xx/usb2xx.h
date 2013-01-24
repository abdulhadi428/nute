/*
 * usb2xx.h
 *
 *  Created on: 31.12.2012
 *      Author: kreyl
 */

#ifndef USB2XX_H_
#define USB2XX_H_

class EndPnt_t {
private:

public:
};

enum UsbStatus_t {usOn, usOff, usFailure};

class Usb_t {
private:

public:
    UsbStatus_t Status;
    void On();
    void Off();
};

extern Usb_t Usb;

#endif /* USB2XX_H_ */
