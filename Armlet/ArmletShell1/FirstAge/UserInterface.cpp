// Реализация класса интерфейса пользователя
#include "ArmletShell.h"
#include "UserInterface.h"
#include "Images.h"
#include "ArmletApi.h"
#include "AppMain.h"
#include "Osanve.h"
#include <string.h>

#pragma warning(disable:4100)

// инит
fresult UserInterface::Init()
{
	fresult fres;
	fres = UIDesigner::Init(); 
	if (fres!= SUCCESS)
		return fres;

	char buff[4];
	buff[3] =0;
	// получем аппаратный ID
//	Player._armletId = ArmletApi::GetArmletId();
	//init parameters
	Player.osanve = OSANVE_NORM;
	Player.status = AL_STATUS_OSANVE;
	Int2Str(Player.userId, (char*) buff,4);
// Запишем в лог все ID
	AppendLog(LOG_SYSTEM, "\nArmletId:");
	AppendLog(LOG_SYSTEM, buff);
	AppendLog(LOG_SYSTEM, "\nFirmware:");
	AppendLog(LOG_SYSTEM, FIRMWARE_VERSION);
// показ формы
	fres = ShowForm(&_frmMainForm);
	if (fres != SUCCESS)
		return fres;

    PlayerInit();

 //Launch osanve send Timer (10 sec)
	ArmletApi::RequestTimer(_OsanveSendTimer, OSANVE_SEND_TIME);
		
//launch force regeneration timer (1 min)
	ArmletApi::RequestTimer(_ForceTimerTickCallback, MED_TICK_TIME);

	//launch battery status timer
	ArmletApi::RequestTimer(_QueryBatteryStatusTimerCallback, BATTERYSTATUS_POLL_TIME);

	return SUCCESS;
}

// дополнительный обработчик всех кнопок
fresult UserInterface::OnButtonPressed( ButtonState button )
{
	return SUCCESS;
}

//fresult UserInterface::OnWoundMnuHead( IMenuItem* sender )
//{
//	AddNewWound(HeadTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuStomach( IMenuItem* sender )
//{
//	AddNewWound(AbdomenTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuLeftArm( IMenuItem* sender )
//{
//	AddNewWound(LeftArmTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuRightArm( IMenuItem* sender )
//{
//	AddNewWound(RightArmTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuChest( IMenuItem* sender )
//{
//	AddNewWound(ChestTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuBack( IMenuItem* sender )
//{
//	AddNewWound(BackTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuLeftLeg( IMenuItem* sender )
//{
//	AddNewWound(LeftLegTarget);
//	return SUCCESS;
//}
//
//fresult UserInterface::OnWoundMnuRightLeg( IMenuItem* sender )
//{
//	AddNewWound(RightLegTarget);
//	return SUCCESS;
//}
//
//void UserInterface::OnPillConnected( sword_t cure_id, sword_t charges )
//{
//	fresult fres;
//	
//	//Check if there are charges
//	if (charges==0)
//	{
//		fres=MessageBoxShow("Не удалось!", "Емкость с препаратом пуста!", BlueWarning);
//		if (fres!=SUCCESS)	
//		{
//			LogError("Cant't show msgbox!");
//			return;
//		}
//	}
//	else
//	{
//		//Call medicine
//		const char* str = _medOnPillConnected(cure_id);
//
//		fres=MessageBoxShow("Препарат применен!", str , BlueHealth);
//		if (fres!=SUCCESS)	
//		{
//			LogError("Cant't show msgbox!");
//			return;
//		}
//
//	if(!ArmletApi::WritePill(cure_id, charges-1))
//	{
//		LogError("Cant't write pill!");
//		return;
//	}
//	}
//}
// Установить имя
void UserInterface::SetPlayerName( char* name )
{
  fresult fres;
  strncpy( Player.name, name, 16);
  fres = txtUserName.SetText(name);
  if (fres!=SUCCESS)
	{
		LogError("Can't set text to txtUserName!");
		return;
	}

  fres= RedrawIfForeground(&_frmMainForm);
  if (fres!=SUCCESS)
	{
		LogError("Can't redraw _frmMainForm");
		return;
	}
}

