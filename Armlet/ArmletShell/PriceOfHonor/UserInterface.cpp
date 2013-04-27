#include "UserInterface.h"


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

	fres = txtBatteryStatus.SetText("99\n %");
	if (fres!= SUCCESS)
		return fres;

	return SUCCESS;
}
