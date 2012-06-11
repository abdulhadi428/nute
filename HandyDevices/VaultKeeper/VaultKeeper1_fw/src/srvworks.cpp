/*
 * srvworks.cpp
 *
 *  Created on: 11.06.2012
 *      Author: kreyl
 */
#include "srvworks.h"
#include "sim900.h"
#include "string.h"
#include "comline.h"
#include "stdlib.h"

SrvWorks_t Srv;

// "AT+CIPSTART=\"TCP\",\"ya.ru\",\"80\""
#define SRVCON_PRE      "AT+CIPSTART=\"TCP\",\""
#define SRVCON_PRE_LEN  19
#define SRVCON_PST      "\",\"80\""
//Error_t SrvWorks_t::OpenConnection(const char *AHostName) {
//    Com.Printf("Connecting %S: ", AHostName);
//    strcpy(IHostName, AHostName);
//    char S[MDM_LINE_LEN];
//    uint8_t Len = strlen(AHostName);
//    memcpy(&S[0], SRVCON_PRE, SRVCON_PRE_LEN);
//    memcpy(&S[SRVCON_PRE_LEN], AHostName, Len);
//    strcpy(&S[SRVCON_PRE_LEN+Len], SRVCON_PST);
//    if(Mdm.Command(S)!= erOk) return erError;
//    // Wait for CONNECT or ERROR
//    uint32_t Tmr;
//    Delay.Reset(&Tmr);
//    while (!Delay.Elapsed(&Tmr, MDM_NETREG_TIMEOUT)) {
//        Com.Printf("#");
//        if(Mdm.WaitString(MDM_RX_TIMEOUT) == erOk) {
//            if (strcmp(Mdm.RxLine, "CONNECT OK") == 0) {
//                Com.Printf("\rConnected\r");
//                return erOk;
//            }
//            else if (strcmp(Mdm.RxLine, "CONNECT FAIL") == 0) {
//                Com.Printf("\rFailure\r");
//                return erError;
//            }
//        }
//    } // while
//    Com.Printf("\rTimeout\r");
//    return erTimeout;
//
//}
//Error_t SrvWorks_t::CloseConnection(void) {
//    return erTimeout;
//}

//Mdm.SendData("GET /request.php?time HTTP/1.1\r\nHost: numenor2012.ru\r\n\r\n");

//Error_t SrvWorks_t::GET(const char *AUrl) {
//    Error_t r = erError;
//
//
//    return r;
//}

//Error_t SrvWorks_t::GET(const char *AUrl) {
//    Error_t r = erError;
//    // Init HTTP service
//    if(Mdm.Command("AT+HTTPINIT") == erOk) {
//        // Set parameters
//        if(Mdm.Command("AT+HTTPPARA=\"CID\",1") == erOk) {
//            // AT+HTTPPARA="URL","ya.ru"
//            char S[MDM_LINE_LEN];
//            uint8_t Len = strlen(AUrl);
//            memcpy(&S[0], "AT+HTTPPARA=\"URL\",\"", 19);
//            memcpy(&S[19], AUrl, Len);
//            strcpy(&S[19+Len], "\"");
//            if(Mdm.Command(S) == erOk) {
//                // Start GET session
//                if(Mdm.Command("AT+HTTPACTION=0") == erOk) {
//                    // Get server reply: +HTTPACTION:0,200,13652
//                    if (Mdm.WaitString("+HTTPACTION", 11, MDM_NETREG_TIMEOUT) == erOk) {
//                        // Get error code
//                        strncpy(S, &Mdm.RxLine[14], 3);
//                        int32_t StatusCode = atoi(S);
//                        if(StatusCode == 200) {
//                            // Calculate reply length
//                            strncpy(S, &Mdm.RxLine[18], 9);
//                            int32_t LenToRx = atoi(S);
//                            if (LenToRx != 0) {
//                                // Read data
//                                if(Mdm.Command("AT+HTTPREAD=0,405") == erOk) {
//                                    Delay.ms(4500);
//                                    r = erOk;
//                                }
//                            } // Len>0
//                        } // if 200
//                    } // srv reply
//                } // Start session
//            } // param 2
//        } // param 1
//    } // init
//    // Terminate HTTP service
//    Mdm.Command("AT+HTTPTERM");
//    if(Mdm.Command("AT+SAPBR=0,1", MDM_NETREG_TIMEOUT) != erOk) return erError;
//    return r;
//}
