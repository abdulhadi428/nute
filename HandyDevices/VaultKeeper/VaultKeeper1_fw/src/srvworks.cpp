/*
 * srvworks.cpp
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */
#include "srvworks.h"
#include "sim900.h"
#include "comline.h"
#include "kl_time.h"
#include <string.h>
#include <stdlib.h>
#include "kl_string.h"

SrvWorks_t Srv;

// 2012-06-12 11-51-45
Error_t SrvWorks_t::GetTime(void) {
    Com.Printf("Receiving Time...\r");
    if (Mdm.GET(URL_HOST, URL_TIME, Mdm.DataString, 19) == erOk) {
        char S[7];
        klStrNCpy(S, &Mdm.DataString[0], 4);
        Time.Year = atoi(S);
        klStrNCpy(S, &Mdm.DataString[5], 2);
        Time.Month = atoi(S);
        klStrNCpy(S, &Mdm.DataString[8], 2);
        Time.Day = atoi(S);
        klStrNCpy(S, &Mdm.DataString[11], 2);
        Time.H = atoi(S);
        klStrNCpy(S, &Mdm.DataString[14], 2);
        Time.M = atoi(S);
        klStrNCpy(S, &Mdm.DataString[17], 2);
        Time.S = atoi(S);
        Com.Printf("%u-%u-%u %u:%u:%u\r", Time.Year, Time.Month, Time.Day, Time.H, Time.M, Time.S);
        return erOk;
    }
    else return erError;
}