// прорисовка уровня батареи
fresult UserInterface::SetBatteryLevel( ubyte_t batteryLevel )
{
	fresult fres;
	
	char sBatteryLevel[4];
	sBatteryLevel[3] = 0;

	int ret;
	if (batteryLevel<10) {
		ret = ArmletApi::snprintf(sBatteryLevel, 4, " %d%c", batteryLevel, '%');
	} else if (batteryLevel<100) {
		ret = ArmletApi::snprintf(sBatteryLevel, 4, "%d%c", batteryLevel, '%');
	} else {
		ret = ArmletApi::snprintf(sBatteryLevel, 4, "%d", batteryLevel);
	}
	
	fres = txtBatteryStatus.SetText(sBatteryLevel);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	fres= RedrawIfForeground(&_frmMainForm);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	return SUCCESS;
}

//fresult UserInterface::SetRadioStatus(sword_t gateId, sword_t level)
//{
//	fresult fres;
//
//	char sBuff[5];
//	sBuff[4] = 0;
//
//	int ret;
//
//	if (gateId >= 0)
//		ret = ArmletApi::snprintf(sBuff, 4, " G%d", gateId);
//	else
//		ret = ArmletApi::snprintf(sBuff, 4, " G--");
//	
//	if (ret == 0)
//		return GENERAL_ERROR;
//
//	fres = txtMainFormGateId.SetText(sBuff);
//	if (fres!=SUCCESS)
//	{
//		return fres;
//	}
//
//	sword_t formatLevel = level*-1;
//	
//	if (formatLevel >= 0 && formatLevel <100)
//		ret = ArmletApi::snprintf(sBuff, 4, " L%d", formatLevel);
//	else if (formatLevel >100)
//		ret = ArmletApi::snprintf(sBuff, 4, " %d", formatLevel);
//	else
//		ret = ArmletApi::snprintf(sBuff, 4, " ");
//
//	if (ret ==0)
//		return GENERAL_ERROR;
//
//	fres = fres = txtMainFormSignal.SetText(sBuff);
//	if (fres!=SUCCESS)
//	{
//		return fres;
//	}
//
//	fres= RedrawIfForeground(&_frmMainForm);
//	if (fres!=SUCCESS)
//	{
//		return fres;
//	}
//
//	return SUCCESS;
//}
//
//void UserInterface::SetRoom( sword_t room )
//{
//	fresult fres;
//	char sRoom[4];
//	sRoom[3] = 0;
//
//	if (room == UNKNOWN_ID)
//		ArmletApi::snprintf(sRoom, 4, "???");
//	else if (room == 0)
//		ArmletApi::snprintf(sRoom, 4, "---");
//	else if ((room>0)&&(room<90))
//		ArmletApi::snprintf(sRoom, 4, "r%d", room);
//	else 
//		ArmletApi::snprintf(sRoom, 4, "r99");
//
//	fres = txtRoomId.SetText(sRoom);
//	if (fres!=SUCCESS)
//	{
//		LogError("Can't set text to txtRoomId!");
//		return;
//	}
//
//	fres= RedrawIfForeground(&_frmMainForm);
//	if (fres!=SUCCESS)
//	{
//		LogError("Can't redraw _frmMainForm");
//		return;
//	}
//}
//
//fresult UserInterface::OnMainMnuKnockout(IMenuItem* sender)
//{
//	fresult fres;
//
//	char* knockoutResult = _medOnKnockout();
//
//	char* medstatus;
//	_medAfterWoundUpdate(&medstatus);
//	SetMedStatus(medstatus);
//
//
//	fres = MessageBoxShow("Нокаут!", knockoutResult, BlueHealth);
//	if (fres!=SUCCESS)	
//	{
//		LogError("Cant't show msgbox!");
//		return GENERAL_ERROR;
//	}
//
//	return SUCCESS;
//}
//
//void UserInterface::OnExplosion( sword_t roomId )
//{
//	fresult fres;
//	if (roomId==_lastKnownRoomId)
//	{
//		char* explosionResult = _medOnExplosion();
//		char* medstatus;
//		_medAfterWoundUpdate(&medstatus);
//		SetMedStatus(medstatus);
//
//
//		fres = MessageBoxShow("В отсеке взрыв!", explosionResult, BlueHealth);
//		if (fres!=SUCCESS)	
//		{
//			LogError("Cant't show msgbox!");
//			return;
//		}
//	}
//}


