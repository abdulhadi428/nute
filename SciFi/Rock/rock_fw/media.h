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

#define MAX_FNAME_LEN   18

enum sndState_t {sndStopped, sndPlaying, sndMustStop};

class Sound_t {
private:
    char IFilename[MAX_FNAME_LEN];
    FIL IFile;
public:
    sndState_t State;
    void Init(void);
    void Task(void);
    void Play(const char* AFilename);
    void Stop(void);
};

extern Sound_t ESnd;

#endif	/* MEDIA_H */

