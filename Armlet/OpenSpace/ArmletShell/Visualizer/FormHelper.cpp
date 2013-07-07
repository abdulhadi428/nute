#include "stdAfx.h"
#include "MainForm.h"

namespace Visualizer {

	int FormHelper::KeyToButton(System::Windows::Forms::Keys key)
	{
		if (key == Keys::A) return BUTTON_A;
		if (key == Keys::B) return BUTTON_B;
		if (key == Keys::C) return BUTTON_C;
		if (key == Keys::X) return BUTTON_X;
		if (key == Keys::Y) return BUTTON_Y;
		if (key == Keys::Z) return BUTTON_Z;
		if (key == Keys::L) return BUTTON_L;
		if (key == Keys::E) return BUTTON_E;
		if (key == Keys::R) return BUTTON_R;
		return NO_BUTTON;
	}

	void FormHelper::RenderScreen()
	{
		for (int y=0; y<SCREENY; y++)
		for (int x=0; x<SCREENX; x++)
		{
			unsigned short val = KernelApi::VideoMemory[SCREENX*y+x];
			unsigned char r = (val & 0xF00) >> 8;
			unsigned char g = (val & 0xF0) >> 4;
			unsigned char b = val & 0xF;
			bmp->SetPixel(
				x,
				y,
				Color::FromArgb(0, r*16, g*16, b*16));
		}
		KernelApi::CheckTimers();
	}

	void FormHelper::SetPlayerName(String^ name)
	{
		array<unsigned char>^ bytes = Encoding::GetEncoding(1251)->GetBytes(name);
		if (bytes->Length == 0)
			return;
		pin_ptr<unsigned char> pinned = &bytes[0];
		Callouts::SetPlayerName((char const*const)pinned);
	}

	void FormHelper::SendMessage(String^ msg)
	{
		array<unsigned char>^ bytes = Encoding::GetEncoding(1251)->GetBytes(msg);
		if (bytes->Length == 0)
			return;
		pin_ptr<unsigned char> pinned = &bytes[0];
		Callouts::SendMessage((char const*const)pinned);
	}

	void FormHelper::BindDelegates() {
		BIND_DELEGATE(Vibro);
		BIND_DELEGATE(Log);
		BIND_DELEGATE(SetCureName);
		BIND_DELEGATE(UpdateCurrentCure);
	}

} //namespace
