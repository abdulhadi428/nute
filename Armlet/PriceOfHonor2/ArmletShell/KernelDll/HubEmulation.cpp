//Uart = serial com port
//gate daemon works with virtual com port
//in real life this virtual com port is over USB with real armlet/hub 
//in emulation mode virtual com port is over named pipe (via Free Virtual Serial Ports)

//modes when armlet connected to gate directly
#define MODE_DIRECT_ARMLET	1	//to change it ID
#define MODE_DIRECT_HUB		2	//to write  pills
//mode with hub emulation and armlet use radio
#define MODE_EMULATE_HUB	3	//to debug armlet server
//mode (default) when radio is emulated by visualizer UI form
#define MODE_NONE			4	//to debug armlet application

// Uart Command Codes. See https://docs.google.com/document/d/1pGQf9CrQ016ObS0w7PhPLAy92MRPhdBriICflt1YGXA/edit
#define CMD_PING            0x01
#define CMD_SET_GATE_NUM    0x10
#define CMD_RTX             0x11
#define CMD_PILL_STATE      0x30
#define CMD_PILL_WRITE      0x31
#define CMD_PILL_READ       0x32
#define CMD_PIN             0x40

#define RPL_ACK             0x90    // Acknowledge
#define RPL_SET_GATE_NUM    0xA0
#define RPL_RTX             0xA1    // TX completed
#define RPL_RRX             0xA4    // RX completed
#define RPL_PILL_STATE      0xC0
#define RPL_PILL_WRITE      0xC1
#define RPL_PILL_READ       0xC2
#define RPL_PIN             0xD0

#if 0

//	\\.\pipe\vcom

GetReceivedPkt
GetTransmittedState
AddPktToTx

void OnReceivePacket()
{
	//rLevel1.GetReceivedPkt
	//Printf("#%X,%A\r\n", CmdCode, PData);
	//Uart.Cmd(RPL_RRX, (uint8_t*)&SRxPkt);
}

void OnTransmitPacket()
{
	//rLevel1.GetTransmittedState
	//Uart.Cmd(RPL_RTX, (uint8_t*)&PktState);
}

//=========================== Command processing ===============================
void Ack(uint8_t Result) { Uart.Cmd(0x90, &Result, 1); }
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    uint8_t b, b2;
    switch(CmdCode) {
        case CMD_PING: Ack(OK); break;
        // ==== Radio ====
        case CMD_SET_GATE_NUM:
            SBuf[0] = PData[0];   // Gate number
            if(SBuf[0] < RGATE_CNT) {
                rLevel1.SetID(SBuf[0]);
                SBuf[1] = OK;
            }
            else SBuf[1] = FAILURE;
            Uart.Cmd(RPL_SET_GATE_NUM, SBuf, 2);
            break;
        case CMD_RTX:
            b = PData[0];   // Armlet ID
            if((b >= RDEV_BOTTOM_ID) and (b <= RDEV_TOP_ID)) {
                rLevel1.AddPktToTx(b, &PData[1], (Length-1));
            } // if addr ok
            break;
        // ==== Pills ====
        case CMD_PILL_STATE:
            b = PData[0];   // Pill address
            if(b <= 7) SBuf[1] = Pill[b].CheckIfConnected();
            SBuf[0] = b;
            Uart.Cmd(RPL_PILL_STATE, SBuf, 2);
            break;
        case CMD_PILL_WRITE:
            b = PData[0];
            if(b <= 7) SBuf[1] = Pill[b].Write(&PData[1], Length-1);
            SBuf[0] = b;
            Uart.Cmd(RPL_PILL_WRITE, SBuf, 2);
            break;
        case CMD_PILL_READ:
            b = PData[0];           // Pill address
            b2 = PData[1];          // Data size to read
            if(b2 > 250) b2 = 250;  // Check data size
            if(b <= 7) SBuf[1] = Pill[b].Read(&SBuf[2], b2);
            SBuf[0] = b;
            if(SBuf[1] == OK) Uart.Cmd(RPL_PILL_READ, SBuf, b2+2);
            else Uart.Cmd(RPL_PILL_READ, SBuf, 2);
            break;
        // ==== Pin ====
        case CMD_PIN:
            SBuf[0] = PData[0];     // Pin ID
            SBuf[1] = OK;
            b = PData[1];
            if     (b == 0x00) Pin.Low();
            else if(b == 0x01) Pin.High();
            else if(b == 0x02) Pin.Pulse(((uint32_t)PData[2]) * 100);
            else SBuf[1] = CMD_ERROR;
            Uart.Cmd(RPL_SET_GATE_NUM, SBuf, 2);
            break;

        case 0xF0:
            b2 = PData[0];
            for(b=0; b<b2; b++) {
                Uart.Cmd(0xF1, &b, 1);
                Uart.Cmd(0xF1, &b, 1);
                Uart.Cmd(0xF1, &b, 1);
                chThdSleepMilliseconds(21);
            }
            break;
        default:
            Ack(CMD_ERROR);
            break;
    } // switch
}

