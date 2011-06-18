/*
 * File:   media.h
 * Author: g.kruglov
 *
 * Created on June 15, 2011, 4:44 PM
 */

#ifndef MEDIA_H
#define	MEDIA_H

#include "ff.h"
#include "diskio.h"
#include <stdint.h>

#define SND_BUF_SIZE    512

enum sndState_t {sndStopped, sndPlaying, sndMustStop};

typedef struct {
    uint8_t Arr[SND_BUF_SIZE];
    UINT Size;
} SndBuf_t;

class Sound_t {
private:
    FIL IFile;
    SndBuf_t Buf1, Buf2, *CBuf;
    uint32_t BytesSent;
    void UploadData(void);
    void StopNow(void);
public:
    sndState_t State;
    void Init(void);
    void Task(void);
    void Play(const char* AFilename);
    void Stop(void) { State = sndMustStop; }
};

extern Sound_t ESnd;

#endif	/* MEDIA_H */

