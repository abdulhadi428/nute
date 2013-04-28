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

	void FormHelper::Render()
	{
		for (int y=0; y<SCREENY; y++)
		for (int x=0; x<SCREENX; x++)
		{
			unsigned short val = LowLevel::VideoMemory[SCREENX*y+x];
			unsigned char r = (val & 0xF00) >> 8;
			unsigned char g = (val & 0xF0) >> 4;
			unsigned char b = val & 0xF;
			bmp->SetPixel(
				x,
				y,
				Color::FromArgb(0, r*16, g*16, b*16));
		}
	}

	void FormHelper::SetPlayerName(String^ name)
	{
		array<unsigned char>^ bytes = Encoding::ASCII->GetBytes(name);
		pin_ptr<unsigned char> pinned = &bytes[0];
		LowLevel::SetPlayerName((char const*const)pinned);
	}

	void FormHelper::SendMessage(String^ msg)
	{
		array<unsigned char>^ bytes = Encoding::ASCII->GetBytes(msg);
		pin_ptr<unsigned char> pinned = &bytes[0];
		LowLevel::SendMessage((char const*const)pinned);
	}

	void FormHelper::BindDelegates() {
		dwVibro = gcnew VoidRoutineDelegateWrapper();
		dwVibro->Delegate = gcnew VoidRoutineDelegate(mainForm,&MainForm::Vibro);
		dwDecreasePillCharges = gcnew VoidRoutineDelegateWrapper();
		dwDecreasePillCharges->Delegate = gcnew VoidRoutineDelegate(mainForm,&MainForm::DecreasePillCharges);
		dwLog = gcnew IntStrRoutineDelegateWrapper();
		dwLog->Delegate = gcnew IntStrRoutineDelegate(mainForm,&MainForm::Log);
		dwSetCureName = gcnew IntStrRoutineDelegateWrapper();
		dwSetCureName->Delegate = gcnew IntStrRoutineDelegate(mainForm,&MainForm::SetCureName);

		LowLevel::VOID_ROUTINE *pfn1,*pfn2;
		Marshal::StructureToPtr(dwVibro, (IntPtr)&pfn1, false);
		Marshal::StructureToPtr(dwDecreasePillCharges, (IntPtr)&pfn2, false);
		LowLevel::INT_STR_ROUTINE *pfn3,*pfn4;
		Marshal::StructureToPtr(dwLog, (IntPtr)&pfn3, false);
		Marshal::StructureToPtr(dwSetCureName, (IntPtr)&pfn4, false);
		LowLevel::RegisterCallbacks(pfn1,pfn2,pfn3,pfn4);
	 }

} //namespace
