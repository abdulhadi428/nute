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

    Pn.Init();
    SD.Init();          // SD-card init
    App.IDStore.Load(); // Init Srorage of IDs
    SndList.Init();

    App.ReadConfig();   // Read config from SD-card
    Sound.Init();
    Sound.SetVolume(225);
    Sound.RegisterAppThd(chThdSelf());
    Sound.Play("alive.wav");

#if USB_ENABLED
    MassStorage.Init(); // Init USB MassStorage device
#endif

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
                        // Iterate AccessAdd / AccessRemove / Idle
                        case keyA:
                            switch(State) {
                                // If was adding, enter removing state
                                case asAddingAccess: EnterRemovingAccessState(); break;
                                // If was removing, enter Idle
                                case asRemovingAccess: EnterIdleState(); break;
                                // Otherwise, enter adding
                                default: EnterAddingAccessState(); break;
                            } // switch State
                            break;

                        // Iterate AdderAdd / AdderRemove / Idle
                        case keyB:
                            switch(State) {
                                case asAddingAdder:    // If was adding, now remove
                                    State = asRemovingAdder;
                                    LedService.StartSequence(lsqRemovingAdderWaiting);
                                    RestartStateTimer();
                                    break;
                                // If was removing, enter Idle
                                case asRemovingAdder: EnterIdleState(); break;
                                // Otherwise, enter adding
                                default:
                                    State = asAddingAdder;
                                    LedService.StartSequence(lsqAddingAdderWaiting);
                                    RestartStateTimer();
                                    break;
                            } // switch State
                            break;

                        // Iterate RemoverAdd / RemoverRemove / Idle
                        case keyC:
                            switch(State) {
                                case asAddingRemover:    // If was adding, now remove
                                    State = asRemovingRemover;
                                    LedService.StartSequence(lsqRemovingRemoverWaiting);
                                    RestartStateTimer();
                                    break;
                                case asRemovingRemover:  // If was removing, now Idle
                                    EnterIdleState();
                                    break;
                                default:                // Otherwise, add
                                    State = asAddingRemover;
                                    LedService.StartSequence(lsqAddingRemoverWaiting);
                                    RestartStateTimer();
                                    break;
                            } // switch State
                            break;
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
            Uart.Printf("\rUsb connected, AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
        }
        if(EvtMsk & EVTMSK_USB_DISCONNECTED) {
            Usb.Shutdown();
            MassStorage.Reset();
            chSysLock();
            Clk.SetFreq12Mhz();
            Clk.InitSysTick();
            chSysUnlock();
            Uart.Printf("\rUsb disconnected, AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
        }
#endif

        // ==== State timeout ====
        if(EvtMsk & EVTMSK_STATE_TIMEOUT) EnterIdleState();
    } // while true
}

#if 1 // ========================= States ======================================
void App_t::EnterIdleState() {
    if(IDStore.HasChanged) IDStore.Save();
    Led.StartSequence(lsqDoorClose);
    LedService.StartSequence(lsqIdle);
    State = asIdle;
}

void App_t::EnterAddingAccessState() {
    State = asAddingAccess;
    Led.StartSequence(lsqAddingAccessWaiting);
    LedService.StartSequence(lsqAddingAccessWaiting);
    RestartStateTimer();
}

void App_t::EnterRemovingAccessState() {
    State = asRemovingAccess;
    Led.StartSequence(lsqRemovingAccessWaiting);
    LedService.StartSequence(lsqRemovingAccessWaiting);
    RestartStateTimer();
}
#endif

