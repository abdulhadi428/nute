#include "media.h"
#include <string.h>

#include "uart.h"
#include "vs.h"

Sound_t ESnd;

void Sound_t::Task() {
    switch (State) {
        case sndPlaying:
            if (!Vs.IsBusy()) UploadData();
            break;

        case sndMustStop:
            if (!Vs.IsBusy()) StopNow();
            break;

        default: break; // just get out
    } // switch
}

void Sound_t::Init() {
    // *IFilename = 0; // null-terminate string
    State = sndStopped;
}

void Sound_t::Play(const char* AFilename) {
    //strcpy(IFilename, AFilename);
    FRESULT rslt;
    // Open file
    rslt = f_open(&IFile, AFilename, FA_READ+FA_OPEN_EXISTING);
    if (rslt != FR_OK) {
        if (rslt == FR_NO_FILE) {
            UART_PrintString(AFilename);
            UART_PrintString("File not found\r");
        }
        else UART_StrUint("OpenFile error: ", rslt);
        return;
    }
    // Check if zero file
    if (IFile.fsize == 0) {
        f_close(&IFile);
        UART_PrintString("Empty file\r");
        return;
    }
    // Fill buffer
    rslt = f_read(&IFile, Buf1.Arr, SND_BUF_SIZE, &Buf1.Size);
    if (rslt != FR_OK) {
        UART_StrUint("ReadFile1 error: ", rslt);
        State = sndStopped;
        return;
    }
//    if (Buf1.Size == SND_BUF_SIZE) {    // if first block is filled, there is reason to read second block
//        rslt = f_read(&IFile, Buf2.Arr, SND_BUF_SIZE, &Buf2.Size);
//        if (rslt != FR_OK) {
//            UART_StrUint("ReadFile2 error: ", rslt);
//            State = sndStopped;
//            return;
//        }
//    }
    // Setup buffers
    CBuf = &Buf1;
    // Setup VS
    Vs.Enable();
    Vs.AmplifierOn();
    // Load first bunch of data
    UART_PrintString("Playing...\r");
    State = sndPlaying;
    UploadData();
}

void Sound_t::UploadData() {
    Vs.WriteData(CBuf->Arr, CBuf->Size);
    if(IFile.fptr < IFile.fsize) { // Not EOF
        // Switch to other buffer
        CBuf = (CBuf == &Buf1)? &Buf2 : &Buf1;
        // Fill the buffer
        FRESULT rslt = f_read(&IFile, CBuf->Arr, SND_BUF_SIZE, &CBuf->Size);
        if (rslt != FR_OK) {
            UART_StrUint("ReadFileU error: ", rslt);
            StopNow();
            return;
        }
    }
    else StopNow();
}

void Sound_t::StopNow() {
    f_close(&IFile);
    Vs.WriteTrailingZeroes();
    State = sndStopped;
    Vs.AmplifierOff();
    Vs.Disable();
    UART_PrintString("Stopped\r");
}
