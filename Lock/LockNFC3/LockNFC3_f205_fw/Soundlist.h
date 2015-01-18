/*
 * Soundlist.h
 *
 *  Created on: 18 џэт. 2015 у.
 *      Author: Kreyl
 */

#ifndef SRC_SOUNDLIST_H_
#define SRC_SOUNDLIST_H_

#include "kl_lib_f2xx.h"
#include "kl_sd.h"

#define DIRS_MAX_CNT     3  // Max number of dirs used. Here GoodKey, BadKey, Closing

//struct Dir_t {
//     char Filename[MAX_NAME_LEN];
//     uint16_t NumberOfFiles;
//};

class SndList_t {
private:
//    uint16_t NumberOfFilesInDir[DIRS_MAX_CNT];
//    Snd_t Phrases[SND_COUNT_MAX];
//    int32_t Count;
//    int32_t ProbSumm;
    char Filename[MAX_NAME_LEN];    // to store name with path
    uint32_t PreviousN;
    DIR Dir;
    FILINFO FileInfo;
    FRESULT CountFilesInDir(const char* DirName, uint32_t *PCnt);
public:
    void Init();
    void PlayRandomFileFromDir(const char* DirName);
};



#endif /* SRC_SOUNDLIST_H_ */