//DEVICE
        if(EvtMsk &EVTMASK_KEYS) {
            for(uint8_t i=0; i<KEYS_CNT; i++) {
                if(KeyStatus[i].HasChanged) {
                    if(KeyStatus[i].State == ksReleased) ArmletApi::OnButtonRelease(i);
                    else {
                        #ifdef DEMONSTRATE
                        Beeper.Beep(ShortBeep);
                        #endif
                        Vibro.Vibrate(ShortBrr);
                        ArmletApi::OnButtonPress(i);
                    }
                    KeyStatus[i].HasChanged = false;
                }
            } // for
        } // keys

        if(EvtMsk & EVTMASK_RADIO_RX) {
            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK) {
                #ifdef DEMONSTRATE
                Uart.Printf("%A\r", SRxPkt.Data, SRxPkt.Length, ' ');
                #endif
                //Uart.Printf("%A\r", SRxPkt.Data, SRxPkt.Length, ' ');
                //rLevel1.AddPktToTx(0, SRxPkt.Data, SRxPkt.Length);
                ArmletApi::OnRadioPacket(SRxPkt.Data, SRxPkt.Length);
            }
        } // if evtmsk

        if(EvtMsk & EVTMASK_PILL) {
            #ifdef DEMONSTRATE
            Beeper.Beep(ShortBeep);
            #endif
            // Read med
            if(Pill[0].Connected) {
                Pill[0].Read((uint8_t*)&Med, sizeof(Med_t));
                ArmletApi::OnPillConnect(Med.CureID, Med.Charges);
            }
        }

        if(EvtMsk & EVTMASK_IR) {
            uint16_t w = IR.RxWord;
            Lustra.SetID(w >> 8);
          //  #ifdef DEMONSTRATE
//            Uart.Printf("IR ID=%u\r", Lustra.IDForApp);
           // Beeper.Beep(ShortBeep);
           // #endif
        }

//this is done periodically
void RFillPkt(uint8_t *Ptr, uint8_t *PLen) {
    *Ptr++ = Lustra.IDForRadio;
    memcpy(Ptr, AppState, APP_STATE_LEN);
    *PLen = APP_STATE_LEN+1;    // state len and lustra
}


// Pill
bool ArmletApi::WritePill(int cure_id, int charges) {
    if(!Pill[0].Connected) return false;
    Med.CureID = cure_id;
    Med.Charges = charges;
    return (Pill[0].Write((uint8_t*)&Med, sizeof(Med_t)) == OK);
}
void ArmletApi::SendRadioPacket(unsigned char* packet, int len) {
    rLevel1.AddPktToTx(0, packet, len);
}
unsigned short ArmletApi::GetArmletId() { return rLevel1.GetID(); }
void ArmletApi::SendAppState(unsigned char packet[APP_STATE_LEN]) {
    chSysLock();
    memcpy(AppState, packet, APP_STATE_LEN);
    chSysUnlock();
}
void ArmletApi::GetRadioStatus(int* gate_id, int* signal_level) {
    int8_t FId, FSl;
    rLevel1.GetGateParams(&FId, &FSl);
    if(FId == R_NO_ID) *gate_id = -1;
    else *gate_id = FId;
    *signal_level = FSl;
}

//=========================== Command processing ===============================
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    uint8_t b;
    FRESULT res;
    switch(CmdCode) {
        case 0x01:
            b = OK;
            Uart.Cmd(0x90, &b, 1);
            break;

        case 0x51:  // GetID
            Uart.Printf("ID=%u\r", rLevel1.GetID());
            break;

        case 0x52:  // SetID
            b = PData[0];
            res = f_open(&SD.File, "settings.ini", FA_CREATE_ALWAYS | FA_WRITE);
            if(res == FR_OK) {
                f_printf(&SD.File, "[Radio]\r\nID=%u\r\n", b);
                f_close(&SD.File);
                Uart.Printf("Written\r");
            }
            rLevel1.SetID(b);
            Uart.Printf("New ID=%u\r", rLevel1.GetID());
            break;

        default: break;
    } // switch
}
#endif