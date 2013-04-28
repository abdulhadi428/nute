#pragma once

namespace Visualizer {

	using namespace System::Runtime::InteropServices;
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::ComponentModel;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace System::Windows::Forms;
	using namespace System::Drawing::Imaging;

	private delegate void VoidRoutineDelegate();
	[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Ansi )]
	private ref struct VoidRoutineDelegateWrapper
	{
		[MarshalAsAttribute(UnmanagedType::FunctionPtr)]
		VoidRoutineDelegate^ Delegate;
	};
	private delegate void IntStrRoutineDelegate(int n, char* str);
	[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Ansi )]
	private ref struct IntStrRoutineDelegateWrapper
	{
		[MarshalAsAttribute(UnmanagedType::FunctionPtr)]
		IntStrRoutineDelegate^ Delegate;
	};

	ref class MainForm;
	public ref class FormHelper
	{
	//callbacks from LowLevel lib
	private:
		static VoidRoutineDelegateWrapper^ dwVibro;
		static VoidRoutineDelegateWrapper^ dwDecreasePillCharges;
		static IntStrRoutineDelegateWrapper^ dwLog;
		static IntStrRoutineDelegateWrapper^ dwSetCureName;
		static void BindDelegates();
	//calls to LowLevel lib
	public:
		static void SetPlayerName(String^ name);
		static void SendMessage(String^ msg);
	//other
	public:
		static MainForm^ mainForm;
		static Bitmap^ bmp;

		static void Init(MainForm^ _mainForm) {
			mainForm = _mainForm;
			bmp = gcnew Bitmap(SCREENX, SCREENY, PixelFormat::Format24bppRgb);
			BindDelegates();
		}

		static int KeyToButton(System::Windows::Forms::Keys key);
		static void Render();
	};

} //namespace
