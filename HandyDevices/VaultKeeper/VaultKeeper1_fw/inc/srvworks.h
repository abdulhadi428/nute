/*
 * srvworks.h
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */

#ifndef SRVWORKS_H_
#define SRVWORKS_H_

#include "kl_lib.h"

#define URL_HOST    "numenor2012.ru"
#define URL_TIME    "/request.php?time"

class SrvWorks_t {
private:
public:
    Error_t GetTime(void);
//    Error_t CloseConnection(void);
//    Error_t GET(const char *AUrl);
};

extern SrvWorks_t Srv;


#endif /* SRVWORKS_H_ */

