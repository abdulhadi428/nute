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

	#define DECLARE_MANAGED_CALLBACK_TYPE(FnName) \
		[StructLayoutAttribute( LayoutKind::Sequential, CharSet = CharSet::Ansi )] \
		private ref struct FnName##ManagedCallback \
		{ \
			[MarshalAsAttribute(UnmanagedType::FunctionPtr)] \
			FnName##Delegate^ Delegate; \
		}

	#define DECLARE_MANAGED_CALLBACK_VAR(FnName) \
		static FnName##ManagedCallback^ pmfn##FnName

	#define BIND_DELEGATE(FnName) \
			pmfn##FnName = gcnew FnName##ManagedCallback(); \
			pmfn##FnName->Delegate = gcnew FnName##Delegate(mainForm,&MainForm::##FnName); \
			Marshal::StructureToPtr(pmfn##FnName, (IntPtr)&LowLevel::##FnName, false);

	private delegate void VibroDelegate();
	private delegate void LogDelegate(char* msg);
	private delegate void SetCureNameDelegate(int cure_id, char* name);
	private delegate void UpdateCurrentCureDelegate();

	DECLARE_MANAGED_CALLBACK_TYPE(Vibro);
	DECLARE_MANAGED_CALLBACK_TYPE(Log);
	DECLARE_MANAGED_CALLBACK_TYPE(SetCureName);
	DECLARE_MANAGED_CALLBACK_TYPE(UpdateCurrentCure);

	ref class MainForm;
	public ref class FormHelper
	{
	//callbacks from LowLevel unmanaged
	private:
		static void BindDelegates();
		DECLARE_MANAGED_CALLBACK_VAR(Vibro);
		DECLARE_MANAGED_CALLBACK_VAR(Log);
		DECLARE_MANAGED_CALLBACK_VAR(SetCureName);
		DECLARE_MANAGED_CALLBACK_VAR(UpdateCurrentCure);
	//calls to LowLevel unmanaged
	public:
		static void SetPlayerName(int olduid, int uid); //String^ name);
		static void SendMessage(String^ msg);
//		static void SendOsanve(int from, int osanve, int force);
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
		static void RenderScreen();
	};

} //namespace