// Сообщение (всплывающее) на всяк случай
//void UserInterface::OnServerMessage(char* msg)
//{
//	fresult fres;
//
//	fres = MessageBoxShow("Новое сообщение!", msg, BlueMail);
//	if (fres!=SUCCESS)	
//	{
//		LogError("Cant't show msgbox!");
//		return;
//	}
//
//	fres = AppendLog(LOG_MESSAGE, "\n");
//	if (fres!=SUCCESS)	
//	{
//		LogError("Cant't write message log!");
//		return;
//	}
//// дубль в лог
//	fres = AppendLog(LOG_MESSAGE, msg);
//	if (fres!=SUCCESS)	
//	{
//		LogError("Cant't write message log!");
//		return;
//	}
//}

// время послать осанву и перерисовать список видимых побратимов
bool UserInterface::OnOsanveTimer()
{
	// Send current osanve
	unsigned char osanve_packet[4];

    GetOsanvePacket(osanve_packet);
	
	ArmletApi::SendRadioPacket( osanve_packet, 4);

	SetOsanve();

	return true; // always true - next period timer
}

// время опросить батарейку
bool UserInterface::OnBatteryTimer()
{
	fresult fres;
	
	ubyte_t currBattery = ArmletApi::GetBatteryLevel();
	fres = SetBatteryLevel(currBattery);
	if (fres!=SUCCESS)	
	{
		LogError("Cant't set battery level");
		return true;
	}

	return true;
}

// время рубица!
bool UserInterface::OnPreFightTimer()
{
	PreFightEnd();
	return false; // всегда выход из таймера
}

//sword_t UserInterface::GetRoomIdFromLustraId(sword_t lustraId)
//{
//	sword_t roomId=lustraId;
//	
//	for(ubyte_t i=0; i< ZERO_ROOM_LUSTRAS_COUNT; i++)
//	{
//		if(lustraId == _gZeroRoomLustras[i])
//		{
//			roomId = 0;
//			break;
//		}
//	}
//
//	return roomId;
//}

// Roman таймер восстановления сил
void UserInterface::DoForceTick()
{
	fresult fres;
	
	if( OnForceTick()) {
	  ShowForm(&_frmMainForm);
	  MessageBoxClose();
	}

	fres = SetForces();
	if (fres!=SUCCESS)
	{
		LogError("Can't set forces status!");
		return;
	}

	fres = RedrawIfForeground(&_frmMainForm);
	if (fres!=SUCCESS)
	{
		LogError("Can't redraw osanve form!");
		return;
	}
}

void UserInterface::Int2Str(uword_t value, char* buff, ubyte_t buffSize)
{
	ArmletApi::snprintf(buff, buffSize, "%d", value);
}

//fresult UserInterface::AddNewWound(TARGET bodyPart)
//{
//	fresult fres;
//	
//	if(_woundSelected!= TRUE)
//	{
//	char* result=_medNewWound(bodyPart);
//	fres = txtWoundResult.SetText(result);
//	if(fres!=SUCCESS)
//		return fres;
//
//		char* medstatus;
//		_medAfterWoundUpdate(&medstatus);
//		SetMedStatus(medstatus);
//
//	RedrawIfForeground(&_frmWoundForm);
//	if(fres!=SUCCESS)
//		return fres;
//
//		_woundSelected = TRUE;
//	}
//
//	return SUCCESS;
//}

// перерисовать форму, если она текущая
fresult UserInterface::RedrawIfForeground(Form* form)
{
	fresult fres;
	
	if (currentForm == form)
	{
		fres = Draw();
		if(fres!=SUCCESS)
			return fres;
	}

	return SUCCESS;
}

void UserInterface::LogError(char* message)
{
	AppendLog(LOG_SYSTEM, "\nERROR:"); 
	AppendLog(LOG_SYSTEM, message);

	MessageBoxShow("ОШИБКА!", message, RedCancel);
}
