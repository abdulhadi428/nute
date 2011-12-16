/*
 * lock_acttion.h
 *
 *  Created on: 06.11.2011
 *      Author: Stefan
 */

#ifndef LOCK_ACTTION_H_
#define LOCK_ACTTION_H_

#include <inttypes.h>

#define MAX_PASS_LEN            8// максимальна€ длинна парол€
#define MAX_HACK_RX_BUF_SIZE    255 // размер буфера, дл€ приема команд по USART-у
#define MAX_HACK_COMMAND_LEN    255 // максимальна€ длинна команды от хакерского терминала
#define HACK_DELAY_TIME         10000 // врем€ перехода в нормальный режим, после отсоеденени€ хакера
//#define USB_RX_BUF_SIZE         64

#define SOUND_BEEP_PASS                 "beep_p.wav"  // звук правильно нажатой кнопки
#define SOUND_BEEP_FAIL                 "beep_f.wav" // «¬”  Ќ≈ѕ–ј¬»Ћ№Ќќ Ќјжј“ќ…  Ќќѕ »
//#define SOUND_UNLOCK                    "unlock.wav"  // замок открылс€
#define SOUND_LOCK_BLOCK_LONG           "block_l.wav"  // замок заблокирован, полна€ верси€
#define SOUND_LOCK_BLOCK_SHORT          "block_s.wav"  // замок заблокирован, сокращенна€ верси€
#define SOUND_LOCK_UNBOCK               "block_u.wav"  // замок разблокирован
#define SOUND_LOCK_OPEN                 "open.wav"  // замок открылс€, доступ разрешен
#define SOUND_LOCK_CLOSE                "close.wav"  // замок закрыт
#define SOUND_LOCK_WARNING              "close_w.wav"  // замок закрыт
#define SOUND_LOCK_REBOOT               "reboot.wav"  // перед перезагрузкой
//#define SOUND_LOCK_OPEN                 "open.wav"  // замок открылс€, доступ разрешен


// автомат нормального режима
#define NORMAL_START            11
#define NORMAL_ON               12
#define NORMAL_OPEN             13
#define NORMAL_CLOSE            14
#define NORMAL_BLOCK            15

// режимы замка
#define LOCK_NORMAL             01
#define LOCK_BREAKED            02
#define LOCK_HACKED             03
#define LOCK_RESTART            04

// команды от хакерского терминала
#define HACK_COMMAND_PREFIX_LEN         7
#define HACK_COMMAND_PREFIX             "PREFIX "
#define HACK_COMMAND_POSTFIX            "POSTFIX"
#define HACK_COMMAND_CODE_BLANK         0
#define HACK_COMMAND_TEST               "TEST"
#define HACK_COMMAND_CODE_TEST          1


class Lock_t {
private:
  bool bKeyFlag;
  uint8_t chNewKey;
  uint8_t chLockState;
  uint8_t chNormState;
  //uint8_t chUsbRxBuf[USB_RX_BUF_SIZE];
  uint8_t chUsbRxCounter;
  char chPassword[MAX_PASS_LEN];
  uint8_t chPasswordLength; // длинна парол€
  uint8_t chMaxCodeError; // количество допустимых ошибок, до блокировки
  uint8_t chKeyCount, chErrorCount;
  uint32_t wOpenTimer,wBlockTimer, wHackTimer;
  uint32_t wTimeToOpen, wTimeToBlock;

  bool bNewCommandFlag; // флаг получени€ новой команды
  uint8_t iNewCommandCode; // код новой команды

  bool GetCharFromUsb(uint8_t* pData);
  void NormalTask(void);
  void HackedTask(void);
  void ReadSettingsFromSD(void);
  void Open(void);
  void Close(void);

  char strRxBuf[MAX_HACK_RX_BUF_SIZE];
  char strCommand[MAX_HACK_COMMAND_LEN];
  uint16_t iRxCounter;

  void ParsCommand(void);
//    bool IsActivated;
//    bool SoundPlayed;
//    uint8_t ActivitySoundRepeatCount;
//    uint8_t Rnd13, Rnd19, Rnd0_97;   // Random numbers
//    uint8_t MaxV(uint8_t a, uint8_t b) { return ((a > b)? a : b); }
//    uint8_t MinV(uint8_t a, uint8_t b) { return ((a < b)? a : b); }
//    // Indication
//    void ShowChargeCount(void);
//    void DecreaseCharge(void);
//    // Inner use
//    char UintToSmallHexChar(uint8_t b) { return ((b<=0x09) ? (b+'0') : (b+'a'-10)); }
//    void AddRandURL(char *ALastSymbol);

public:
  uint8_t KeyPressed(uint8_t chKey);
  void UsbRecData(uint8_t* pData, uint8_t iSize);
  void UsartRxEvent(uint8_t chByte);
//    void ChooseType(void);
//    void Activate(void);
//    void ShowFieldExistance(FieldType_t AFType);
//    // User use
//	ArtType_t Type;
//	uint8_t ChargeCount;
//	void TryToActivate(ActType_t);
//	// Inner use
//	FieldPoint_t H, E, C;
	void Init(void);
	void Task(void);
//	void Charge(FieldType_t AFType);
};


extern Lock_t ELock;

#endif /* LOCK_ACTTION_H_ */
