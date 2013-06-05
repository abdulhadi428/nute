// Roman
// UserInterface содержит методы взаимодействия с пользователем, реакции на события и на нажатия кнопок
#pragma once
#include "UIDesigner.h"
#include "ArmletShell.h"
#include "Med.h"



// Описание основного класса интерфейса
class UserInterface : public UIDesigner
{


//	sword_t GetRoomIdFromLustraId(sword_t lustraId);
	
//	fresult AddNewWound(TARGET bodyPart);
// перерисовать
	fresult RedrawIfForeground(Form* form);

	void LogError(char* message);
	
//	bool_t _woundSelected;
public: 
	fresult Init();

	//void OnPillConnected(sword_t cure_id, sword_t charges);
	void SetPlayerName(char* name);
	//void SetRoom(sword_t room);
	fresult SetBatteryLevel(ubyte_t batteryLevel);
	//fresult SetRadioStatus(sword_t gateId, sword_t level);
	//void OnExplosion(sword_t room);
	void OnServerMessage(char* msg);
	void SetFPH(ubyte_t regenRate);
	bool OnOsanweTimer();
	//void DoMedTick();
	bool OnBatteryTimer();
	void Int2Str(uword_t value, char* buff, ubyte_t buffSize);

};