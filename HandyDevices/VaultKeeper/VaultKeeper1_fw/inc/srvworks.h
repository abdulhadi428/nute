/*
 * srvworks.h
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */

#ifndef SRVWORKS_H_
#define SRVWORKS_H_

#include "kl_lib.h"

#define SRV_HEADER_SZ   207
#define SRV_REPLY_SZ    45
#define SRV_HOSTNAME_SZ 18

class SrvWorks_t {
private:
    char IHostName[SRV_HOSTNAME_SZ];
public:
    char Header[SRV_HEADER_SZ], Reply[SRV_REPLY_SZ];
//    Error_t OpenConnection(const char *AHostName);
//    Error_t CloseConnection(void);
//    Error_t GET(const char *AUrl);
};

extern SrvWorks_t Srv;


#endif /* SRVWORKS_H_ */

