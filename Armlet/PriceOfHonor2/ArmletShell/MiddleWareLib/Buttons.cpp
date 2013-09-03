#include "ArmletShell.h"

namespace ArmletShell {

} //namespace

//TODO managed buttons algorithm  => TODO here
/*
	static array<Int64>^ ButtonTicks;
	static array<Boolean>^ ButtonPressed;
	static array<int>^ ButtonHolded;
	static array<Boolean>^ SkipButtonRelease;

	ButtonTicks = gc0; // new array<Int64>(N_BUTTONS);
	ButtonPressed = gc0; // new array<Boolean>(N_BUTTONS);
	ButtonHolded = gc0; // new array<int>(N_BUTTONS);
	SkipButtonRelease = gc0; // new array<Boolean>(N_BUTTONS);

	void FormHelper::OnButtonPress(int button)
	{
		if (ButtonPressed[button])
			return;
		Log(String::Format("Button {0} down",GetButtonName(button)));
		ButtonPressed[button] = true;
		ButtonHolded[button] = 0;
		ButtonTicks[button] = DateTime::Now.Ticks;
		SkipButtonRelease[button] = false;
	}

	void FormHelper::OnButtonRelease(int button)
	{
		if (!ButtonPressed[button])
			return;
		ButtonPressed[button] = false;
		if (SkipButtonRelease[button]) {
			SkipButtonRelease[button] = false;
			return;
		}
		
		Log(String::Format("Button {0} up",GetButtonName(button)));
		__int64 ticks = DateTime::Now.Ticks - ButtonTicks[button];
		int msecs = (int)(ticks / 10000);
		Log(String::Format("{0} msecs elapsed", msecs));
		if (msecs>=0) {
			Log(String::Format("Button {0} clicked",GetButtonName(button)));
			 OnButtonClick(button);
		 }
	}

	String^ GetButtonName(int button)
	{
		if (button==BUTTON_A) return "A";
		if (button==BUTTON_B) return "B";
		if (button==BUTTON_C) return "C";
		if (button==BUTTON_X) return "X";
		if (button==BUTTON_Y) return "Y";
		if (button==BUTTON_Z) return "Z";
		if (button==BUTTON_L) return "L";
		if (button==BUTTON_E) return "E";
		if (button==BUTTON_R) return "R";
		return "?";
	}

	void FormHelper::HalfSecondTick()
	{
		for (int button=0; button<N_BUTTONS; button++)
		{
			if (!ButtonPressed[button])
				continue;
			__int64 ticks = DateTime::Now.Ticks - ButtonTicks[button];
			int secs = (int)(ticks / 10000000);
			if (secs > ButtonHolded[button]) {
				if (!SkipButtonRelease[button]) {
					SkipButtonRelease[button] = true;
					Log(String::Format("Button {0} was holded 1 sec SKIP RELEASE",GetButtonName(button)));
				}
				ButtonHolded[button] = secs;
				Log(String::Format("Button {0} was holded +1 {1}",GetButtonName(button),secs));
				OnButtonHold(button, secs);
			}
		}
	}
*/