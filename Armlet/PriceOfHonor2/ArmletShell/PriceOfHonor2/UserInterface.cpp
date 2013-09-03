#include "ArmletShell.h"
#include "UserInterface.h"
#include "OpenSpaceImages.h"
#include "ArmletApi.h"
#include "AppMain.h"

#pragma warning(disable:4100)

#ifdef _MSC_VER
#define ZERO_ROOM_LUSTRAS_COUNT 2
static const int _gZeroRoomLustras[ZERO_ROOM_LUSTRAS_COUNT] = {12,15};
#else
#define ZERO_ROOM_LUSTRAS_COUNT 1
static const int _gZeroRoomLustras[ZERO_ROOM_LUSTRAS_COUNT] = {72};
#endif

fresult UserInterface::Init()
{
	fresult fres;
	fres = UIDesigner::Init();
	if (fres!= SUCCESS)
		return fres;

	char buff[4];
	buff[3] =0;
	_armletId = ArmletApi::GetArmletId();
	Int2Str(_armletId, (char*) buff,4);

	AppendLog(LOG_SYSTEM, "\nArmletId:");
	AppendLog(LOG_SYSTEM, buff);

	AppendLog(LOG_SYSTEM, "\nFirmware:");
	AppendLog(LOG_SYSTEM, FIRMWARE_VERSION);

	fres = ShowForm(&_frmMainForm);
	if (fres != SUCCESS)
		return fres;

	//init lustra
	_lastKnownDiscoveryTime = ArmletApi::GetUpTime();
	_lastKnownRoomId =0;
	_lastKnownLustraId = UNKNOWN_ID;
	//Launch lustra query Timer
	ArmletApi::RequestTimer(_QueryLustraTimerCallback, LUSTRA_POLL_TIME);

	//launch battery status timer
	ArmletApi::RequestTimer(_QueryBatteryStatusTimerCallback, BATTERYSTATUS_POLL_TIME);

	//launch medicine
	ArmletApi::RequestTimer(_MedicineTimerTickCallback, MED_TICK_TIME);

	return SUCCESS;
}


fresult UserInterface::OnMainMnuShot(IMenuItem* sender)
{
	_woundSelected=FALSE;
	return ShowForm(&_frmWoundForm);
}

