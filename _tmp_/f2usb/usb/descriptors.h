/*
 * descriptors.h
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#ifndef DESCRIPTORS_H_
#define DESCRIPTORS_H_

#include "stdint.h"

enum USB_DescriptorTypes_t {
    dtDevice                    = 0x01, // device descriptor
    dtConfiguration             = 0x02, // configuration descriptor
    dtString                    = 0x03, // string descriptor
    dtInterface                 = 0x04, // interface descriptor
    dtEndpoint                  = 0x05, // endpoint descriptor
    dtDeviceQualifier           = 0x06, // device qualifier descriptor
    dtOther                     = 0x07, // other type
    dtInterfacePower            = 0x08, // interface power descriptor
    dtInterfaceAssociation      = 0x0B, // interface association descriptor
    dtCSInterface               = 0x24, // class specific interface descriptor
    dtCSEndpoint                = 0x25, // class specific endpoint descriptor
};

uint16_t GetDescriptor(uint8_t Type, uint8_t Indx, const uint8_t** const Ptr);


#endif /* DESCRIPTORS_H_ */
