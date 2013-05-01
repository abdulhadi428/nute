#include "ArmletShell.h"
#include "UserInterface.h"
#include "Images.h"
#include "ArmletApi.h"

#pragma warning(disable:4100)

fresult UserInterface::Init()
{
	fresult fres;
	fres = UIDesigner::Init();
	if (fres!= SUCCESS)
		return fres;

	//Setting texts
	fres = txtUserName.SetText("Вася\nХуйкин");
	if (fres!= SUCCESS)
		return fres;

	fres = txtBatteryStatus.SetText("99%");
	if (fres!= SUCCESS)
		return fres;

	fres = txtRoomId.SetText("r13");
	if (fres!= SUCCESS)
		return fres;

	fres = txtMainLog.SetText("О войне. О том, как вы поднимитесь в последнюю атаку. О том, как вы умрете, но не сдадитесь. О том, как вы станете героями ради своей страны.\n"
		"О чести. О высоких идеалах, о любви к своей стране, о крепкой дружбе и верности товарищу.\n"
		"О моменте, когда со смертью ты останешься один на один.\n"
		"О моменте, когда ты заплатишь сполна.");
	if (fres!= SUCCESS)
		return fres;


	return SUCCESS;
}

fresult UserInterface::OnMainMnuHelp(IMenuItem* sender)
{
	return SUCCESS;
}

fresult UserInterface::OnMainMnuScrollUp(IMenuItem* sender)
{
	return txtMainLog.ScrollUp();
}

fresult	UserInterface::OnMainMnuScrollDown(IMenuItem* sender)
{
	return txtMainLog.ScrollDown();
}

fresult UserInterface::OnMainMnuCycleLeft(IMenuItem* sender)
{
	return SUCCESS;
}

fresult UserInterface::OnMainMnuShot(IMenuItem* sender)
{
	return ShowForm(&_frmWoundForm);
}

fresult UserInterface::OnMainMnuKnockout(IMenuItem* sender)
{
	return SUCCESS;
}

fresult UserInterface::OnMainMnuOpenLock(IMenuItem* sender)
{
	MessageBoxShow("Открываем замок!", "Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		"Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		"Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		"Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		"Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		"Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		"Хер там! У этого замка такая защщита, что можно голову сломать пополам. А может быть просто в браслете сломалось NFC? Блин! Замки то по ИК работают, а оно не заимплементено!"
		, BlueInfo2);
	return SUCCESS;
}

fresult UserInterface::OnMainMnuCycleRight(IMenuItem* sender)
{
	return SUCCESS;
}

fresult UserInterface::OnButtonPressed( ButtonState button )
{
	txtMainLog.SetText("Menu not bound!");
	return SUCCESS;
}

fresult UserInterface::OnWoundMnuHead( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Ой! Что-то шлепнуло по голове! Болит!\n");;
}

fresult UserInterface::OnWoundMnuStomach( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Живот что-то как-то нехорошо крутит..\n");;
}

fresult UserInterface::OnWoundMnuLeftArm( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Теперь на левой руке здоровый порез. Болит, блин.\n");
}

fresult UserInterface::OnWoundMnuRightArm( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Правая рука вся опухла. Стремно...\n");
}

fresult UserInterface::OnWoundMnuChest( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Откудато взялась тянущая боль в груди. Неужели сердце сдает???\n");
}

fresult UserInterface::OnWoundMnuBack( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Спину ломит. Остеохондроз что-ли опять? \n");
}

fresult UserInterface::OnWoundMnuLeftLeg( IMenuItem* sender )
{
	return txtWoundResult.AppendText("Ну вот! Теперь я хромаю на левую ногу. Зашибись!\n");
}

fresult UserInterface::OnWoundMnuRightLeg( IMenuItem* sender )
{
	return txtWoundResult.AppendText("На правую ногу просто не наступить! Жуткая боль!\n");
}


void UserInterface::OnPillConnected( sword_t cure_id, sword_t charges )
{
	//Check if there are charges
	if (charges==0)
	{
		MessageBoxShow("Не удалось!", "Емкость с препаратом пуста!", BlueWarning);
	}
	else
	{
		MessageBoxShow("Препарат применен!", "Инъекция препарата прошла успешно.", BlueHealth);
	}

	ArmletApi::WritePill(cure_id, charges-1);

	//Call medicine
}

void UserInterface::SetPlayerName( char* name )
{
	txtUserName.SetText(name);

	if (currentForm == &_frmMainForm)
	{
		Draw();
	}
}

void UserInterface::SetBatteryLevel( ubyte_t batteryLevel )
{
	char sBatteryLevel[4];
	sBatteryLevel[3] = 0;

	int ret = ArmletApi::snprintf(sBatteryLevel, 4, "%d\%", batteryLevel);
	
	txtBatteryStatus.SetText(sBatteryLevel);
	
	if (currentForm == &_frmMainForm)
	{
		Draw();
	}
}

void UserInterface::SetRoom( sword_t room )
{
	_roomId = room;

	char sRoom[4];
	sRoom[3] = 0;

	if (room >= 0) 
		ArmletApi::snprintf(sRoom, 4, "r%d", room);
	else
		ArmletApi::snprintf(sRoom, 4, "%d", room);

	txtBatteryStatus.SetText(sRoom);

	if (currentForm == &_frmMainForm)
	{
		Draw();
	}
}

void UserInterface::OnExplosion( sword_t room )
{
	if (room==_roomId)
	{
		int explosionType = ArmletApi::GetRandom(2);

	}
}
