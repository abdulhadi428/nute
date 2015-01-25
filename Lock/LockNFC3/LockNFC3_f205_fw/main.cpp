/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f2xx.h"
#include "kl_sd.h"
#include "sound.h"
#include "cmd_uart.h"
#include "ff.h"
#include "MassStorage.h"
#include "main.h"
#include "pn.h"
#include "SimpleSensors.h"
#include "keys.h"
#include "Soundlist.h"

#define USB_ENABLED TRUE

App_t App;
SndList_t SndList;

LedRgbBlinker_t LedService({GPIOB, 10}, {GPIOB, 12}, {GPIOB, 11});
LedRGB_t Led({GPIOB, 0, TIM3, 3}, {GPIOB, 5, TIM3, 2}, {GPIOB, 1, TIM3, 4});

// Universal VirtualTimer callback
void TmrGeneralCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, (eventmask_t)p);
    chSysUnlockFromIsr();
}

int main() {
    // ==== Setup clock ====
    Clk.UpdateFreqValues();
    uint8_t ClkResult = FAILURE;
    Clk.SetupFlashLatency(12);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    App.PThd = chThdSelf();
    Uart.Init(115200);
    Uart.Printf("\rLockNFC3 F205   AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    LedService.Init();
    LedService.StartSequence(lsqBlinkGreenX2);
    Led.Init();
    Led.StartSequence(lsqDoorClose);
    Sensors.Init();

    // Wait to allow power to stabilize
    chThdSleepMilliseconds(702);

    App.IDStore.Load(); // Init Srorage of IDs

    Pn.Init();
    SD.Init();          // SD-card init
    SndList.Init();
//    SndList.PlayRandomFileFromDir("GoodKey");
//    SndList.PlayRandomFileFromDir("Closing");

    App.ReadConfig();   // Read config from SD-card
#if USB_ENABLED
    MassStorage.Init(); // Init USB MassStorage device
#endif
    Sound.Init();
    Sound.SetVolume(180);
    Sound.RegisterAppThd(chThdSelf());
    Sound.Play("alive.wav");

    // ==== Main cycle ====
    App.ITask();
}

__attribute__ ((__noreturn__))
void App_t::ITask() {
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        // ==== Card ====
        if(EvtMsk & EVTMSK_CARD_APPEARS) ProcessCardAppearance();

#if 1 // ==== Door ====
        if(EvtMsk & EVTMSK_DOOR_OPEN) {
            DoorState = dsOpen;
            Led.StartSequence(lsqDoorOpen); // Set color
            SndList.PlayRandomFileFromDir("GoodKey");
            Uart.Printf("\rDoor is open");
            chSysLock();
            if(chVTIsArmedI(&IDoorTmr)) chVTResetI(&IDoorTmr);
            chVTSetI(&IDoorTmr, MS2ST(DOOR_CLOSE_TIMEOUT), TmrGeneralCallback, (void*)EVTMSK_DOOR_SHUT);
            chSysUnlock();
        }
        if(EvtMsk & EVTMSK_DOOR_SHUT) {
            DoorState = dsClosed;
            Led.StartSequence(lsqDoorClose);    // Set color
            SndList.PlayRandomFileFromDir("Closing");
            Uart.Printf("\rDoor is closed");
        }

        if(EvtMsk & EVTMSK_BAD_KEY) {
            Led.StartSequence(lsqDoorWrongKey);
            SndList.PlayRandomFileFromDir("BadKey");
            Uart.Printf("\rBadKey");
        }
#endif

#if 1 // ==== Keys ====
        if(EvtMsk & EVTMSK_KEYS) {
            KeyEvtInfo_t EInfo;
            while(Keys.EvtBuf.Get(&EInfo) == OK) {
//                Uart.Printf("\rEinfo: %u, %u, %u", EInfo.Type, EInfo.KeysCnt, EInfo.KeyID[0]);
                if(EInfo.Type == kePress) {
                    switch(EInfo.KeyID[0]) {
                        case keyA:
                            if(State == asAddingAcc) {
                                if(IDStore.HasChanged) IDStore.Save();
                                LedService.StartSequence(lsqIdle);
                                State = asIdle;
                            }
                            else {
                                State = asAddingAcc;
                                LedService.StartSequence(lsqAddingAccIdle);
                                RestartStateTimer();
                            }
                            break;

                        case keyB:
                            if(State == asRemovingAcc) {
                                if(IDStore.HasChanged) IDStore.Save();
                                LedService.StartSequence(lsqIdle);
                                State = asIdle;
                            }
                            else {
                                State = asRemovingAcc;
                                LedService.StartSequence(lsqRemovingAccIdle);
                                RestartStateTimer();
                            }
                            break;

                        case keyC: break;
                    } // switch
                } // if keypress
                else if(EInfo.Type == keCombo and EInfo.KeyID[0] == keyA and EInfo.KeyID[1] == keyB) {
                    LedService.StartSequence(lsqEraseAll);
                    IDStore.EraseAll();
                    IDStore.Save();
                    State = asIdle;
                }
            } // while
        } // if keys
#endif

#if USB_ENABLED // ==== USB connection ====
        if(EvtMsk & EVTMSK_USB_CONNECTED) {
            chSysLock();
            Clk.SetFreq48Mhz();
            Clk.InitSysTick();
            chSysUnlock();
            Usb.Init();
            chThdSleepMilliseconds(540);
            Usb.Connect();
            Uart.Printf("\rUsb connected");
        }
        if(EvtMsk & EVTMSK_USB_DISCONNECTED) {
            Usb.Shutdown();
            MassStorage.Reset();
            chSysLock();
            Clk.SetFreq12Mhz();
            Clk.InitSysTick();
            chSysUnlock();
            Uart.Printf("\rUsb disconnected");
        }
#endif

        // ==== State timeout ====
        if(EvtMsk & EVTMSK_STATE_TIMEOUT) {
            LedService.StartSequence(lsqIdle);
            State = asIdle;
            if(IDStore.HasChanged) IDStore.Save();
        }
    } // while true
}