void App_t::ProcessCardAppearance() {
//    App.CurrentID.Print();
#if SAVE_LAST_ID
    if(LastID != CurrentID) {
        LastID = CurrentID;
        if(SD.OpenRewrite(LAST_ID_FILENAME) == OK) {
            for(uint32_t i=0; i<8; i++) f_printf(&SD.File, "%02X", LastID.ID8[i]);
            SD.Close();
//            Uart.Printf("\rID saved");
        }
    }
#endif
    // Proceed with check
    IdKind_t IdKind = IDStore.Check(CurrentID);
    switch(State) {
        case asIdle:
            if(DoorState == dsClosed) {
                switch(IdKind) {
                    case ikAccess:  SendEvt(EVTMSK_DOOR_OPEN); break;
                    case ikSecret:  break;
                    case ikAdder:   EnterAddingAccessState(); break;
                    case ikRemover: EnterRemovingAccessState(); break;
                    case ikNone:    SendEvt(EVTMSK_BAD_KEY); break;
                }
            }
            return;
            break;

        case asAddingAccess:
            switch(IdKind) {
                case ikAdder: EnterIdleState(); break;
                case ikRemover: EnterRemovingAccessState(); break;
                case ikNone:
                    if(IDStore.Add(CurrentID, ikAccess) == OK) LedService.StartSequence(lsqAddingAccessNew);
                    else LedService.StartSequence(lsqAddingAccessError);
                    break;
                case ikAccess: LedService.StartSequence(lsqAddingAccessNew); break; // already in base
                case ikSecret: break;
            }
            break;
        case asRemovingAccess:
            switch(IdKind) {
                case ikAdder: EnterAddingAccessState(); break;
                case ikRemover: EnterIdleState(); break;
                case ikNone: LedService.StartSequence(lsqRemovingAccessNew); break; // already absent
                case ikAccess:
                    IDStore.Remove(CurrentID, ikAccess);
                    LedService.StartSequence(lsqRemovingAccessNew);
                    break;
                case ikSecret: break;
            }
            break;

        case asAddingAdder:
            Uart.Printf("\rAdding Adder");
            switch(IdKind) {
                case ikAdder: LedService.StartSequence(lsqAddingAdderNew); break; // already in base
                case ikRemover:
                    IDStore.Remove(CurrentID, ikRemover);
                    if(IDStore.Add(CurrentID, ikAdder) == OK) LedService.StartSequence(lsqAddingAdderNew);
                    else LedService.StartSequence(lsqAddingAdderError);
                    break;
                case ikNone:
                    if(IDStore.Add(CurrentID, ikAdder) == OK) LedService.StartSequence(lsqAddingAdderNew);
                    else LedService.StartSequence(lsqAddingAdderError);
                    break;
                case ikAccess:
                    IDStore.Remove(CurrentID, ikAccess);
                    if(IDStore.Add(CurrentID, ikAdder) == OK) LedService.StartSequence(lsqAddingAdderNew);
                    else LedService.StartSequence(lsqAddingAdderError);
                    break;
                case ikSecret: break;
            }
            break;

        case asRemovingAdder:
            Uart.Printf("\rRemoving Adder");
            LedService.StartSequence(lsqRemovingAdderNew);
            IDStore.Remove(CurrentID, ikAdder);
            break;

        case asAddingRemover:
            Uart.Printf("\rAdding Remover");
            switch(IdKind) {
                case ikAdder:
                    IDStore.Remove(CurrentID, ikAdder);
                    if(IDStore.Add(CurrentID, ikRemover) == OK) LedService.StartSequence(lsqAddingRemoverNew);
                    else LedService.StartSequence(lsqAddingRemoverError);
                    break;
                case ikRemover: LedService.StartSequence(lsqAddingRemoverNew); break; // already in base
                case ikNone:
                    if(IDStore.Add(CurrentID, ikRemover) == OK) LedService.StartSequence(lsqAddingRemoverNew);
                    else LedService.StartSequence(lsqAddingRemoverError);
                    break;
                case ikAccess:
                    IDStore.Remove(CurrentID, ikAccess);
                    if(IDStore.Add(CurrentID, ikRemover) == OK) LedService.StartSequence(lsqAddingRemoverNew);
                    else LedService.StartSequence(lsqAddingRemoverError);
                    break;
                case ikSecret: break;
            }
            break;

        case asRemovingRemover:
            Uart.Printf("\rRemoving Remover");
            LedService.StartSequence(lsqRemovingRemoverNew);
            IDStore.Remove(CurrentID, ikRemover);
            break;
    } // switch
    RestartStateTimer();
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