fresult UserInterface::OnButtonPressed( ButtonState button )
{
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuHead( IMenuItem* sender )
{
	AddNewWound(HeadTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuStomach( IMenuItem* sender )
{
	AddNewWound(AbdomenTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuLeftArm( IMenuItem* sender )
{
	AddNewWound(LeftArmTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuRightArm( IMenuItem* sender )
{
	AddNewWound(RightArmTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuChest( IMenuItem* sender )
{
	AddNewWound(ChestTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuBack( IMenuItem* sender )
{
	AddNewWound(BackTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuLeftLeg( IMenuItem* sender )
{
	AddNewWound(LeftLegTarget);
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuRightLeg( IMenuItem* sender )
{
	AddNewWound(RightLegTarget);
	return SUCCESS;
}

void UserInterface::OnPillConnected( sword_t cure_id, sword_t charges )
{
	fresult fres;
	
	//Check if there are charges
	if (charges==0)
	{
		fres=MessageBoxShow("Не удалось!", "Емкость с препаратом пуста!", BlueWarning);
		if (fres!=SUCCESS)	
		{
			LogError("Cant't show msgbox!");
			return;
		}
	}
	else
	{
		//Call medicine
		const char* str = _medOnPillConnected(cure_id);

		fres=MessageBoxShow("Препарат применен!", str , BlueHealth);
		if (fres!=SUCCESS)	
		{
			LogError("Cant't show msgbox!");
			return;
		}

	if(!ArmletApi::WritePill(cure_id, charges-1))
	{
		LogError("Cant't write pill!");
		return;
	}
	}
}

void UserInterface::SetPlayerName( char* name )
{
	fresult fres;

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

fresult UserInterface::SetRadioStatus(sword_t gateId, sword_t level)
{
	fresult fres;

	char sBuff[5];
	sBuff[4] = 0;

	int ret;

	if (gateId >= 0)
		ret = ArmletApi::snprintf(sBuff, 4, " G%d", gateId);
	else
		ret = ArmletApi::snprintf(sBuff, 4, " G--");
	
	if (ret == 0)
		return GENERAL_ERROR;

	fres = txtMainFormGateId.SetText(sBuff);
	if (fres!=SUCCESS)
	{
		return fres;
	}

	sword_t formatLevel = level*-1;
	
	if (formatLevel >= 0 && formatLevel <100)
		ret = ArmletApi::snprintf(sBuff, 4, " L%d", formatLevel);
	else if (formatLevel >100)
		ret = ArmletApi::snprintf(sBuff, 4, " %d", formatLevel);
	else
		ret = ArmletApi::snprintf(sBuff, 4, " ");

	if (ret ==0)
		return GENERAL_ERROR;

	fres = fres = txtMainFormSignal.SetText(sBuff);
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

void UserInterface::SetRoom( sword_t room )
{
	fresult fres;
	char sRoom[4];
	sRoom[3] = 0;

	if (room == UNKNOWN_ID)
		ArmletApi::snprintf(sRoom, 4, "???");
	else if (room == 0)
		ArmletApi::snprintf(sRoom, 4, "---");
	else if ((room>0)&&(room<90))
		ArmletApi::snprintf(sRoom, 4, "r%d", room);
	else 
		ArmletApi::snprintf(sRoom, 4, "r99");

	fres = txtRoomId.SetText(sRoom);
	if (fres!=SUCCESS)
	{
		LogError("Can't set text to txtRoomId!");
		return;
	}

	fres= RedrawIfForeground(&_frmMainForm);
	if (fres!=SUCCESS)
	{
		LogError("Can't redraw _frmMainForm");
		return;
	}
}

fresult UserInterface::OnMainMnuKnockout(IMenuItem* sender)
{
	fresult fres;

	char* knockoutResult = _medOnKnockout();

	char* medstatus;
	_medAfterWoundUpdate(&medstatus);
	SetMedStatus(medstatus);


	fres = MessageBoxShow("Нокаут!", knockoutResult, BlueHealth);
	if (fres!=SUCCESS)	
	{
		LogError("Cant't show msgbox!");
		return GENERAL_ERROR;
	}

	return SUCCESS;
}

void UserInterface::OnExplosion( sword_t roomId )
{
	fresult fres;
	if (roomId==_lastKnownRoomId)
	{
		char* explosionResult = _medOnExplosion();
		char* medstatus;
		_medAfterWoundUpdate(&medstatus);
		SetMedStatus(medstatus);


		fres = MessageBoxShow("В отсеке взрыв!", explosionResult, BlueHealth);
		if (fres!=SUCCESS)	
		{
			LogError("Cant't show msgbox!");
			return;
		}
	}
}

void SetRegenerationRate(sword_t regenRate)
{
	return _medSetRegenerationRate(regenRate);
}

void UserInterface::OnServerMessage(char* msg)
{
	fresult fres;

	fres = MessageBoxShow("Новое сообщение!", msg, BlueMail);
	if (fres!=SUCCESS)	
	{
		LogError("Cant't show msgbox!");
		return;
	}
	ArmletApi::DoVibroAndBeep(500);

	fres = AppendLog(LOG_MESSAGE, "\n");
	if (fres!=SUCCESS)	
	{
		LogError("Cant't write message log!");
		return;
	}

	fres = AppendLog(LOG_MESSAGE, msg);
	if (fres!=SUCCESS)	
	{
		LogError("Cant't write message log!");
		return;
	}
}

bool UserInterface::OnLustraTimer()
{
	//Query current lustraid
	ubyte_t currLustraId = ArmletApi::GetLustraId();
	sword_t currRoomId = GetRoomIdFromLustraId(currLustraId); 
	int currTime = ArmletApi::GetUpTime();
	
	
	//char strBuff[40];
	//ArmletApi::snprintf(strBuff, 40, "t:%d, r:%d", currTime,currLustraId);
	//AppendLog(LOG_EVENT, strBuff);

	if(currLustraId == UNKNOWN_ID)
	{
		if (_lastKnownRoomId == 0)
		{
			//zero room, all is ok
	//		AppendLog(LOG_EVENT, "продолжаем быть в 0 комнате");
		}
		else
		{
			//check if we had a good room no longer that specified time
			if (currTime - _lastKnownDiscoveryTime > MAX_LUSTRA_WAIT_TIME)
			{
				ArmletApi::DoVibroAndBeep(DOVIBRO_TIME);
				MessageBoxShow("Потеря связи!", "Срочно покажи браслет системе безопасности отсека!", BlueExclamation);
				
				if(_lastLoggedTime ==-1 || currTime - _lastKnownRoomId)
				{
					//AppendLog(LOG_EVENT, "\n Нет активного отсека!");
					_lastLoggedTime = currTime;
				}
				SetRoom(currRoomId);
				//AppendLog(LOG_EVENT, "продолжаем не видеть");
			}
		}
	}
	else
	{
		//good lustra
		_lastLoggedTime = -1;
		_lastKnownDiscoveryTime = currTime;
		if ((currRoomId!= _lastKnownRoomId)||(currLustraId != _lastKnownLustraId))
		{
			SetRoom(currRoomId);
		}
		_lastKnownRoomId = currRoomId;
                _lastKnownLustraId = currLustraId;
	}
	return true;
}

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

	int GateId=25;
	int Level=-99;

	ArmletApi::GetRadioStatus(&GateId, &Level);
	//hint for level values
	//-35 good
	//-100 bad

	fres = SetRadioStatus(GateId, Level);
	if (fres!=SUCCESS)	
	{
		LogError("Cant't set radio status level");
		return true;
	}

	return true;
}


sword_t UserInterface::GetRoomIdFromLustraId(sword_t lustraId)
{
	sword_t roomId=lustraId;
	
	for(ubyte_t i=0; i< ZERO_ROOM_LUSTRAS_COUNT; i++)
	{
		if(lustraId == _gZeroRoomLustras[i])
		{
			roomId = 0;
			break;
		}
	}

	return roomId;
}

void UserInterface::DoMedTick()
{
	fresult fres;

	char* symptoms;
	char* medLog;

	_medOnMedTick(&medLog);
	fres = SetMedStatus(medLog);
	if (fres!=SUCCESS)
	{
		LogError("Can't set med status!");
		return;
	}

	fres = RedrawIfForeground(&_frmMainForm);
	if (fres!=SUCCESS)
	{
		LogError("Can't redraw main form!");
		return;
	}
}

void UserInterface::Int2Str(uword_t value, char* buff, ubyte_t buffSize)
{
	ArmletApi::snprintf(buff, buffSize, "%d", value);
}

fresult UserInterface::AddNewWound(TARGET bodyPart)
{
	fresult fres;
	
	if(_woundSelected!= TRUE)
	{
	char* result=_medNewWound(bodyPart);
	fres = txtWoundResult.SetText(result);
	if(fres!=SUCCESS)
		return fres;

		char* medstatus;
		_medAfterWoundUpdate(&medstatus);
		SetMedStatus(medstatus);

	RedrawIfForeground(&_frmWoundForm);
	if(fres!=SUCCESS)
		return fres;

		_woundSelected = TRUE;
	}

	return SUCCESS;
}

fresult UserInterface::RedrawIfForeground(Form_DEPRECATED* form)
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
