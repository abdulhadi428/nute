#include "UserInterface.h"
#include "Icons.h"

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

	fres = txtBatteryStatus.SetText("99%\nr10");
	if (fres!= SUCCESS)
		return fres;

	fres = txtStatus.SetText("О войне. О том, как вы поднимитесь в последнюю атаку. О том, как вы умрете, но не сдадитесь. О том, как вы станете героями ради своей страны.\n"
		"О чести. О высоких идеалах, о любви к своей стране, о крепкой дружбе и верности товарищу.\n"
		"О моменте, когда со смертью ты останешься один на один.\n"
		"О моменте, когда ты заплатишь сполна.");
	if (fres!= SUCCESS)
		return fres;


	return SUCCESS;
}

fresult UserInterface::OnMainMnuHelp(IMenuItem* sender)
{
	txtStatus.SetText("OnMainMnuHelp");
	return SUCCESS;
}

fresult UserInterface::OnMainMnuScrollUp(IMenuItem* sender)
{
	return txtStatus.ScrollUp();
}

fresult	UserInterface::OnMainMnuScrollDown(IMenuItem* sender)
{
	return txtStatus.ScrollDown();
}

fresult UserInterface::OnMainMnuCycleLeft(IMenuItem* sender)
{
	txtStatus.SetText("OnMainMnuCycleLeft");
	return SUCCESS;
}

fresult UserInterface::OnMainMnuShot(IMenuItem* sender)
{
	return ShowForm(&_frmWoundForm);
}

fresult UserInterface::OnMainMnuKnockout(IMenuItem* sender)
{
	txtStatus.SetText("OnMainMnuKnockout");
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
		, ORANGE_ARROW_DOWN);
	return SUCCESS;
}

fresult UserInterface::OnMainMnuCycleRight(IMenuItem* sender)
{
	txtStatus.SetText("OnMainMnuCycleRight");
	return SUCCESS;
}

fresult UserInterface::OnButtonPressed( ButtonState button )
{
	txtStatus.SetText("Menu not bound!");
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
