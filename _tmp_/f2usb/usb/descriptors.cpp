/*
 * descriptors.cpp
 *
 *  Created on: May 8, 2013
 *      Author: g.kruglov
 */

#include "descriptors.h"

// Macro to calculate the power value for the configuration descriptor, from a given number of milliamperes.
#define USB_CONFIG_POWER_MA(mA) ((mA) >> 1)
// Macro to encode a given four digit floating point version number (e.g. 01.23) into Binary Coded Decimal format for descriptor fields requiring BCD encoding, such as the USB version number in the standard device descriptor.
#define VERSION_BCD(x)          (((((VERSION_TENS(x) << 4) | VERSION_ONES(x)) << 8) | ((VERSION_TENTHS(x) << 4) | VERSION_HUNDREDTHS(x))))
//Macro to calculate the Unicode length of a string with a given number of Unicode characters
#define USB_STRING_LEN(UnicodeChars)      (2 + ((UnicodeChars) << 1))
// Helper macro for byte values into descriptor strings.
#define USB_DESC_BYTE(b) ((uint8_t)(b))
// Helper macro for word values into descriptor strings.
#define USB_DESC_WORD(w)                                                    \
  (uint8_t)((w) & 255),                                                     \
  (uint8_t)(((w) >> 8) & 255)


// ============================== Descriptors ==================================
// Device
static const uint8_t DeviceDescriptor[] = {
        USB_DESC_BYTE(18),
        USB_DESC_BYTE(dtDevice),
        USB_DESC_WORD(0x0200),     // USB 2.0
        USB_DESC_BYTE(0x02),       // bDeviceClass (CDC)
        USB_DESC_BYTE(0x00),       // bDeviceSubClass
        USB_DESC_BYTE(0x00),       // bDeviceProtocol
        USB_DESC_BYTE(0x40),       // bMaxPacketSize.
        USB_DESC_WORD(0x0483),     // idVendor (ST).
        USB_DESC_WORD(0x5740),     // idProduct.
        USB_DESC_WORD(0x0200),     // bcdDevice.
        USB_DESC_BYTE(1),          // iManufacturer.
        USB_DESC_BYTE(2),          // iProduct.
        USB_DESC_BYTE(0),          // iSerialNumber.
        USB_DESC_BYTE(1)           // bNumConfigurations.
};

// Configuration
static const uint8_t ConfigDescriptor[] = {
        USB_DESC_BYTE(2),
        USB_DESC_BYTE(dtConfiguration),
};


// ================================= Strings ===================================
// U.S. English language identifier
static const uint8_t LanguageString[] = {
        USB_DESC_BYTE(USB_STRING_LEN(1)),
        USB_DESC_BYTE(dtString),
        USB_DESC_WORD(0x0409)
};

// Vendor string
static const uint8_t ManufacturerString[] = {
        USB_DESC_BYTE(USB_STRING_LEN(38)),
        USB_DESC_BYTE(dtString),
        'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
        'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
        'c', 0, 's', 0
};

// Device Description string
static const uint8_t ProductString[] = {
        USB_DESC_BYTE(USB_STRING_LEN(1)),
        USB_DESC_BYTE(dtString),
        'C', 0, 'h', 0, 'i', 0, 'b', 0, 'i', 0, 'O', 0, 'S', 0, '/', 0,
  'R', 0, 'T', 0, ' ', 0, 'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0,
  'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0, 'M', 0, ' ', 0, 'P', 0,
  'o', 0, 'r', 0, 't', 0
};

// ============================= Functions =====================================
uint16_t GetDescriptor(uint8_t Type, uint8_t Indx, const uint8_t** const Ptr) {
    const uint8_t *PAddress = 0;
    uint16_t Size = 0;
    switch(Type) {
        case dtDevice:
            PAddress = DeviceDescriptor;
            Size     = sizeof(DeviceDescriptor);
            break;
        case dtConfiguration:
            PAddress = ConfigDescriptor;
            Size     = sizeof(ConfigDescriptor);
            break;
        case dtString:
            switch(Indx) {
                case 0:
                    PAddress = LanguageString;
                    Size     = sizeof(LanguageString);
                    break;
                case 1:
                    PAddress = ManufacturerString;
                    Size     = sizeof(ManufacturerString);
                    break;
                case 2:
                    PAddress = ProductString;
                    Size     = sizeof(ProductString);
                    break;
                default: break;
            }
            break;
        default: break;
    }
    *Ptr = PAddress;
    return Size;
}
