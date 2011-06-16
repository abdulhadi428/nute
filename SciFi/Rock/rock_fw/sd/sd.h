/*
 * File:   sd.h
 * Author: Kreyl
 *
 * Created on June 5, 2011, 6:40 PM
 */

#ifndef SD_H
#define	SD_H

#include "stm32f10x.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "stm32_eval_sdio_sd.h"
#include "diskio.h"

// ============================ Defines ========================================
#define SD_BUF_SIZE 4096

// ============================= Types =========================================

class sd_t {
private:
public:
    // Files data
    FATFS FatFilesystem;
//    FIL file1;
//    DIR dir;
//    uint8_t Buf[SD_BUF_SIZE];
    // SD data
    SD_Error Status;
    void Init(void);
};


//Structure to access Master Boot Record for getting info about partioions

typedef struct __attribute__ ((packed)) {
    uint8_t nothing[446]; //ignore, placed here to fill the gap in the structure
    uint8_t partitionData[64]; //partition records (16x4)
    uint16_t signature; //0xaa55
} MBRinfo_Structure;

//Structure to access info of the first partioion of the disk

typedef struct __attribute__ ((packed)) {
    unsigned char status; //0x80 - active partition
    unsigned char headStart; //starting head
    unsigned int cylSectStart; //starting cylinder and sector
    unsigned char type; //partition type
    unsigned char headEnd; //ending head of the partition
    unsigned int cylSectEnd; //ending cylinder and sector
    unsigned long firstSector; //total sectors between MBR & the first sector of the partition
    unsigned long sectorsTotal; //size of this partition in sectors
} partitionInfo_Structure;

//Structure to access boot sector data

typedef struct __attribute__ ((packed)) {
    unsigned char jumpBoot[3]; //default: 0x009000EB
    unsigned char OEMName[8];
    unsigned int bytesPerSector; //deafault: 512
    unsigned char sectorPerCluster;
    unsigned int reservedSectorCount;
    unsigned char numberofFATs;
    unsigned int rootEntryCount;
    unsigned int totalSectors_F16; //must be 0 for FAT32
    unsigned char mediaType;
    unsigned int FATsize_F16; //must be 0 for FAT32
    unsigned int sectorsPerTrack;
    unsigned int numberofHeads;
    unsigned long hiddenSectors;
    unsigned long totalSectors_F32;
    unsigned long FATsize_F32; //count of sectors occupied by one FAT
    unsigned int extFlags;
    unsigned int FSversion; //0x0000 (defines version 0.0)
    unsigned long rootCluster; //first cluster of root directory (=2)
    unsigned int FSinfo; //sector number of FSinfo structure (=1)
    unsigned int BackupBootSector;
    unsigned char reserved[12];
    unsigned char driveNumber;
    unsigned char reserved1;
    unsigned char bootSignature;
    unsigned long volumeID;
    unsigned char volumeLabel[11]; //"NO NAME "
    unsigned char fileSystemType[8]; //"FAT32"
    unsigned char bootData[420];
    unsigned int bootEndSignature; //0xaa55
} BS_Structure;


extern sd_t SD;

// ==== Declare Timer IRQ. Use externC to make it visible from asm file. =======
extern "C" {
    void SDIO_IRQHandler(void);
}



#endif	/* SD_H */

