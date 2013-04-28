#include "StdAfx.h"

namespace Visualizer {

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
		return "???";
	}

void VisualizerInterface::Init(TextBox^ _LogWindow)
{
	Bmp = gcnew Bitmap(SCREENX, SCREENY, PixelFormat::Format24bppRgb);
	LogWindow = _LogWindow;
	ButtonTicks = gcnew array<Int64>(N_BUTTONS);
	ButtonPressed = gcnew array<Boolean>(N_BUTTONS);
	ButtonHolded = gcnew array<int>(N_BUTTONS);
	SkipButtonRelease = gcnew array<Boolean>(N_BUTTONS);	
}

void VisualizerInterface::Log(String^ message)
{
	LogWindow->Text += (message + "\r\n");
	LogWindow->SelectionStart = LogWindow->Text->Length;
	LogWindow->ScrollToCaret();
	//LogWindow->Refresh();
}

int VisualizerInterface::KeyToButton(System::Windows::Forms::Keys key)
{
	int button = NO_BUTTON;
	do {
		if (key == Keys::A) {
			button = BUTTON_A;
			break;
		}
		if (key == Keys::B) {
			button = BUTTON_B;
			break;
		}
		if (key == Keys::C) {
			button = BUTTON_C;
			break;
		}
		if (key == Keys::X) {
			button = BUTTON_X;
			break;
		}
		if (key == Keys::Y) {
			button = BUTTON_Y;
			break;
		}
		if (key == Keys::Z) {
			button = BUTTON_Z;
			break;
		}
		if (key == Keys::L) {
			button = BUTTON_L;
			break;
		}
		if (key == Keys::E) {
			button = BUTTON_E;
			break;
		}
		if (key == Keys::R) {
			button = BUTTON_R;
			break;
		}
	} while(0);
	return button;
}

void VisualizerInterface::OnButtonPress(int button)
{
	if (ButtonPressed[button])
		return;
	Log(String::Format("Button {0} down",GetButtonName(button)));
	ButtonPressed[button] = true;
	ButtonHolded[button] = 0;
	ButtonTicks[button] = DateTime::Now.Ticks;
	SkipButtonRelease[button] = false;
}

void VisualizerInterface::OnButtonRelease(int button)
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

void VisualizerInterface::HalfSecButtonTimer()
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

void VisualizerInterface::Render()
{
	for (int y=0; y<128; y++)
	for (int x=0; x<160; x++)
	{
		short val = VideoMemory[160*y+x];
		char r = (val & 0xF00) >> 8;
		char g = (val & 0xF0) >> 4;
		char b = val & 0xF;
		Bmp->SetPixel(
			x,
			y,
			Color::FromArgb(0, r*16, g*16, b*16)); //TODO LUT
	}
	OnElapsed50msec();
}

}