void App_t::ProcessCardAppearance() {
//    App.CurrentID.Print();
    switch(State) {
        case asIdle:
            if(DoorState == dsClosed) {
                IdKind_t IdKind = IDStore.Check(CurrentID);
                switch(IdKind) {
                    case ikAccess:  SendEvt(EVTMSK_DOOR_OPEN); break;
                    case ikSpecial: break;
                    case ikAdder: break;
                    case ikRemover: break;
                    case ikNone:    SendEvt(EVTMSK_BAD_KEY);   break;
                }
            }
            break;

        case asAddingAcc:
            LedService.StartSequence(lsqAddingAccNew);
            if(IDStore.AddAccessID(CurrentID) != OK) LedService.StartSequence(lsqError);
            RestartStateTimer();
            break;

        case asRemovingAcc:
            LedService.StartSequence(lsqRemovingAccNew);
            IDStore.RemoveAccess(CurrentID);
            RestartStateTimer();
            break;

        case asAddingMaster:
        case asRemovingMaster:
            break;
    } // switch
}

uint8_t App_t::ReadConfig() {
//    int32_t Probability;
//    if(SD.iniReadInt32("Sound", "Count", "settings.ini", &SndList.Count) != OK) return FAILURE;
//    Uart.Printf("\rCount: %d", SndList.Count);
//    if (SndList.Count <= 0) return FAILURE;
//    char *c, SndKey[MAX_NAME_LEN]="Sound";
//    SndList.ProbSumm = 0;
    // Read sounds data
//    for(int i=0; i<SndList.Count; i++) {
        // Build SndKey
//        c = Convert::Int32ToStr(i+1, &SndKey[5]);   // first symbol after "Sound"
//        strcpy(c, "Name");
//        Uart.Printf("\r%s", SndKey);
        // Read filename and probability
//        char *S = nullptr;
//        if(SD.iniReadString("Sound", SndKey, "settings.ini", &S) != OK) return FAILURE;
//        strcpy(SndList.Phrases[i].Filename, S);
//        strcpy(c, "Prob");
//        Uart.Printf("\r%s", SndKey);
//        if(SD.iniReadInt32 ("Sound", SndKey, "settings.ini", &Probability) != OK) return FAILURE;
//        // Calculate probability boundaries
//        SndList.Phrases[i].ProbBottom = SndList.ProbSumm;
//        SndList.ProbSumm += Probability;
//        SndList.Phrases[i].ProbTop = SndList.ProbSumm;
//    }
//    for(int i=0; i<SndList.Count; i++) Uart.Printf("\r%u %S Bot=%u Top=%u", i, SndList.Phrases[i].Filename, SndList.Phrases[i].ProbBottom, SndList.Phrases[i].ProbTop);
    return OK;
}
