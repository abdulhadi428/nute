/*
 * FlashStorage.h
 *
 *  Created on: 12.09.2012
 *      Author: kreyl
 */

#ifndef FLASHSTORAGE_H_
#define FLASHSTORAGE_H_

#include <inttypes.h>

#define FLASH_PAGE_SIZE     1024    // Constant
#define SAVED_DATA_SIZE     1024    // must be multiple of FlashPageSize
#define PAGE_COUNT          (SAVED_DATA_SIZE / FLASH_PAGE_SIZE)

class FlashStorage_t {
public:
    void Load(uint32_t *PData, uint32_t ALength);
    void Save(uint32_t *PData, uint32_t ALength);
};

#endif /* FLASHSTORAGE_H_ */
