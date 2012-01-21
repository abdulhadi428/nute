#include "misc.h"
#include "sd.h"
#include "sd_lowlevel.h"

#include "uart.h"
#include "ff.h"
#include "delay_util.h"


#define BLOCK_SIZE          512 /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS    4  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE   1024

uint8_t Buffer_Rx[MULTI_BUFFER_SIZE];

sd_t SD;

void sd_t::Init() {
    // Register filesystem
    f_mount(0, &SD.FatFilesystem);

    // NVIC configuration
    //    NVIC_InitTypeDef NVIC_InitStructure;
    //    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    //    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //    NVIC_Init(&NVIC_InitStructure);

    // Init SD
//    Status = SD_Init();
//    if (Status != SD_OK) {
//        UART_StrUint("SD init error: ", Status);
//        return;
//    } else UART_PrintString("Crd on\r");


//
//            Status = SD_ReadBlock(Buffer_Rx, (0x0083 * 512), 512);
//            if (Status == SD_OK) {
//                for (uint32_t i = 0; i < 512; i++) {
//                    UART_PrintAsHex(Buffer_Rx[i]);
//                    UART_Print(' ');
//                }
//                UART_NewLine();
//            }
//            else UART_PrintString("err");
}

void sd_t::Test() {
//    UINT BytesWasRead;
//    uint8_t IBuf[4096];//    FRESULT rslt;
//    rslt = f_open(&IFile, IFilename, FA_READ+FA_OPEN_EXISTING);
//    UART_StrUint("Open: ", rslt);
//
//    if (rslt == FR_OK) {
//        rslt = f_read(&IFile, IBuf, 4096, &BytesWasRead);
//        UART_StrUint("f_read: ", rslt);
//        UART_StrUint("BytesWasRead: ", BytesWasRead);
//
//        if (rslt == FR_OK) {
//            for (uint32_t i=0; i<BytesWasRead; i++) {
////                UART_PrintAsHex(IBuf[i]);
////                UART_Print(' ');
//                UART_Print(IBuf[i]);
//            }
//            UART_NewLine();
//        }
//
//
//        rslt = f_close(&IFile);
//        UART_StrUint("Close: ", rslt);
//    }
}

// ========================= FAT needed functions ==============================
// Physical drive number (0), Pointer to the data buffer to store read data, Start sector number (LBA), Sector count (1..255)
DRESULT disk_read (BYTE drv, BYTE *buff, DWORD sector, BYTE count) {
        if (drv || !count) return RES_PARERR;
//    UART_StrUint("Sector: ", sector);
//    UART_StrUint("Count: ", count);
    //uint32_t Tmr1, Tmr2;
    //Tmr1 = Delay.TickCounter;
    SD.Status = SD_ReadBlock(buff, (sector * 512), 512);
    //Tmr2 = Delay.TickCounter;
    //UART_StrUint("Tmr1: ", Tmr1);
    //UART_StrUint("Tmr2: ", Tmr2);
    if (SD.Status != SD_OK) {
        UART_StrUint("Err rd 1: ", SD.Status);
        return RES_ERROR;
    }
    // Wait for DMA transfer to finish
    SD.Status = SD_WaitReadOperation();
    if (SD.Status != SD_OK) {
        UART_StrUint("Err rd 2: ", SD.Status);
        return RES_ERROR;
    }
    return RES_OK;
}


/************* disk_write  *************************/
/* ну это я уже сам написал, вроде должно работать*/
/**************************************************/
DRESULT disk_write (BYTE drv, const BYTE *buff, DWORD sector, BYTE count) {
        if (drv || !count) return RES_PARERR;
    SD.Status = SD_WriteBlock((uint8_t*) buff, (sector * 512), 512);

    SD.TrState=SD_GetStatus();
    while (SD.TrState==SD_TRANSFER_BUSY) // вот эту штуку нужно обязательно подождать
      {
        SD.TrState=SD_GetStatus();
      }


    if (SD.Status != SD_OK) {
        UART_StrUint("Err wr 1: ", SD.Status);
        return RES_ERROR;
    }
    // Wait for DMA transfer to finish
    SD.Status = SD_WaitWriteOperation();
    if (SD.Status != SD_OK) {
        UART_StrUint("Err wr 2: ", SD.Status);
        return RES_ERROR;
    }
    return RES_OK;
}

/*
The disk_ioctl function cntrols device specified
features and miscellaneous functions other than disk read/write.
*/
DRESULT disk_ioctl (BYTE drv, BYTE command, void *buff){
  if (drv) return RES_NOTRDY;
  if (command==CTRL_SYNC) //Make sure that the disk drive has finished pending write process.
    {
      if (SD_GetStatus()!=SD_TRANSFER_OK) return RES_ERROR;
      if (SD_GetTransferState()!=SD_TRANSFER_OK)return RES_ERROR;
      return RES_OK;
    }
  if (command==GET_SECTOR_SIZE) //This command is not used in fixed sector size configuration, _MAX_SS is 512.
    {
      return RES_OK;
    }
  if (command==GET_SECTOR_COUNT) //This command is used by only f_mkfs function to determine the volume size to be created.
    {
      return RES_OK;
    }
  if (command==GET_BLOCK_SIZE) //This command is used by only f_mkfs function and it attempts to align data area to the erase block boundary.
    {
      return RES_OK;
    }
  if (command==CTRL_ERASE_SECTOR) //This command is called on removing a cluster chain when _USE_ERASE is 1.
    {
      return RES_OK;
    }
  return RES_PARERR;
}

DWORD  get_fattime(void){ //Currnet time is returned with packed into a DWORD value.
  DWORD time=0;
  time+=31<<25; //Year from 1980 (0..127)
  time+=12<<21; //Month (1..12)
  time+=25<<16; //Day in month(1..31)
  time+=10<<11; //Hour (0..23)
  time+=10<<25; //Minute (0..59)
  time+=5<<25; //Second / 2 (0..29)
  return time; // 2011_12_25_10_10_10
}

DSTATUS disk_status (BYTE drv) {
    if (drv) return STA_NOINIT;			// Supports only single drive
    return SD.Status;
}
DSTATUS disk_initialize (BYTE drv) {
    if (drv) return STA_NOINIT;			// Supports only single drive
    SD.Status = SD_Init();
    if (SD.Status != SD_OK) {
        UART_StrUint("SD init error: ", SD.Status);
        return STA_NOINIT;
    }

    UART_PrintString("Crd on\r");
    return 0;
}

// ============================= Interrupts ====================================

void SDIO_IRQHandler(void) {
    UART_PrintString("SDIO IRQ\r");
    /* Process All SDIO Interrupt Sources */
    SD_ProcessIRQSrc();
}
