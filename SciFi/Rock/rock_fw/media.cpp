#include "media.h"
#include <string.h>

#include "uart.h"

Sound_t ESnd;

void Sound_t::Task() {
    switch (State) {
        case sndPlaying:
            break;

        case sndMustStop:
            break;

        default: break; // just get out
    } // switch
}

void Sound_t::Init() {
    *IFilename = 0; // null-terminate string
    State = sndStopped;
}

void Sound_t::Play(const char* AFilename) {
    strcpy(IFilename, AFilename);
    // OpenFile (IFilename);
    FRESULT rslt;
    rslt = f_open(&IFile, IFilename, FA_READ+FA_OPEN_EXISTING);

    UART_StrUint("open: ", rslt);
}
void Sound_t::Stop() {
    State = sndMustStop;
}