#pragma once
#include "stdafx.h"
#pragma unmanaged
#include "Common.h"
#include "CalloutInterface.h"
#include "..\KernelDll\KernelApi.h"
#pragma managed
#include "FormHelper.h"
#pragma warning(disable:4100)

namespace Visualizer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			 bKeysToButtons = true;
			 currCure=0;
			 currTorture=20;
			 currExplosionType=0;
			 last_mins = 0;
			 FormHelper::Init(this);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	//TIMERS
	private: System::Windows::Forms::Timer^  timerRender;
	//ARMLET
	private: System::Windows::Forms::GroupBox^  groupBoxMain;
		//BUTTONS
		private: System::Windows::Forms::Button^  buttonA;
		private: System::Windows::Forms::Button^  buttonB;
		private: System::Windows::Forms::Button^  buttonC;
		private: System::Windows::Forms::Button^  buttonX;
		private: System::Windows::Forms::Button^  buttonY;
		private: System::Windows::Forms::Button^  buttonZ;
		private: System::Windows::Forms::Button^  buttonL;
		private: System::Windows::Forms::Button^  buttonR;
		private: System::Windows::Forms::Button^  buttonE;
		//SCREEN
		private: System::Windows::Forms::PictureBox^  Screen;
	//SCREEN ZOOMED	
	private: System::Windows::Forms::PictureBox^  ScreenZoomed;
	//TABCONTROL
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::TabPage^  tabPage6;
	//TAB PAGE 1
	private: System::Windows::Forms::GroupBox^  groupBoxServer;
	private: System::Windows::Forms::TextBox^  textBoxPlayerName;
	private: System::Windows::Forms::Button^  buttonSetPlayerName;
	private: System::Windows::Forms::RadioButton^  radioButtonRegenerationLevel3;
	private: System::Windows::Forms::RadioButton^  radioButtonRegenerationLevel2;
	private: System::Windows::Forms::RadioButton^  radioButtonRegenerationLevel1;
	private: System::Windows::Forms::TextBox^  textBoxServerMessage;
	private: System::Windows::Forms::Button^  buttonServerMessage;
	//TAB PAGE 2
	private: System::Windows::Forms::GroupBox^  groupBoxLocks;
	private: System::Windows::Forms::CheckBox^  checkBoxLock1;
	private: System::Windows::Forms::CheckBox^  checkBoxLock2;
	private: System::Windows::Forms::CheckBox^  checkBoxLock3;
	private: System::Windows::Forms::RadioButton^  radioButtonLockNo;
	private: System::Windows::Forms::RadioButton^  radioButtonLock1;
	private: System::Windows::Forms::RadioButton^  radioButtonLock2;
	private: System::Windows::Forms::RadioButton^  radioButtonLock3;
	//TAB PAGE 3
	private: System::Windows::Forms::GroupBox^  groupBoxRooms;
	private: System::Windows::Forms::RadioButton^  radioButtonLustra10;
	private: System::Windows::Forms::RadioButton^  radioButtonLustra12;
	private: System::Windows::Forms::RadioButton^  radioButtonLustra11;
	private: System::Windows::Forms::RadioButton^  radioButtonLustra13;
	private: System::Windows::Forms::RadioButton^  radioButtonLustra14;
	private: System::Windows::Forms::RadioButton^  radioButtonLustra15;
	private: System::Windows::Forms::CheckBox^  checkBoxNoLustra;
	private: System::Windows::Forms::Button^  buttonRoom13Explosion;
	private: System::Windows::Forms::Button^  buttonRoom14Explosion;
	private: System::Windows::Forms::Button^  buttonRoom0Explosion;

	private: System::Windows::Forms::GroupBox^  groupBoxKernel;
	private: System::Windows::Forms::TextBox^  KernelParamBatteryLevel;
	private: System::Windows::Forms::Label^  KernelLabel5;
	private: System::Windows::Forms::TextBox^  KernelParamRadioLevel;
	private: System::Windows::Forms::Label^  KernelLabel4;
	private: System::Windows::Forms::TextBox^  KernelParamTime;
	private: System::Windows::Forms::Label^  KernelLabel3;
	private: System::Windows::Forms::TextBox^  KernelParamLustraId;
	private: System::Windows::Forms::Label^  KernelLabel2;
	private: System::Windows::Forms::TextBox^  KernelParamArmletId;
	private: System::Windows::Forms::Label^  KernelLabel1;

	private: System::Windows::Forms::GroupBox^  groupBoxExplosionType;
	private: System::Windows::Forms::RadioButton^  radioButtonExplosion2;
	private: System::Windows::Forms::RadioButton^  radioButtonExplosion1;
	private: System::Windows::Forms::RadioButton^  radioButtonExplosion0;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownExplosionProbability;
	private: System::Windows::Forms::RadioButton^  radioButtonExplosion3;
	private: System::Windows::Forms::Label^  ExplosionProbabilityLabel;
	//TAB PAGE 4
	private: System::Windows::Forms::GroupBox^  groupBoxPills;
	private: System::Windows::Forms::RadioButton^  radioButtonCure14;
	private: System::Windows::Forms::RadioButton^  radioButtonCure13;
	private: System::Windows::Forms::RadioButton^  radioButtonCure12;
	private: System::Windows::Forms::RadioButton^  radioButtonCure11;
	private: System::Windows::Forms::RadioButton^  radioButtonCure10;
	private: System::Windows::Forms::RadioButton^  radioButtonCure9;
	private: System::Windows::Forms::RadioButton^  radioButtonCure8;
	private: System::Windows::Forms::RadioButton^  radioButtonCure7;
	private: System::Windows::Forms::RadioButton^  radioButtonCure6;
	private: System::Windows::Forms::RadioButton^  radioButtonCure5;
	private: System::Windows::Forms::RadioButton^  radioButtonCure4;
	private: System::Windows::Forms::RadioButton^  radioButtonCure3;
	private: System::Windows::Forms::RadioButton^  radioButtonCure2;
	private: System::Windows::Forms::RadioButton^  radioButtonCure1;
	private: System::Windows::Forms::RadioButton^  radioButtonCure0;
	private: System::Windows::Forms::Label^  labelPillsCharges;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownPillsCharges;
	private: System::Windows::Forms::Button^  buttonNextMedTick;
	private: System::Windows::Forms::Button^  buttonConnectPill;
	//TAB PAGE 5
	private: System::Windows::Forms::GroupBox^  groupBoxPills2;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture26;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture25;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture24;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture23;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture22;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture21;
	private: System::Windows::Forms::RadioButton^  radioButtonTorture20;
	private: System::Windows::Forms::Label^  labelPills2Charges;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownPills2Charges;
	private: System::Windows::Forms::Button^  buttonConnectPill2;
	private: System::Windows::Forms::Button^  buttonNextMedTick2;
	private: System::Windows::Forms::TextBox^  ParamValue4;
	private: System::Windows::Forms::Label^  ParamLabel4;
	private: System::Windows::Forms::TextBox^  ParamValue3;
	private: System::Windows::Forms::Label^  ParamLabel3;
	private: System::Windows::Forms::TextBox^  ParamValue2;
	private: System::Windows::Forms::Label^  ParamLabel2;
	private: System::Windows::Forms::TextBox^  ParamValue1;
	private: System::Windows::Forms::Label^  ParamLabel1;	
	//TAB PAGE 6
	private: System::Windows::Forms::TextBox^  LogWindow;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		bool bKeysToButtons;
		int currCure;
		int currTorture;
		int currExplosionType;
		int last_mins;
		bool bVibrating;
		bool bVibratingReverseDirection;
		 int RemainignVibrationTicks;
		//LOG
		void ScrollLogToEnd() {
			LogWindow->SelectionStart = LogWindow->Text->Length;
			LogWindow->ScrollToCaret();
			LogWindow->Refresh();
		}
		void UpdateCurrentCureCharges(int charges) {
			if ((charges < 0)||(charges>99))
				return;
			numericUpDownPillsCharges->Value = charges;
		}
		void UpdateCurrentTortureCharges(int charges) {
			if ((charges < 0)||(charges>99))
				return;
			numericUpDownPills2Charges->Value = charges;
		}
	public:
		void Vibro() {
			bVibrating = true;
			RemainignVibrationTicks = 10; //0.5 seconds
		}
		void UpdateCurrentCure() {
			UpdateCurrentCureCharges(KernelApi::GetPillCharges(currCure));
		}
		void UpdateCurrentTorture() {
			UpdateCurrentTortureCharges(KernelApi::GetPillCharges(currTorture));
		}
		void Log(String^ message) {
			LogWindow->Text += (message + "\r\n");
			ScrollLogToEnd();
		}
		void Log(char* msg)
		{
			String^ message = gcnew String(msg);
			Log(message);
		}
		void SetPillName(int pill_id, char* _name)
		{
			String^ name = gcnew String(_name);
			SetPillName(pill_id, name);
		}
		void SetPillName(int pill_id, String^ name) {
			switch (pill_id) {
				case 0:
					radioButtonCure0->Text = name;
					break;
				case 1:
					radioButtonCure1->Text = name;
					break;
				case 2:
					radioButtonCure2->Text = name;
					break;
				case 3:
					radioButtonCure3->Text = name;
					break;
				case 4:
					radioButtonCure4->Text = name;
					break;
				case 5:
					radioButtonCure5->Text = name;
					break;
				case 6:
					radioButtonCure6->Text = name;
					break;
				case 7:
					radioButtonCure7->Text = name;
					break;
				case 8:
					radioButtonCure8->Text = name;
					break;
				case 9:
					radioButtonCure9->Text = name;
					break;
				case 10:
					radioButtonCure10->Text = name;
					break;
				case 11:
					radioButtonCure11->Text = name;
					break;
				case 12:
					radioButtonCure12->Text = name;
					break;
				case 13:
					radioButtonCure13->Text = name;
					break;
				case 14:
					radioButtonCure14->Text = name;
					break;
				case 20:
					radioButtonTorture20->Text = name;
					break;
				case 21:
					radioButtonTorture21->Text = name;
					break;
				case 22:
					radioButtonTorture22->Text = name;
					break;
				case 23:
					radioButtonTorture23->Text = name;
					break;
				case 24:
					radioButtonTorture24->Text = name;
					break;
				case 25:
					radioButtonTorture25->Text = name;
					break;
				case 26:
					radioButtonTorture26->Text = name;
					break;
			}
		}
		void UpdateKernel(int armlet_id, int lustra_id, int battery_level,
			int gate_id, int signal_level, int hours, int minutes, int uptime)
		{
			KernelParamArmletId->Text = armlet_id.ToString();	
			KernelParamLustraId->Text = lustra_id.ToString();
			KernelParamBatteryLevel->Text = battery_level.ToString();
			KernelParamRadioLevel->Text = String::Format("G{0}:Db{1}",gate_id,signal_level);	
			KernelParamTime->Text = String::Format("{0}:{1}/{2}",hours,minutes,uptime);
			if (minutes!=last_mins) {
				Callouts::NewTime(hours,minutes);
				last_mins = minutes;
			}
		}
		void UpdateAppState(int blood, int toxins, int pulse, int temperature)
		{
			ParamValue1->Text = blood.ToString();			//BloodCapacity
			ParamValue2->Text = toxins.ToString();			//ToxinsCapacity
			ParamValue3->Text = pulse.ToString();			//Pulse
			ParamValue4->Text = (30+temperature/10).ToString();	//Temperature
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timerRender = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBoxMain = (gcnew System::Windows::Forms::GroupBox());
			this->Screen = (gcnew System::Windows::Forms::PictureBox());
			this->buttonA = (gcnew System::Windows::Forms::Button());
			this->buttonB = (gcnew System::Windows::Forms::Button());
			this->buttonC = (gcnew System::Windows::Forms::Button());
			this->buttonX = (gcnew System::Windows::Forms::Button());
			this->buttonY = (gcnew System::Windows::Forms::Button());
			this->buttonZ = (gcnew System::Windows::Forms::Button());
			this->buttonL = (gcnew System::Windows::Forms::Button());
			this->buttonE = (gcnew System::Windows::Forms::Button());
			this->buttonR = (gcnew System::Windows::Forms::Button());
			this->ScreenZoomed = (gcnew System::Windows::Forms::PictureBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxServer = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonRegenerationLevel3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonRegenerationLevel2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonRegenerationLevel1 = (gcnew System::Windows::Forms::RadioButton());
			this->buttonSetPlayerName = (gcnew System::Windows::Forms::Button());
			this->textBoxPlayerName = (gcnew System::Windows::Forms::TextBox());
			this->textBoxServerMessage = (gcnew System::Windows::Forms::TextBox());
			this->buttonServerMessage = (gcnew System::Windows::Forms::Button());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxLocks = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonLockNo = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLock3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLock2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLock1 = (gcnew System::Windows::Forms::RadioButton());
			this->checkBoxLock3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxLock2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxLock1 = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxExplosionType = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonExplosion3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonExplosion2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonExplosion1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonExplosion0 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBoxKernel = (gcnew System::Windows::Forms::GroupBox());
			this->KernelParamLustraId = (gcnew System::Windows::Forms::TextBox());
			this->KernelLabel2 = (gcnew System::Windows::Forms::Label());
			this->KernelParamBatteryLevel = (gcnew System::Windows::Forms::TextBox());
			this->KernelLabel5 = (gcnew System::Windows::Forms::Label());
			this->KernelParamTime = (gcnew System::Windows::Forms::TextBox());
			this->KernelLabel3 = (gcnew System::Windows::Forms::Label());
			this->KernelParamRadioLevel = (gcnew System::Windows::Forms::TextBox());
			this->KernelLabel4 = (gcnew System::Windows::Forms::Label());
			this->KernelParamArmletId = (gcnew System::Windows::Forms::TextBox());
			this->KernelLabel1 = (gcnew System::Windows::Forms::Label());
			this->groupBoxRooms = (gcnew System::Windows::Forms::GroupBox());
			this->ExplosionProbabilityLabel = (gcnew System::Windows::Forms::Label());
			this->numericUpDownExplosionProbability = (gcnew System::Windows::Forms::NumericUpDown());
			this->radioButtonLustra15 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra12 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra14 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra13 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra11 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra10 = (gcnew System::Windows::Forms::RadioButton());
			this->buttonRoom14Explosion = (gcnew System::Windows::Forms::Button());
			this->buttonRoom13Explosion = (gcnew System::Windows::Forms::Button());
			this->buttonRoom0Explosion = (gcnew System::Windows::Forms::Button());
			this->checkBoxNoLustra = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxPills = (gcnew System::Windows::Forms::GroupBox());
			this->buttonNextMedTick = (gcnew System::Windows::Forms::Button());
			this->radioButtonCure14 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure13 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure12 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure11 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure10 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure9 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure8 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure7 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure6 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure5 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonCure0 = (gcnew System::Windows::Forms::RadioButton());
			this->labelPillsCharges = (gcnew System::Windows::Forms::Label());
			this->numericUpDownPillsCharges = (gcnew System::Windows::Forms::NumericUpDown());
			this->buttonConnectPill = (gcnew System::Windows::Forms::Button());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxPills2 = (gcnew System::Windows::Forms::GroupBox());
			this->ParamValue4 = (gcnew System::Windows::Forms::TextBox());
			this->ParamLabel4 = (gcnew System::Windows::Forms::Label());
			this->ParamValue3 = (gcnew System::Windows::Forms::TextBox());
			this->ParamLabel3 = (gcnew System::Windows::Forms::Label());
			this->ParamValue2 = (gcnew System::Windows::Forms::TextBox());
			this->ParamLabel2 = (gcnew System::Windows::Forms::Label());
			this->ParamValue1 = (gcnew System::Windows::Forms::TextBox());
			this->ParamLabel1 = (gcnew System::Windows::Forms::Label());
			this->buttonNextMedTick2 = (gcnew System::Windows::Forms::Button());
			this->radioButtonTorture26 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTorture25 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTorture24 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTorture23 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTorture22 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTorture21 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonTorture20 = (gcnew System::Windows::Forms::RadioButton());
			this->labelPills2Charges = (gcnew System::Windows::Forms::Label());
			this->numericUpDownPills2Charges = (gcnew System::Windows::Forms::NumericUpDown());
			this->buttonConnectPill2 = (gcnew System::Windows::Forms::Button());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->LogWindow = (gcnew System::Windows::Forms::TextBox());
			this->groupBoxMain->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Screen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenZoomed))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBoxServer->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBoxLocks->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->groupBoxExplosionType->SuspendLayout();
			this->groupBoxKernel->SuspendLayout();
			this->groupBoxRooms->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownExplosionProbability))->BeginInit();
			this->tabPage4->SuspendLayout();
			this->groupBoxPills->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownPillsCharges))->BeginInit();
			this->tabPage5->SuspendLayout();
			this->groupBoxPills2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownPills2Charges))->BeginInit();
			this->tabPage6->SuspendLayout();
			this->SuspendLayout();
			// 
			// timerRender
			// 
			this->timerRender->Enabled = true;
			this->timerRender->Interval = 50;
			this->timerRender->Tick += gcnew System::EventHandler(this, &MainForm::Render_Tick);
			// 
			// groupBoxMain
			// 
			this->groupBoxMain->BackColor = System::Drawing::Color::Black;
			this->groupBoxMain->Controls->Add(this->Screen);
			this->groupBoxMain->Controls->Add(this->buttonA);
			this->groupBoxMain->Controls->Add(this->buttonB);
			this->groupBoxMain->Controls->Add(this->buttonC);
			this->groupBoxMain->Controls->Add(this->buttonX);
			this->groupBoxMain->Controls->Add(this->buttonY);
			this->groupBoxMain->Controls->Add(this->buttonZ);
			this->groupBoxMain->Controls->Add(this->buttonL);
			this->groupBoxMain->Controls->Add(this->buttonE);
			this->groupBoxMain->Controls->Add(this->buttonR);
			this->groupBoxMain->ForeColor = System::Drawing::Color::White;
			this->groupBoxMain->Location = System::Drawing::Point(12, 13);
			this->groupBoxMain->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBoxMain->Name = L"groupBoxMain";
			this->groupBoxMain->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBoxMain->Size = System::Drawing::Size(418, 236);
			this->groupBoxMain->TabIndex = 21;
			this->groupBoxMain->TabStop = false;
			this->groupBoxMain->Text = L"Браслет";
			// 
			// Screen
			// 
			this->Screen->BackColor = System::Drawing::Color::Gray;
			this->Screen->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Screen->Location = System::Drawing::Point(114, 32);
			this->Screen->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->Screen->Name = L"Screen";
			this->Screen->Size = System::Drawing::Size(162, 130);
			this->Screen->TabIndex = 3;
			this->Screen->TabStop = false;
			// 
			// buttonA
			// 
			this->buttonA->BackColor = System::Drawing::Color::Orange;
			this->buttonA->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonA->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonA->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonA->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonA->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonA->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonA->Location = System::Drawing::Point(41, 37);
			this->buttonA->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonA->Name = L"buttonA";
			this->buttonA->Size = System::Drawing::Size(60, 26);
			this->buttonA->TabIndex = 4;
			this->buttonA->Text = L"A";
			this->buttonA->UseVisualStyleBackColor = true;
			this->buttonA->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonA_MouseDown);
			this->buttonA->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonA_MouseUp);
			// 
			// buttonB
			// 
			this->buttonB->BackColor = System::Drawing::Color::Orange;
			this->buttonB->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonB->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonB->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonB->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonB->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonB->Location = System::Drawing::Point(41, 72);
			this->buttonB->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonB->Name = L"buttonB";
			this->buttonB->Size = System::Drawing::Size(60, 26);
			this->buttonB->TabIndex = 5;
			this->buttonB->Text = L"B";
			this->buttonB->UseVisualStyleBackColor = true;
			this->buttonB->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonB_MouseDown);
			this->buttonB->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonB_MouseUp);
			// 
			// buttonC
			// 
			this->buttonC->BackColor = System::Drawing::Color::Orange;
			this->buttonC->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonC->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonC->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonC->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonC->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonC->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonC->Location = System::Drawing::Point(41, 104);
			this->buttonC->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonC->Name = L"buttonC";
			this->buttonC->Size = System::Drawing::Size(60, 26);
			this->buttonC->TabIndex = 6;
			this->buttonC->Text = L"C";
			this->buttonC->UseVisualStyleBackColor = true;
			this->buttonC->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonC_MouseDown);
			this->buttonC->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonC_MouseUp);
			// 
			// buttonX
			// 
			this->buttonX->BackColor = System::Drawing::Color::Orange;
			this->buttonX->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonX->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonX->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonX->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonX->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonX->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonX->Location = System::Drawing::Point(291, 64);
			this->buttonX->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonX->Name = L"buttonX";
			this->buttonX->Size = System::Drawing::Size(70, 26);
			this->buttonX->TabIndex = 7;
			this->buttonX->Text = L"X";
			this->buttonX->UseVisualStyleBackColor = true;
			this->buttonX->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonX_MouseDown);
			this->buttonX->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonX_MouseUp);
			// 
			// buttonY
			// 
			this->buttonY->BackColor = System::Drawing::Color::Orange;
			this->buttonY->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonY->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonY->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonY->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonY->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonY->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonY->Location = System::Drawing::Point(291, 101);
			this->buttonY->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonY->Name = L"buttonY";
			this->buttonY->Size = System::Drawing::Size(70, 26);
			this->buttonY->TabIndex = 8;
			this->buttonY->Text = L"Y";
			this->buttonY->UseVisualStyleBackColor = true;
			this->buttonY->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonY_MouseDown);
			this->buttonY->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonY_MouseUp);
			// 
			// buttonZ
			// 
			this->buttonZ->BackColor = System::Drawing::Color::Orange;
			this->buttonZ->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonZ->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonZ->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonZ->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonZ->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonZ->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonZ->Location = System::Drawing::Point(291, 135);
			this->buttonZ->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonZ->Name = L"buttonZ";
			this->buttonZ->Size = System::Drawing::Size(70, 26);
			this->buttonZ->TabIndex = 9;
			this->buttonZ->Text = L"Z";
			this->buttonZ->UseVisualStyleBackColor = true;
			this->buttonZ->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonZ_MouseDown);
			this->buttonZ->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonZ_MouseUp);
			// 
			// buttonL
			// 
			this->buttonL->BackColor = System::Drawing::Color::Orange;
			this->buttonL->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonL->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonL->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonL->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonL->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonL->Location = System::Drawing::Point(86, 179);
			this->buttonL->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonL->Name = L"buttonL";
			this->buttonL->Size = System::Drawing::Size(60, 26);
			this->buttonL->TabIndex = 10;
			this->buttonL->Text = L"L";
			this->buttonL->UseVisualStyleBackColor = true;
			this->buttonL->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonL_MouseDown);
			this->buttonL->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonL_MouseUp);
			// 
			// buttonE
			// 
			this->buttonE->BackColor = System::Drawing::Color::Orange;
			this->buttonE->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonE->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonE->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonE->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonE->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonE->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonE->Location = System::Drawing::Point(162, 179);
			this->buttonE->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonE->Name = L"buttonE";
			this->buttonE->Size = System::Drawing::Size(60, 26);
			this->buttonE->TabIndex = 12;
			this->buttonE->Text = L"E";
			this->buttonE->UseVisualStyleBackColor = true;
			this->buttonE->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonE_MouseDown);
			this->buttonE->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonE_MouseUp);
			// 
			// buttonR
			// 
			this->buttonR->BackColor = System::Drawing::Color::Orange;
			this->buttonR->FlatAppearance->BorderColor = System::Drawing::Color::SaddleBrown;
			this->buttonR->FlatAppearance->MouseDownBackColor = System::Drawing::Color::OrangeRed;
			this->buttonR->FlatAppearance->MouseOverBackColor = System::Drawing::Color::DarkOrange;
			this->buttonR->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonR->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9));
			this->buttonR->ForeColor = System::Drawing::SystemColors::WindowText;
			this->buttonR->Location = System::Drawing::Point(235, 179);
			this->buttonR->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonR->Name = L"buttonR";
			this->buttonR->Size = System::Drawing::Size(60, 26);
			this->buttonR->TabIndex = 11;
			this->buttonR->Text = L"R";
			this->buttonR->UseVisualStyleBackColor = true;
			this->buttonR->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonR_MouseDown);
			this->buttonR->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::buttonR_MouseUp);
			// 
			// ScreenZoomed
			// 
			this->ScreenZoomed->BackColor = System::Drawing::Color::Gray;
			this->ScreenZoomed->Location = System::Drawing::Point(450, 13);
			this->ScreenZoomed->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->ScreenZoomed->Name = L"ScreenZoomed";
			this->ScreenZoomed->Size = System::Drawing::Size(644, 516);
			this->ScreenZoomed->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->ScreenZoomed->TabIndex = 13;
			this->ScreenZoomed->TabStop = false;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Controls->Add(this->tabPage5);
			this->tabControl1->Controls->Add(this->tabPage6);
			this->tabControl1->Location = System::Drawing::Point(12, 256);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 4;
			this->tabControl1->Size = System::Drawing::Size(418, 269);
			this->tabControl1->TabIndex = 23;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBoxServer);
			this->tabPage1->Location = System::Drawing::Point(4, 24);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(410, 241);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Сервер";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// groupBoxServer
			// 
			this->groupBoxServer->Controls->Add(this->radioButtonRegenerationLevel3);
			this->groupBoxServer->Controls->Add(this->radioButtonRegenerationLevel2);
			this->groupBoxServer->Controls->Add(this->radioButtonRegenerationLevel1);
			this->groupBoxServer->Controls->Add(this->buttonSetPlayerName);
			this->groupBoxServer->Controls->Add(this->textBoxPlayerName);
			this->groupBoxServer->Controls->Add(this->textBoxServerMessage);
			this->groupBoxServer->Controls->Add(this->buttonServerMessage);
			this->groupBoxServer->Location = System::Drawing::Point(0, 0);
			this->groupBoxServer->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBoxServer->Name = L"groupBoxServer";
			this->groupBoxServer->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBoxServer->Size = System::Drawing::Size(410, 245);
			this->groupBoxServer->TabIndex = 22;
			this->groupBoxServer->TabStop = false;
			this->groupBoxServer->Text = L"Основное";
			// 
			// radioButtonRegenerationLevel3
			// 
			this->radioButtonRegenerationLevel3->AutoSize = true;
			this->radioButtonRegenerationLevel3->Location = System::Drawing::Point(15, 114);
			this->radioButtonRegenerationLevel3->Name = L"radioButtonRegenerationLevel3";
			this->radioButtonRegenerationLevel3->Size = System::Drawing::Size(91, 19);
			this->radioButtonRegenerationLevel3->TabIndex = 24;
			this->radioButtonRegenerationLevel3->TabStop = true;
			this->radioButtonRegenerationLevel3->Text = L"RegenLev 3";
			this->radioButtonRegenerationLevel3->UseVisualStyleBackColor = true;
			this->radioButtonRegenerationLevel3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonRegenerationLevel3_CheckedChanged);
			// 
			// radioButtonRegenerationLevel2
			// 
			this->radioButtonRegenerationLevel2->AutoSize = true;
			this->radioButtonRegenerationLevel2->Location = System::Drawing::Point(15, 88);
			this->radioButtonRegenerationLevel2->Name = L"radioButtonRegenerationLevel2";
			this->radioButtonRegenerationLevel2->Size = System::Drawing::Size(91, 19);
			this->radioButtonRegenerationLevel2->TabIndex = 23;
			this->radioButtonRegenerationLevel2->TabStop = true;
			this->radioButtonRegenerationLevel2->Text = L"RegenLev 2";
			this->radioButtonRegenerationLevel2->UseVisualStyleBackColor = true;
			this->radioButtonRegenerationLevel2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonRegenerationLevel2_CheckedChanged);
			// 
			// radioButtonRegenerationLevel1
			// 
			this->radioButtonRegenerationLevel1->AutoSize = true;
			this->radioButtonRegenerationLevel1->Location = System::Drawing::Point(15, 62);
			this->radioButtonRegenerationLevel1->Name = L"radioButtonRegenerationLevel1";
			this->radioButtonRegenerationLevel1->Size = System::Drawing::Size(91, 19);
			this->radioButtonRegenerationLevel1->TabIndex = 22;
			this->radioButtonRegenerationLevel1->TabStop = true;
			this->radioButtonRegenerationLevel1->Text = L"RegenLev 1";
			this->radioButtonRegenerationLevel1->UseVisualStyleBackColor = true;
			this->radioButtonRegenerationLevel1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonRegenerationLevel1_CheckedChanged);
			// 
			// buttonSetPlayerName
			// 
			this->buttonSetPlayerName->Location = System::Drawing::Point(287, 20);
			this->buttonSetPlayerName->Name = L"buttonSetPlayerName";
			this->buttonSetPlayerName->Size = System::Drawing::Size(107, 23);
			this->buttonSetPlayerName->TabIndex = 21;
			this->buttonSetPlayerName->Text = L"SetPlayerName";
			this->buttonSetPlayerName->UseVisualStyleBackColor = true;
			this->buttonSetPlayerName->Click += gcnew System::EventHandler(this, &MainForm::buttonSetPlayerName_Click);
			// 
			// textBoxPlayerName
			// 
			this->textBoxPlayerName->Location = System::Drawing::Point(15, 22);
			this->textBoxPlayerName->Name = L"textBoxPlayerName";
			this->textBoxPlayerName->Size = System::Drawing::Size(266, 21);
			this->textBoxPlayerName->TabIndex = 20;
			this->textBoxPlayerName->Leave += gcnew System::EventHandler(this, &MainForm::textBoxPlayerName_Leave);
			this->textBoxPlayerName->Enter += gcnew System::EventHandler(this, &MainForm::textBoxPlayerName_Enter);
			// 
			// textBoxServerMessage
			// 
			this->textBoxServerMessage->Location = System::Drawing::Point(107, 62);
			this->textBoxServerMessage->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->textBoxServerMessage->Multiline = true;
			this->textBoxServerMessage->Name = L"textBoxServerMessage";
			this->textBoxServerMessage->Size = System::Drawing::Size(287, 132);
			this->textBoxServerMessage->TabIndex = 19;
			this->textBoxServerMessage->Leave += gcnew System::EventHandler(this, &MainForm::textBoxServerMessage_Leave);
			this->textBoxServerMessage->Enter += gcnew System::EventHandler(this, &MainForm::textBoxServerMessage_Enter);
			// 
			// buttonServerMessage
			// 
			this->buttonServerMessage->Location = System::Drawing::Point(229, 202);
			this->buttonServerMessage->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->buttonServerMessage->Name = L"buttonServerMessage";
			this->buttonServerMessage->Size = System::Drawing::Size(165, 26);
			this->buttonServerMessage->TabIndex = 17;
			this->buttonServerMessage->Text = L"SendMessage";
			this->buttonServerMessage->UseVisualStyleBackColor = true;
			this->buttonServerMessage->Click += gcnew System::EventHandler(this, &MainForm::buttonServerMessage_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBoxLocks);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(410, 243);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Замки";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBoxLocks
			// 
			this->groupBoxLocks->Controls->Add(this->radioButtonLockNo);
			this->groupBoxLocks->Controls->Add(this->radioButtonLock3);
			this->groupBoxLocks->Controls->Add(this->radioButtonLock2);
			this->groupBoxLocks->Controls->Add(this->radioButtonLock1);
			this->groupBoxLocks->Controls->Add(this->checkBoxLock3);
			this->groupBoxLocks->Controls->Add(this->checkBoxLock2);
			this->groupBoxLocks->Controls->Add(this->checkBoxLock1);
			this->groupBoxLocks->Location = System::Drawing::Point(0, 0);
			this->groupBoxLocks->Name = L"groupBoxLocks";
			this->groupBoxLocks->Size = System::Drawing::Size(410, 245);
			this->groupBoxLocks->TabIndex = 0;
			this->groupBoxLocks->TabStop = false;
			this->groupBoxLocks->Text = L"Могу открыть / Виден браслету";
			// 
			// radioButtonLockNo
			// 
			this->radioButtonLockNo->AutoSize = true;
			this->radioButtonLockNo->Location = System::Drawing::Point(110, 106);
			this->radioButtonLockNo->Name = L"radioButtonLockNo";
			this->radioButtonLockNo->Size = System::Drawing::Size(169, 19);
			this->radioButtonLockNo->TabIndex = 17;
			this->radioButtonLockNo->TabStop = true;
			this->radioButtonLockNo->Text = L"Замок не детектируется";
			this->radioButtonLockNo->UseVisualStyleBackColor = true;
			this->radioButtonLockNo->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLockNo_CheckedChanged);
			// 
			// radioButtonLock3
			// 
			this->radioButtonLock3->AutoSize = true;
			this->radioButtonLock3->Location = System::Drawing::Point(110, 80);
			this->radioButtonLock3->Name = L"radioButtonLock3";
			this->radioButtonLock3->Size = System::Drawing::Size(72, 19);
			this->radioButtonLock3->TabIndex = 16;
			this->radioButtonLock3->TabStop = true;
			this->radioButtonLock3->Text = L"Замок 3";
			this->radioButtonLock3->UseVisualStyleBackColor = true;
			this->radioButtonLock3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLock3_CheckedChanged);
			// 
			// radioButtonLock2
			// 
			this->radioButtonLock2->AutoSize = true;
			this->radioButtonLock2->Location = System::Drawing::Point(110, 54);
			this->radioButtonLock2->Name = L"radioButtonLock2";
			this->radioButtonLock2->Size = System::Drawing::Size(72, 19);
			this->radioButtonLock2->TabIndex = 15;
			this->radioButtonLock2->TabStop = true;
			this->radioButtonLock2->Text = L"Замок 2";
			this->radioButtonLock2->UseVisualStyleBackColor = true;
			this->radioButtonLock2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLock2_CheckedChanged);
			// 
			// radioButtonLock1
			// 
			this->radioButtonLock1->AutoSize = true;
			this->radioButtonLock1->Location = System::Drawing::Point(110, 28);
			this->radioButtonLock1->Name = L"radioButtonLock1";
			this->radioButtonLock1->Size = System::Drawing::Size(72, 19);
			this->radioButtonLock1->TabIndex = 14;
			this->radioButtonLock1->TabStop = true;
			this->radioButtonLock1->Text = L"Замок 1";
			this->radioButtonLock1->UseVisualStyleBackColor = true;
			this->radioButtonLock1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLock1_CheckedChanged);
			// 
			// checkBoxLock3
			// 
			this->checkBoxLock3->AutoSize = true;
			this->checkBoxLock3->Location = System::Drawing::Point(15, 77);
			this->checkBoxLock3->Name = L"checkBoxLock3";
			this->checkBoxLock3->Size = System::Drawing::Size(77, 19);
			this->checkBoxLock3->TabIndex = 7;
			this->checkBoxLock3->Text = L"Доступ 3";
			this->checkBoxLock3->UseVisualStyleBackColor = true;
			this->checkBoxLock3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::checkBoxLock3_CheckedChanged);
			// 
			// checkBoxLock2
			// 
			this->checkBoxLock2->AutoSize = true;
			this->checkBoxLock2->Location = System::Drawing::Point(15, 53);
			this->checkBoxLock2->Name = L"checkBoxLock2";
			this->checkBoxLock2->Size = System::Drawing::Size(77, 19);
			this->checkBoxLock2->TabIndex = 5;
			this->checkBoxLock2->Text = L"Доступ 2";
			this->checkBoxLock2->UseVisualStyleBackColor = true;
			this->checkBoxLock2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::checkBoxLock2_CheckedChanged);
			// 
			// checkBoxLock1
			// 
			this->checkBoxLock1->AutoSize = true;
			this->checkBoxLock1->Location = System::Drawing::Point(15, 29);
			this->checkBoxLock1->Name = L"checkBoxLock1";
			this->checkBoxLock1->Size = System::Drawing::Size(77, 19);
			this->checkBoxLock1->TabIndex = 3;
			this->checkBoxLock1->Text = L"Доступ 1";
			this->checkBoxLock1->UseVisualStyleBackColor = true;
			this->checkBoxLock1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::checkBoxLock1_CheckedChanged);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->groupBoxExplosionType);
			this->tabPage3->Controls->Add(this->groupBoxKernel);
			this->tabPage3->Controls->Add(this->groupBoxRooms);
			this->tabPage3->Location = System::Drawing::Point(4, 24);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(410, 241);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Отсеки";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// groupBoxExplosionType
			// 
			this->groupBoxExplosionType->Controls->Add(this->radioButtonExplosion3);
			this->groupBoxExplosionType->Controls->Add(this->radioButtonExplosion2);
			this->groupBoxExplosionType->Controls->Add(this->radioButtonExplosion1);
			this->groupBoxExplosionType->Controls->Add(this->radioButtonExplosion0);
			this->groupBoxExplosionType->Location = System::Drawing::Point(246, 126);
			this->groupBoxExplosionType->Name = L"groupBoxExplosionType";
			this->groupBoxExplosionType->Size = System::Drawing::Size(161, 112);
			this->groupBoxExplosionType->TabIndex = 2;
			this->groupBoxExplosionType->TabStop = false;
			this->groupBoxExplosionType->Text = L"Тип взрыва";
			// 
			// radioButtonExplosion3
			// 
			this->radioButtonExplosion3->AutoSize = true;
			this->radioButtonExplosion3->Location = System::Drawing::Point(6, 87);
			this->radioButtonExplosion3->Name = L"radioButtonExplosion3";
			this->radioButtonExplosion3->Size = System::Drawing::Size(82, 19);
			this->radioButtonExplosion3->TabIndex = 4;
			this->radioButtonExplosion3->Text = L"Радиация";
			this->radioButtonExplosion3->UseVisualStyleBackColor = true;
			this->radioButtonExplosion3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonExplosion3_CheckedChanged);
			// 
			// radioButtonExplosion2
			// 
			this->radioButtonExplosion2->AutoSize = true;
			this->radioButtonExplosion2->Location = System::Drawing::Point(6, 66);
			this->radioButtonExplosion2->Name = L"radioButtonExplosion2";
			this->radioButtonExplosion2->Size = System::Drawing::Size(99, 19);
			this->radioButtonExplosion2->TabIndex = 3;
			this->radioButtonExplosion2->Text = L"Термальный";
			this->radioButtonExplosion2->UseVisualStyleBackColor = true;
			this->radioButtonExplosion2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonExplosion2_CheckedChanged);
			// 
			// radioButtonExplosion1
			// 
			this->radioButtonExplosion1->AutoSize = true;
			this->radioButtonExplosion1->Location = System::Drawing::Point(6, 43);
			this->radioButtonExplosion1->Name = L"radioButtonExplosion1";
			this->radioButtonExplosion1->Size = System::Drawing::Size(113, 19);
			this->radioButtonExplosion1->TabIndex = 2;
			this->radioButtonExplosion1->Text = L"Ударная волна";
			this->radioButtonExplosion1->UseVisualStyleBackColor = true;
			this->radioButtonExplosion1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonExplosion1_CheckedChanged);
			// 
			// radioButtonExplosion0
			// 
			this->radioButtonExplosion0->AutoSize = true;
			this->radioButtonExplosion0->Checked = true;
			this->radioButtonExplosion0->Location = System::Drawing::Point(6, 21);
			this->radioButtonExplosion0->Name = L"radioButtonExplosion0";
			this->radioButtonExplosion0->Size = System::Drawing::Size(88, 19);
			this->radioButtonExplosion0->TabIndex = 1;
			this->radioButtonExplosion0->TabStop = true;
			this->radioButtonExplosion0->Text = L"Случайный";
			this->radioButtonExplosion0->UseVisualStyleBackColor = true;
			this->radioButtonExplosion0->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonExplosion0_CheckedChanged);
			// 
			// groupBoxKernel
			// 
			this->groupBoxKernel->Controls->Add(this->KernelParamLustraId);
			this->groupBoxKernel->Controls->Add(this->KernelLabel2);
			this->groupBoxKernel->Controls->Add(this->KernelParamBatteryLevel);
			this->groupBoxKernel->Controls->Add(this->KernelLabel5);
			this->groupBoxKernel->Controls->Add(this->KernelParamTime);
			this->groupBoxKernel->Controls->Add(this->KernelLabel3);
			this->groupBoxKernel->Controls->Add(this->KernelParamRadioLevel);
			this->groupBoxKernel->Controls->Add(this->KernelLabel4);
			this->groupBoxKernel->Controls->Add(this->KernelParamArmletId);
			this->groupBoxKernel->Controls->Add(this->KernelLabel1);
			this->groupBoxKernel->Location = System::Drawing::Point(0, 126);
			this->groupBoxKernel->Name = L"groupBoxKernel";
			this->groupBoxKernel->Size = System::Drawing::Size(239, 119);
			this->groupBoxKernel->TabIndex = 1;
			this->groupBoxKernel->TabStop = false;
			this->groupBoxKernel->Text = L"Параметры ядра браслета";
			// 
			// KernelParamLustraId
			// 
			this->KernelParamLustraId->Location = System::Drawing::Point(76, 41);
			this->KernelParamLustraId->Name = L"KernelParamLustraId";
			this->KernelParamLustraId->ReadOnly = true;
			this->KernelParamLustraId->Size = System::Drawing::Size(54, 21);
			this->KernelParamLustraId->TabIndex = 39;
			// 
			// KernelLabel2
			// 
			this->KernelLabel2->AutoSize = true;
			this->KernelLabel2->Location = System::Drawing::Point(73, 23);
			this->KernelLabel2->Name = L"KernelLabel2";
			this->KernelLabel2->Size = System::Drawing::Size(56, 15);
			this->KernelLabel2->TabIndex = 38;
			this->KernelLabel2->Text = L"Lustra ID";
			// 
			// KernelParamBatteryLevel
			// 
			this->KernelParamBatteryLevel->AcceptsReturn = true;
			this->KernelParamBatteryLevel->Location = System::Drawing::Point(118, 88);
			this->KernelParamBatteryLevel->Name = L"KernelParamBatteryLevel";
			this->KernelParamBatteryLevel->ReadOnly = true;
			this->KernelParamBatteryLevel->Size = System::Drawing::Size(100, 21);
			this->KernelParamBatteryLevel->TabIndex = 37;
			// 
			// KernelLabel5
			// 
			this->KernelLabel5->AutoSize = true;
			this->KernelLabel5->Location = System::Drawing::Point(115, 70);
			this->KernelLabel5->Name = L"KernelLabel5";
			this->KernelLabel5->Size = System::Drawing::Size(120, 15);
			this->KernelLabel5->TabIndex = 36;
			this->KernelLabel5->Text = L"УровеньБатарейки";
			// 
			// KernelParamTime
			// 
			this->KernelParamTime->Location = System::Drawing::Point(139, 41);
			this->KernelParamTime->Name = L"KernelParamTime";
			this->KernelParamTime->ReadOnly = true;
			this->KernelParamTime->Size = System::Drawing::Size(79, 21);
			this->KernelParamTime->TabIndex = 35;
			// 
			// KernelLabel3
			// 
			this->KernelLabel3->AutoSize = true;
			this->KernelLabel3->Location = System::Drawing::Point(136, 23);
			this->KernelLabel3->Name = L"KernelLabel3";
			this->KernelLabel3->Size = System::Drawing::Size(45, 15);
			this->KernelLabel3->TabIndex = 34;
			this->KernelLabel3->Text = L"Время";
			// 
			// KernelParamRadioLevel
			// 
			this->KernelParamRadioLevel->Location = System::Drawing::Point(9, 88);
			this->KernelParamRadioLevel->Name = L"KernelParamRadioLevel";
			this->KernelParamRadioLevel->ReadOnly = true;
			this->KernelParamRadioLevel->Size = System::Drawing::Size(100, 21);
			this->KernelParamRadioLevel->TabIndex = 33;
			// 
			// KernelLabel4
			// 
			this->KernelLabel4->AutoSize = true;
			this->KernelLabel4->Location = System::Drawing::Point(6, 70);
			this->KernelLabel4->Name = L"KernelLabel4";
			this->KernelLabel4->Size = System::Drawing::Size(106, 15);
			this->KernelLabel4->TabIndex = 32;
			this->KernelLabel4->Text = L"Уровень сигнала";
			// 
			// KernelParamArmletId
			// 
			this->KernelParamArmletId->Location = System::Drawing::Point(9, 41);
			this->KernelParamArmletId->Name = L"KernelParamArmletId";
			this->KernelParamArmletId->ReadOnly = true;
			this->KernelParamArmletId->Size = System::Drawing::Size(54, 21);
			this->KernelParamArmletId->TabIndex = 31;
			// 
			// KernelLabel1
			// 
			this->KernelLabel1->AutoSize = true;
			this->KernelLabel1->Location = System::Drawing::Point(6, 23);
			this->KernelLabel1->Name = L"KernelLabel1";
			this->KernelLabel1->Size = System::Drawing::Size(57, 15);
			this->KernelLabel1->TabIndex = 30;
			this->KernelLabel1->Text = L"Armlet ID";
			// 
			// groupBoxRooms
			// 
			this->groupBoxRooms->Controls->Add(this->ExplosionProbabilityLabel);
			this->groupBoxRooms->Controls->Add(this->numericUpDownExplosionProbability);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra15);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra12);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra14);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra13);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra11);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra10);
			this->groupBoxRooms->Controls->Add(this->buttonRoom14Explosion);
			this->groupBoxRooms->Controls->Add(this->buttonRoom13Explosion);
			this->groupBoxRooms->Controls->Add(this->buttonRoom0Explosion);
			this->groupBoxRooms->Controls->Add(this->checkBoxNoLustra);
			this->groupBoxRooms->Location = System::Drawing::Point(0, 0);
			this->groupBoxRooms->Name = L"groupBoxRooms";
			this->groupBoxRooms->Size = System::Drawing::Size(410, 124);
			this->groupBoxRooms->TabIndex = 0;
			this->groupBoxRooms->TabStop = false;
			this->groupBoxRooms->Text = L"Люстра / Отсек";
			// 
			// ExplosionProbabilityLabel
			// 
			this->ExplosionProbabilityLabel->AutoSize = true;
			this->ExplosionProbabilityLabel->Location = System::Drawing::Point(249, 101);
			this->ExplosionProbabilityLabel->Name = L"ExplosionProbabilityLabel";
			this->ExplosionProbabilityLabel->Size = System::Drawing::Size(87, 15);
			this->ExplosionProbabilityLabel->TabIndex = 19;
			this->ExplosionProbabilityLabel->Text = L"Вероятность:";
			// 
			// numericUpDownExplosionProbability
			// 
			this->numericUpDownExplosionProbability->Location = System::Drawing::Point(345, 99);
			this->numericUpDownExplosionProbability->Name = L"numericUpDownExplosionProbability";
			this->numericUpDownExplosionProbability->Size = System::Drawing::Size(55, 21);
			this->numericUpDownExplosionProbability->TabIndex = 0;
			this->numericUpDownExplosionProbability->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			this->numericUpDownExplosionProbability->Leave += gcnew System::EventHandler(this, &MainForm::numericUpDownExplosionProbability_Leave);
			this->numericUpDownExplosionProbability->Enter += gcnew System::EventHandler(this, &MainForm::numericUpDownExplosionProbability_Enter);
			// 
			// radioButtonLustra15
			// 
			this->radioButtonLustra15->AutoSize = true;
			this->radioButtonLustra15->Location = System::Drawing::Point(139, 75);
			this->radioButtonLustra15->Name = L"radioButtonLustra15";
			this->radioButtonLustra15->Size = System::Drawing::Size(113, 19);
			this->radioButtonLustra15->TabIndex = 18;
			this->radioButtonLustra15->TabStop = true;
			this->radioButtonLustra15->Text = L"15 / Вне отсека";
			this->radioButtonLustra15->UseVisualStyleBackColor = true;
			this->radioButtonLustra15->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLustra15_CheckedChanged);
			// 
			// radioButtonLustra12
			// 
			this->radioButtonLustra12->AutoSize = true;
			this->radioButtonLustra12->Location = System::Drawing::Point(15, 74);
			this->radioButtonLustra12->Name = L"radioButtonLustra12";
			this->radioButtonLustra12->Size = System::Drawing::Size(113, 19);
			this->radioButtonLustra12->TabIndex = 17;
			this->radioButtonLustra12->TabStop = true;
			this->radioButtonLustra12->Text = L"12 / Вне отсека";
			this->radioButtonLustra12->UseVisualStyleBackColor = true;
			this->radioButtonLustra12->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLustra12_CheckedChanged);
			// 
			// radioButtonLustra14
			// 
			this->radioButtonLustra14->AutoSize = true;
			this->radioButtonLustra14->Location = System::Drawing::Point(139, 48);
			this->radioButtonLustra14->Name = L"radioButtonLustra14";
			this->radioButtonLustra14->Size = System::Drawing::Size(100, 19);
			this->radioButtonLustra14->TabIndex = 16;
			this->radioButtonLustra14->TabStop = true;
			this->radioButtonLustra14->Text = L"14 / Отсек 14";
			this->radioButtonLustra14->UseVisualStyleBackColor = true;
			this->radioButtonLustra14->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLustra14_CheckedChanged);
			// 
			// radioButtonLustra13
			// 
			this->radioButtonLustra13->AutoSize = true;
			this->radioButtonLustra13->Location = System::Drawing::Point(139, 22);
			this->radioButtonLustra13->Name = L"radioButtonLustra13";
			this->radioButtonLustra13->Size = System::Drawing::Size(100, 19);
			this->radioButtonLustra13->TabIndex = 15;
			this->radioButtonLustra13->TabStop = true;
			this->radioButtonLustra13->Text = L"13 / Отсек 13";
			this->radioButtonLustra13->UseVisualStyleBackColor = true;
			this->radioButtonLustra13->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLustra13_CheckedChanged);
			// 
			// radioButtonLustra11
			// 
			this->radioButtonLustra11->AutoSize = true;
			this->radioButtonLustra11->Location = System::Drawing::Point(15, 48);
			this->radioButtonLustra11->Name = L"radioButtonLustra11";
			this->radioButtonLustra11->Size = System::Drawing::Size(100, 19);
			this->radioButtonLustra11->TabIndex = 14;
			this->radioButtonLustra11->TabStop = true;
			this->radioButtonLustra11->Text = L"11 / Отсек 11";
			this->radioButtonLustra11->UseVisualStyleBackColor = true;
			this->radioButtonLustra11->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLustra11_CheckedChanged);
			// 
			// radioButtonLustra10
			// 
			this->radioButtonLustra10->AutoSize = true;
			this->radioButtonLustra10->Location = System::Drawing::Point(15, 22);
			this->radioButtonLustra10->Name = L"radioButtonLustra10";
			this->radioButtonLustra10->Size = System::Drawing::Size(100, 19);
			this->radioButtonLustra10->TabIndex = 13;
			this->radioButtonLustra10->TabStop = true;
			this->radioButtonLustra10->Text = L"10 / Отсек 10";
			this->radioButtonLustra10->UseVisualStyleBackColor = true;
			this->radioButtonLustra10->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonLustra10_CheckedChanged);
			// 
			// buttonRoom14Explosion
			// 
			this->buttonRoom14Explosion->Location = System::Drawing::Point(259, 48);
			this->buttonRoom14Explosion->Name = L"buttonRoom14Explosion";
			this->buttonRoom14Explosion->Size = System::Drawing::Size(142, 23);
			this->buttonRoom14Explosion->TabIndex = 12;
			this->buttonRoom14Explosion->Text = L"Взрыв в отсеке 14";
			this->buttonRoom14Explosion->UseVisualStyleBackColor = true;
			this->buttonRoom14Explosion->Click += gcnew System::EventHandler(this, &MainForm::buttonRoom14Explosion_Click);
			// 
			// buttonRoom13Explosion
			// 
			this->buttonRoom13Explosion->Location = System::Drawing::Point(259, 18);
			this->buttonRoom13Explosion->Name = L"buttonRoom13Explosion";
			this->buttonRoom13Explosion->Size = System::Drawing::Size(142, 23);
			this->buttonRoom13Explosion->TabIndex = 11;
			this->buttonRoom13Explosion->Text = L" Взрыв в отсеке 13";
			this->buttonRoom13Explosion->UseVisualStyleBackColor = true;
			this->buttonRoom13Explosion->Click += gcnew System::EventHandler(this, &MainForm::buttonRoom13Explosion_Click);
			// 
			// buttonRoom0Explosion
			// 
			this->buttonRoom0Explosion->Location = System::Drawing::Point(258, 75);
			this->buttonRoom0Explosion->Name = L"buttonRoom0Explosion";
			this->buttonRoom0Explosion->Size = System::Drawing::Size(142, 23);
			this->buttonRoom0Explosion->TabIndex = 11;
			this->buttonRoom0Explosion->Text = L" Взрыв в отсеке 15";
			this->buttonRoom0Explosion->UseVisualStyleBackColor = true;
			this->buttonRoom0Explosion->Click += gcnew System::EventHandler(this, &MainForm::buttonRoom0Explosion_Click);
			// 
			// checkBoxNoLustra
			// 
			this->checkBoxNoLustra->AutoSize = true;
			this->checkBoxNoLustra->Location = System::Drawing::Point(15, 100);
			this->checkBoxNoLustra->Name = L"checkBoxNoLustra";
			this->checkBoxNoLustra->Size = System::Drawing::Size(177, 19);
			this->checkBoxNoLustra->TabIndex = 10;
			this->checkBoxNoLustra->Text = L"Люстра не детектируется";
			this->checkBoxNoLustra->UseVisualStyleBackColor = true;
			this->checkBoxNoLustra->CheckedChanged += gcnew System::EventHandler(this, &MainForm::checkBoxNoLustra_CheckedChanged);
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->groupBoxPills);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(410, 243);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"Лекарства";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// groupBoxPills
			// 
			this->groupBoxPills->Controls->Add(this->buttonNextMedTick);
			this->groupBoxPills->Controls->Add(this->radioButtonCure14);
			this->groupBoxPills->Controls->Add(this->radioButtonCure13);
			this->groupBoxPills->Controls->Add(this->radioButtonCure12);
			this->groupBoxPills->Controls->Add(this->radioButtonCure11);
			this->groupBoxPills->Controls->Add(this->radioButtonCure10);
			this->groupBoxPills->Controls->Add(this->radioButtonCure9);
			this->groupBoxPills->Controls->Add(this->radioButtonCure8);
			this->groupBoxPills->Controls->Add(this->radioButtonCure7);
			this->groupBoxPills->Controls->Add(this->radioButtonCure6);
			this->groupBoxPills->Controls->Add(this->radioButtonCure5);
			this->groupBoxPills->Controls->Add(this->radioButtonCure4);
			this->groupBoxPills->Controls->Add(this->radioButtonCure3);
			this->groupBoxPills->Controls->Add(this->radioButtonCure2);
			this->groupBoxPills->Controls->Add(this->radioButtonCure1);
			this->groupBoxPills->Controls->Add(this->radioButtonCure0);
			this->groupBoxPills->Controls->Add(this->labelPillsCharges);
			this->groupBoxPills->Controls->Add(this->numericUpDownPillsCharges);
			this->groupBoxPills->Controls->Add(this->buttonConnectPill);
			this->groupBoxPills->Location = System::Drawing::Point(0, 0);
			this->groupBoxPills->Name = L"groupBoxPills";
			this->groupBoxPills->Size = System::Drawing::Size(410, 235);
			this->groupBoxPills->TabIndex = 1;
			this->groupBoxPills->TabStop = false;
			this->groupBoxPills->Text = L"Лекарство / Заряды";
			// 
			// buttonNextMedTick
			// 
			this->buttonNextMedTick->Location = System::Drawing::Point(15, 203);
			this->buttonNextMedTick->Name = L"buttonNextMedTick";
			this->buttonNextMedTick->Size = System::Drawing::Size(146, 23);
			this->buttonNextMedTick->TabIndex = 26;
			this->buttonNextMedTick->Text = L"Next Med Tick";
			this->buttonNextMedTick->UseVisualStyleBackColor = true;
			this->buttonNextMedTick->Click += gcnew System::EventHandler(this, &MainForm::buttonNextMedTick_Click);
			// 
			// radioButtonCure14
			// 
			this->radioButtonCure14->AutoSize = true;
			this->radioButtonCure14->Location = System::Drawing::Point(192, 155);
			this->radioButtonCure14->Name = L"radioButtonCure14";
			this->radioButtonCure14->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure14->TabIndex = 29;
			this->radioButtonCure14->TabStop = true;
			this->radioButtonCure14->Text = L"Cure 14";
			this->radioButtonCure14->UseVisualStyleBackColor = true;
			this->radioButtonCure14->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure14_CheckedChanged);
			// 
			// radioButtonCure13
			// 
			this->radioButtonCure13->AutoSize = true;
			this->radioButtonCure13->Location = System::Drawing::Point(192, 131);
			this->radioButtonCure13->Name = L"radioButtonCure13";
			this->radioButtonCure13->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure13->TabIndex = 28;
			this->radioButtonCure13->TabStop = true;
			this->radioButtonCure13->Text = L"Cure 13";
			this->radioButtonCure13->UseVisualStyleBackColor = true;
			this->radioButtonCure13->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure13_CheckedChanged);
			// 
			// radioButtonCure12
			// 
			this->radioButtonCure12->AutoSize = true;
			this->radioButtonCure12->Location = System::Drawing::Point(192, 108);
			this->radioButtonCure12->Name = L"radioButtonCure12";
			this->radioButtonCure12->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure12->TabIndex = 27;
			this->radioButtonCure12->TabStop = true;
			this->radioButtonCure12->Text = L"Cure 12";
			this->radioButtonCure12->UseVisualStyleBackColor = true;
			this->radioButtonCure12->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure12_CheckedChanged);
			// 
			// radioButtonCure11
			// 
			this->radioButtonCure11->AutoSize = true;
			this->radioButtonCure11->Location = System::Drawing::Point(192, 85);
			this->radioButtonCure11->Name = L"radioButtonCure11";
			this->radioButtonCure11->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure11->TabIndex = 25;
			this->radioButtonCure11->TabStop = true;
			this->radioButtonCure11->Text = L"Cure 11";
			this->radioButtonCure11->UseVisualStyleBackColor = true;
			this->radioButtonCure11->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure11_CheckedChanged);
			// 
			// radioButtonCure10
			// 
			this->radioButtonCure10->AutoSize = true;
			this->radioButtonCure10->Location = System::Drawing::Point(192, 62);
			this->radioButtonCure10->Name = L"radioButtonCure10";
			this->radioButtonCure10->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure10->TabIndex = 24;
			this->radioButtonCure10->TabStop = true;
			this->radioButtonCure10->Text = L"Cure 10";
			this->radioButtonCure10->UseVisualStyleBackColor = true;
			this->radioButtonCure10->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure10_CheckedChanged);
			// 
			// radioButtonCure9
			// 
			this->radioButtonCure9->AutoSize = true;
			this->radioButtonCure9->Location = System::Drawing::Point(192, 39);
			this->radioButtonCure9->Name = L"radioButtonCure9";
			this->radioButtonCure9->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure9->TabIndex = 23;
			this->radioButtonCure9->TabStop = true;
			this->radioButtonCure9->Text = L"Cure 9";
			this->radioButtonCure9->UseVisualStyleBackColor = true;
			this->radioButtonCure9->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure9_CheckedChanged);
			// 
			// radioButtonCure8
			// 
			this->radioButtonCure8->AutoSize = true;
			this->radioButtonCure8->Location = System::Drawing::Point(192, 17);
			this->radioButtonCure8->Name = L"radioButtonCure8";
			this->radioButtonCure8->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure8->TabIndex = 22;
			this->radioButtonCure8->TabStop = true;
			this->radioButtonCure8->Text = L"Cure 8";
			this->radioButtonCure8->UseVisualStyleBackColor = true;
			this->radioButtonCure8->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure8_CheckedChanged);
			// 
			// radioButtonCure7
			// 
			this->radioButtonCure7->AutoSize = true;
			this->radioButtonCure7->Location = System::Drawing::Point(15, 178);
			this->radioButtonCure7->Name = L"radioButtonCure7";
			this->radioButtonCure7->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure7->TabIndex = 21;
			this->radioButtonCure7->TabStop = true;
			this->radioButtonCure7->Text = L"Cure 7";
			this->radioButtonCure7->UseVisualStyleBackColor = true;
			this->radioButtonCure7->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure7_CheckedChanged);
			// 
			// radioButtonCure6
			// 
			this->radioButtonCure6->AutoSize = true;
			this->radioButtonCure6->Location = System::Drawing::Point(15, 155);
			this->radioButtonCure6->Name = L"radioButtonCure6";
			this->radioButtonCure6->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure6->TabIndex = 20;
			this->radioButtonCure6->TabStop = true;
			this->radioButtonCure6->Text = L"Cure 6";
			this->radioButtonCure6->UseVisualStyleBackColor = true;
			this->radioButtonCure6->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure6_CheckedChanged);
			// 
			// radioButtonCure5
			// 
			this->radioButtonCure5->AutoSize = true;
			this->radioButtonCure5->Location = System::Drawing::Point(15, 131);
			this->radioButtonCure5->Name = L"radioButtonCure5";
			this->radioButtonCure5->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure5->TabIndex = 19;
			this->radioButtonCure5->TabStop = true;
			this->radioButtonCure5->Text = L"Cure 5";
			this->radioButtonCure5->UseVisualStyleBackColor = true;
			this->radioButtonCure5->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure5_CheckedChanged);
			// 
			// radioButtonCure4
			// 
			this->radioButtonCure4->AutoSize = true;
			this->radioButtonCure4->Location = System::Drawing::Point(15, 108);
			this->radioButtonCure4->Name = L"radioButtonCure4";
			this->radioButtonCure4->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure4->TabIndex = 18;
			this->radioButtonCure4->TabStop = true;
			this->radioButtonCure4->Text = L"Cure 4";
			this->radioButtonCure4->UseVisualStyleBackColor = true;
			this->radioButtonCure4->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure4_CheckedChanged);
			// 
			// radioButtonCure3
			// 
			this->radioButtonCure3->AutoSize = true;
			this->radioButtonCure3->Location = System::Drawing::Point(15, 85);
			this->radioButtonCure3->Name = L"radioButtonCure3";
			this->radioButtonCure3->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure3->TabIndex = 17;
			this->radioButtonCure3->TabStop = true;
			this->radioButtonCure3->Text = L"Cure 3";
			this->radioButtonCure3->UseVisualStyleBackColor = true;
			this->radioButtonCure3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure3_CheckedChanged);
			// 
			// radioButtonCure2
			// 
			this->radioButtonCure2->AutoSize = true;
			this->radioButtonCure2->Location = System::Drawing::Point(15, 62);
			this->radioButtonCure2->Name = L"radioButtonCure2";
			this->radioButtonCure2->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure2->TabIndex = 16;
			this->radioButtonCure2->TabStop = true;
			this->radioButtonCure2->Text = L"Cure 2";
			this->radioButtonCure2->UseVisualStyleBackColor = true;
			this->radioButtonCure2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure2_CheckedChanged);
			// 
			// radioButtonCure1
			// 
			this->radioButtonCure1->AutoSize = true;
			this->radioButtonCure1->Location = System::Drawing::Point(15, 39);
			this->radioButtonCure1->Name = L"radioButtonCure1";
			this->radioButtonCure1->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure1->TabIndex = 15;
			this->radioButtonCure1->TabStop = true;
			this->radioButtonCure1->Text = L"Cure 1";
			this->radioButtonCure1->UseVisualStyleBackColor = true;
			this->radioButtonCure1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure1_CheckedChanged);
			// 
			// radioButtonCure0
			// 
			this->radioButtonCure0->AutoSize = true;
			this->radioButtonCure0->Location = System::Drawing::Point(15, 17);
			this->radioButtonCure0->Name = L"radioButtonCure0";
			this->radioButtonCure0->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure0->TabIndex = 14;
			this->radioButtonCure0->TabStop = true;
			this->radioButtonCure0->Text = L"Cure 0";
			this->radioButtonCure0->UseVisualStyleBackColor = true;
			this->radioButtonCure0->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure0_CheckedChanged);
			// 
			// labelPillsCharges
			// 
			this->labelPillsCharges->AutoSize = true;
			this->labelPillsCharges->Location = System::Drawing::Point(271, 182);
			this->labelPillsCharges->Name = L"labelPillsCharges";
			this->labelPillsCharges->Size = System::Drawing::Size(131, 15);
			this->labelPillsCharges->TabIndex = 13;
			this->labelPillsCharges->Text = L"Число зарядов {0..99}";
			// 
			// numericUpDownPillsCharges
			// 
			this->numericUpDownPillsCharges->Location = System::Drawing::Point(348, 205);
			this->numericUpDownPillsCharges->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			this->numericUpDownPillsCharges->Name = L"numericUpDownPillsCharges";
			this->numericUpDownPillsCharges->Size = System::Drawing::Size(54, 21);
			this->numericUpDownPillsCharges->TabIndex = 12;
			this->numericUpDownPillsCharges->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			this->numericUpDownPillsCharges->ValueChanged += gcnew System::EventHandler(this, &MainForm::numericUpDownPillsCharges_ValueChanged);
			this->numericUpDownPillsCharges->Leave += gcnew System::EventHandler(this, &MainForm::numericUpDownPillsCharges_Leave);
			this->numericUpDownPillsCharges->Enter += gcnew System::EventHandler(this, &MainForm::numericUpDownPillsCharges_Enter);
			// 
			// buttonConnectPill
			// 
			this->buttonConnectPill->Location = System::Drawing::Point(196, 203);
			this->buttonConnectPill->Name = L"buttonConnectPill";
			this->buttonConnectPill->Size = System::Drawing::Size(146, 23);
			this->buttonConnectPill->TabIndex = 11;
			this->buttonConnectPill->Text = L"Connect Pill";
			this->buttonConnectPill->UseVisualStyleBackColor = true;
			this->buttonConnectPill->Click += gcnew System::EventHandler(this, &MainForm::buttonConnectPill_Click);
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->groupBoxPills2);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(410, 243);
			this->tabPage5->TabIndex = 5;
			this->tabPage5->Text = L"Пытки и Параметры";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// groupBoxPills2
			// 
			this->groupBoxPills2->Controls->Add(this->ParamValue4);
			this->groupBoxPills2->Controls->Add(this->ParamLabel4);
			this->groupBoxPills2->Controls->Add(this->ParamValue3);
			this->groupBoxPills2->Controls->Add(this->ParamLabel3);
			this->groupBoxPills2->Controls->Add(this->ParamValue2);
			this->groupBoxPills2->Controls->Add(this->ParamLabel2);
			this->groupBoxPills2->Controls->Add(this->ParamValue1);
			this->groupBoxPills2->Controls->Add(this->ParamLabel1);
			this->groupBoxPills2->Controls->Add(this->buttonNextMedTick2);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture26);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture25);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture24);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture23);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture22);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture21);
			this->groupBoxPills2->Controls->Add(this->radioButtonTorture20);
			this->groupBoxPills2->Controls->Add(this->labelPills2Charges);
			this->groupBoxPills2->Controls->Add(this->numericUpDownPills2Charges);
			this->groupBoxPills2->Controls->Add(this->buttonConnectPill2);
			this->groupBoxPills2->Location = System::Drawing::Point(7, 7);
			this->groupBoxPills2->Name = L"groupBoxPills2";
			this->groupBoxPills2->Size = System::Drawing::Size(397, 228);
			this->groupBoxPills2->TabIndex = 0;
			this->groupBoxPills2->TabStop = false;
			this->groupBoxPills2->Text = L"Пилюли пыток / Заряды";
			// 
			// ParamValue4
			// 
			this->ParamValue4->Location = System::Drawing::Point(263, 150);
			this->ParamValue4->Name = L"ParamValue4";
			this->ParamValue4->ReadOnly = true;
			this->ParamValue4->Size = System::Drawing::Size(100, 21);
			this->ParamValue4->TabIndex = 35;
			// 
			// ParamLabel4
			// 
			this->ParamLabel4->AutoSize = true;
			this->ParamLabel4->Location = System::Drawing::Point(260, 132);
			this->ParamLabel4->Name = L"ParamLabel4";
			this->ParamLabel4->Size = System::Drawing::Size(84, 15);
			this->ParamLabel4->TabIndex = 34;
			this->ParamLabel4->Text = L"Температура";
			// 
			// ParamValue3
			// 
			this->ParamValue3->Location = System::Drawing::Point(263, 110);
			this->ParamValue3->Name = L"ParamValue3";
			this->ParamValue3->ReadOnly = true;
			this->ParamValue3->Size = System::Drawing::Size(100, 21);
			this->ParamValue3->TabIndex = 33;
			// 
			// ParamLabel3
			// 
			this->ParamLabel3->AutoSize = true;
			this->ParamLabel3->Location = System::Drawing::Point(260, 92);
			this->ParamLabel3->Name = L"ParamLabel3";
			this->ParamLabel3->Size = System::Drawing::Size(41, 15);
			this->ParamLabel3->TabIndex = 32;
			this->ParamLabel3->Text = L"Пульс";
			// 
			// ParamValue2
			// 
			this->ParamValue2->Location = System::Drawing::Point(263, 69);
			this->ParamValue2->Name = L"ParamValue2";
			this->ParamValue2->ReadOnly = true;
			this->ParamValue2->Size = System::Drawing::Size(100, 21);
			this->ParamValue2->TabIndex = 31;
			// 
			// ParamLabel2
			// 
			this->ParamLabel2->AutoSize = true;
			this->ParamLabel2->Location = System::Drawing::Point(260, 51);
			this->ParamLabel2->Name = L"ParamLabel2";
			this->ParamLabel2->Size = System::Drawing::Size(104, 15);
			this->ParamLabel2->TabIndex = 30;
			this->ParamLabel2->Text = L"Кол-во токсинов";
			// 
			// ParamValue1
			// 
			this->ParamValue1->Location = System::Drawing::Point(263, 28);
			this->ParamValue1->Name = L"ParamValue1";
			this->ParamValue1->ReadOnly = true;
			this->ParamValue1->Size = System::Drawing::Size(100, 21);
			this->ParamValue1->TabIndex = 29;
			// 
			// ParamLabel1
			// 
			this->ParamLabel1->AutoSize = true;
			this->ParamLabel1->Location = System::Drawing::Point(260, 10);
			this->ParamLabel1->Name = L"ParamLabel1";
			this->ParamLabel1->Size = System::Drawing::Size(84, 15);
			this->ParamLabel1->TabIndex = 28;
			this->ParamLabel1->Text = L"Кол-во крови";
			// 
			// buttonNextMedTick2
			// 
			this->buttonNextMedTick2->Location = System::Drawing::Point(19, 195);
			this->buttonNextMedTick2->Name = L"buttonNextMedTick2";
			this->buttonNextMedTick2->Size = System::Drawing::Size(146, 23);
			this->buttonNextMedTick2->TabIndex = 27;
			this->buttonNextMedTick2->Text = L"Next Med Tick";
			this->buttonNextMedTick2->UseVisualStyleBackColor = true;
			this->buttonNextMedTick2->Click += gcnew System::EventHandler(this, &MainForm::buttonNextMedTick2_Click);
			// 
			// radioButtonTorture26
			// 
			this->radioButtonTorture26->AutoSize = true;
			this->radioButtonTorture26->Location = System::Drawing::Point(6, 170);
			this->radioButtonTorture26->Name = L"radioButtonTorture26";
			this->radioButtonTorture26->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture26->TabIndex = 23;
			this->radioButtonTorture26->TabStop = true;
			this->radioButtonTorture26->Text = L"Пытка 26";
			this->radioButtonTorture26->UseVisualStyleBackColor = true;
			this->radioButtonTorture26->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture26_CheckedChanged);
			// 
			// radioButtonTorture25
			// 
			this->radioButtonTorture25->AutoSize = true;
			this->radioButtonTorture25->Location = System::Drawing::Point(7, 146);
			this->radioButtonTorture25->Name = L"radioButtonTorture25";
			this->radioButtonTorture25->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture25->TabIndex = 22;
			this->radioButtonTorture25->TabStop = true;
			this->radioButtonTorture25->Text = L"Пытка 25";
			this->radioButtonTorture25->UseVisualStyleBackColor = true;
			this->radioButtonTorture25->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture25_CheckedChanged);
			// 
			// radioButtonTorture24
			// 
			this->radioButtonTorture24->AutoSize = true;
			this->radioButtonTorture24->Location = System::Drawing::Point(7, 121);
			this->radioButtonTorture24->Name = L"radioButtonTorture24";
			this->radioButtonTorture24->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture24->TabIndex = 21;
			this->radioButtonTorture24->TabStop = true;
			this->radioButtonTorture24->Text = L"Пытка 24";
			this->radioButtonTorture24->UseVisualStyleBackColor = true;
			this->radioButtonTorture24->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture24_CheckedChanged);
			// 
			// radioButtonTorture23
			// 
			this->radioButtonTorture23->AutoSize = true;
			this->radioButtonTorture23->Location = System::Drawing::Point(7, 96);
			this->radioButtonTorture23->Name = L"radioButtonTorture23";
			this->radioButtonTorture23->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture23->TabIndex = 20;
			this->radioButtonTorture23->TabStop = true;
			this->radioButtonTorture23->Text = L"Пытка 23";
			this->radioButtonTorture23->UseVisualStyleBackColor = true;
			this->radioButtonTorture23->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture23_CheckedChanged);
			// 
			// radioButtonTorture22
			// 
			this->radioButtonTorture22->AutoSize = true;
			this->radioButtonTorture22->Location = System::Drawing::Point(7, 71);
			this->radioButtonTorture22->Name = L"radioButtonTorture22";
			this->radioButtonTorture22->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture22->TabIndex = 19;
			this->radioButtonTorture22->TabStop = true;
			this->radioButtonTorture22->Text = L"Пытка 22";
			this->radioButtonTorture22->UseVisualStyleBackColor = true;
			this->radioButtonTorture22->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture22_CheckedChanged);
			// 
			// radioButtonTorture21
			// 
			this->radioButtonTorture21->AutoSize = true;
			this->radioButtonTorture21->Location = System::Drawing::Point(7, 46);
			this->radioButtonTorture21->Name = L"radioButtonTorture21";
			this->radioButtonTorture21->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture21->TabIndex = 18;
			this->radioButtonTorture21->TabStop = true;
			this->radioButtonTorture21->Text = L"Пытка 21";
			this->radioButtonTorture21->UseVisualStyleBackColor = true;
			this->radioButtonTorture21->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture21_CheckedChanged);
			// 
			// radioButtonTorture20
			// 
			this->radioButtonTorture20->AutoSize = true;
			this->radioButtonTorture20->Location = System::Drawing::Point(7, 21);
			this->radioButtonTorture20->Name = L"radioButtonTorture20";
			this->radioButtonTorture20->Size = System::Drawing::Size(80, 19);
			this->radioButtonTorture20->TabIndex = 17;
			this->radioButtonTorture20->TabStop = true;
			this->radioButtonTorture20->Text = L"Пытка 20";
			this->radioButtonTorture20->UseVisualStyleBackColor = true;
			this->radioButtonTorture20->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonTorture20_CheckedChanged);
			// 
			// labelPills2Charges
			// 
			this->labelPills2Charges->AutoSize = true;
			this->labelPills2Charges->Location = System::Drawing::Point(260, 174);
			this->labelPills2Charges->Name = L"labelPills2Charges";
			this->labelPills2Charges->Size = System::Drawing::Size(131, 15);
			this->labelPills2Charges->TabIndex = 16;
			this->labelPills2Charges->Text = L"Число зарядов {0..99}";
			// 
			// numericUpDownPills2Charges
			// 
			this->numericUpDownPills2Charges->Location = System::Drawing::Point(337, 197);
			this->numericUpDownPills2Charges->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			this->numericUpDownPills2Charges->Name = L"numericUpDownPills2Charges";
			this->numericUpDownPills2Charges->Size = System::Drawing::Size(54, 21);
			this->numericUpDownPills2Charges->TabIndex = 15;
			this->numericUpDownPills2Charges->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			this->numericUpDownPills2Charges->ValueChanged += gcnew System::EventHandler(this, &MainForm::numericUpDownPills2Charges_ValueChanged);
			this->numericUpDownPills2Charges->Leave += gcnew System::EventHandler(this, &MainForm::numericUpDownPills2Charges_Leave);
			this->numericUpDownPills2Charges->Enter += gcnew System::EventHandler(this, &MainForm::numericUpDownPills2Charges_Enter);
			// 
			// buttonConnectPill2
			// 
			this->buttonConnectPill2->Location = System::Drawing::Point(185, 195);
			this->buttonConnectPill2->Name = L"buttonConnectPill2";
			this->buttonConnectPill2->Size = System::Drawing::Size(146, 23);
			this->buttonConnectPill2->TabIndex = 14;
			this->buttonConnectPill2->Text = L"Connect Pill";
			this->buttonConnectPill2->UseVisualStyleBackColor = true;
			this->buttonConnectPill2->Click += gcnew System::EventHandler(this, &MainForm::buttonConnectPill2_Click);
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->LogWindow);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Padding = System::Windows::Forms::Padding(3);
			this->tabPage6->Size = System::Drawing::Size(410, 243);
			this->tabPage6->TabIndex = 4;
			this->tabPage6->Text = L"Log";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// LogWindow
			// 
			this->LogWindow->Location = System::Drawing::Point(5, 1);
			this->LogWindow->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->LogWindow->Multiline = true;
			this->LogWindow->Name = L"LogWindow";
			this->LogWindow->ReadOnly = true;
			this->LogWindow->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->LogWindow->Size = System::Drawing::Size(401, 238);
			this->LogWindow->TabIndex = 17;
			this->LogWindow->VisibleChanged += gcnew System::EventHandler(this, &MainForm::LogWindow_VisibleChanged);
			// 
			// MainForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->AutoScroll = true;
			this->BackColor = System::Drawing::SystemColors::AppWorkspace;
			this->ClientSize = System::Drawing::Size(1094, 574);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->groupBoxMain);
			this->Controls->Add(this->ScreenZoomed);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->ForeColor = System::Drawing::SystemColors::WindowText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->KeyPreview = true;
			this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"Armlet Visualizer";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->Shown += gcnew System::EventHandler(this, &MainForm::MainForm_Shown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyUp);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyDown);
			this->groupBoxMain->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Screen))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenZoomed))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->groupBoxServer->ResumeLayout(false);
			this->groupBoxServer->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->groupBoxLocks->ResumeLayout(false);
			this->groupBoxLocks->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->groupBoxExplosionType->ResumeLayout(false);
			this->groupBoxExplosionType->PerformLayout();
			this->groupBoxKernel->ResumeLayout(false);
			this->groupBoxKernel->PerformLayout();
			this->groupBoxRooms->ResumeLayout(false);
			this->groupBoxRooms->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownExplosionProbability))->EndInit();
			this->tabPage4->ResumeLayout(false);
			this->groupBoxPills->ResumeLayout(false);
			this->groupBoxPills->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownPillsCharges))->EndInit();
			this->tabPage5->ResumeLayout(false);
			this->groupBoxPills2->ResumeLayout(false);
			this->groupBoxPills2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownPills2Charges))->EndInit();
			this->tabPage6->ResumeLayout(false);
			this->tabPage6->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

//INITIALIZATION
private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void MainForm_Shown(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::ArmletMain();
		 }

//TIMER - main output event
private: System::Void Render_Tick(System::Object^  sender, System::EventArgs^  e) {
			 FormHelper::RenderScreen();
			 Screen->Image = FormHelper::bmp;
			 ScreenZoomed->Image = FormHelper::bmp;

			 if (bVibrating) {
				int step = 25;
				bVibratingReverseDirection = !bVibratingReverseDirection;
				if (bVibratingReverseDirection) {
					step = -step;
				}
				Left += step;
				RemainignVibrationTicks--;
				if (RemainignVibrationTicks==0)
					bVibrating = false;
			 }
		 }

//LOG fix
private: System::Void LogWindow_VisibleChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (LogWindow->Visible)
				 ScrollLogToEnd();
		 }

//check ACTIVE CONTROL to disable BUTTONS accelerator keys
private: System::Void textBoxPlayerName_Enter(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = false;
		 }
private: System::Void textBoxPlayerName_Leave(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = true;
		 }
private: System::Void textBoxServerMessage_Enter(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = false;
		 }
private: System::Void textBoxServerMessage_Leave(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = true;
		 }
private: System::Void numericUpDownPillsCharges_Enter(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = false;
		 }
private: System::Void numericUpDownPillsCharges_Leave(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = true;
		 }
private: System::Void numericUpDownPills2Charges_Enter(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = false;
		 }
private: System::Void numericUpDownPills2Charges_Leave(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = true;
		 }	 
private: System::Void numericUpDownExplosionProbability_Enter(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = false;
		 }
private: System::Void numericUpDownExplosionProbability_Leave(System::Object^  sender, System::EventArgs^  e) {
			 bKeysToButtons = true;
		 }

//BUTTONS and BUTTONS accelerator keys (Press/Release)
private: System::Void MainForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if (bKeysToButtons) 
			 {
				int button = FormHelper::KeyToButton(e->KeyCode);
				if (button != NO_BUTTON)
					 Callouts::OnButtonPress(button);
			 }
		 }
private: System::Void MainForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if (bKeysToButtons) 
			 {
				int button = FormHelper::KeyToButton(e->KeyCode);
				if (button != NO_BUTTON)
					 Callouts::OnButtonRelease(button);
			}
		 }
private: System::Void buttonA_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_A);
		 }
private: System::Void buttonA_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_A);
		 }
private: System::Void buttonB_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_B);
		 }
private: System::Void buttonB_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_B);
		 }
private: System::Void buttonC_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_C);
		 }
private: System::Void buttonC_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_C);
		 }
private: System::Void buttonX_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_X);
		 }
private: System::Void buttonX_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_X);
		 }
private: System::Void buttonY_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_Y);
		 }
private: System::Void buttonY_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_Y);
		 }
private: System::Void buttonZ_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_Z);
		 }
private: System::Void buttonZ_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_Z);
		 }
private: System::Void buttonL_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_L);
		 }
private: System::Void buttonL_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_L);
		 }
private: System::Void buttonE_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_E);
		 }
private: System::Void buttonE_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_E);
		 }
private: System::Void buttonR_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonRelease(BUTTON_R);
		 }
private: System::Void buttonR_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 Callouts::OnButtonPress(BUTTON_R);
		 }

//REGENERATION LEVEL
private: System::Void radioButtonRegenerationLevel1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonRegenerationLevel1->Checked;
			if (b)
				Callouts::SetRegenerationLevel(1);
		 }
private: System::Void radioButtonRegenerationLevel2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonRegenerationLevel2->Checked;
			if (b)
				Callouts::SetRegenerationLevel(2);
		 }
private: System::Void radioButtonRegenerationLevel3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonRegenerationLevel3->Checked;
 			if (b)
				Callouts::SetRegenerationLevel(3);
		 }

//PLAYER NAME
private: System::Void buttonSetPlayerName_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormHelper::SetPlayerName(textBoxPlayerName->Text);
		 }

//SEND MESSAGE
private: System::Void buttonServerMessage_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormHelper::SendMessage(textBoxServerMessage->Text);
		 }

//LOCKS
private: System::Void checkBoxLock1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			Callouts::SetLockAccess(1,checkBoxLock1->Checked);
		 }
private: System::Void checkBoxLock2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			Callouts::SetLockAccess(2,checkBoxLock2->Checked);
		 }
private: System::Void checkBoxLock3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			Callouts::SetLockAccess(3,checkBoxLock3->Checked);
		 }
private: System::Void radioButtonLock1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLock1->Checked;
 			if (b)
				KernelApi::SetCurrentLock(1);
		 }
private: System::Void radioButtonLock2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLock2->Checked;
 			if (b)
				KernelApi::SetCurrentLock(2);
		 }
private: System::Void radioButtonLock3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLock3->Checked;
 			if (b)
				KernelApi::SetCurrentLock(3);
		 }
private: System::Void radioButtonLockNo_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLockNo->Checked;
 			if (b)
				KernelApi::SetCurrentLock(0);
		 }

//ROOMS
private: System::Void radioButtonLustra10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra10->Checked;
			 if (b)
				KernelApi::SetCurrentLustra(10);
			 KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }
private: System::Void radioButtonLustra11_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra11->Checked;
			 if (b)
				KernelApi::SetCurrentLustra(11);
			 KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }
private: System::Void radioButtonLustra12_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra12->Checked;
			 if (b)
				KernelApi::SetCurrentLustra(12); //NoRoom
			 KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }
private: System::Void radioButtonLustra13_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra13->Checked;
			 if (b)
				KernelApi::SetCurrentLustra(13);
			 KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }
private: System::Void radioButtonLustra14_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra14->Checked;
			 if (b)
				KernelApi::SetCurrentLustra(14);
			 KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }
private: System::Void radioButtonLustra15_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra15->Checked;
			 if (b)
				KernelApi::SetCurrentLustra(15); //NoRoom
			 KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }
private: System::Void checkBoxNoLustra_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				KernelApi::LustraInRange(!checkBoxNoLustra->Checked);
		 }

//EXPLOSIONS
private: System::Void buttonRoom13Explosion_Click(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::Explosion(13,(int)numericUpDownExplosionProbability->Value,currExplosionType);
		 }
private: System::Void buttonRoom14Explosion_Click(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::Explosion(14,(int)numericUpDownExplosionProbability->Value,currExplosionType);
		 }
private: System::Void buttonRoom0Explosion_Click(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::Explosion(15,(int)numericUpDownExplosionProbability->Value,currExplosionType);
		 }
private: System::Void radioButtonExplosion0_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonExplosion0->Checked)
				 currExplosionType = 0;
		 }
private: System::Void radioButtonExplosion1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonExplosion1->Checked)
				 currExplosionType = 1;
		 }
private: System::Void radioButtonExplosion2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonExplosion2->Checked)
				 currExplosionType = 2;
		 }
private: System::Void radioButtonExplosion3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonExplosion3->Checked)
				 currExplosionType = 3;
		 }
	 
//PILLS(CURES & TORTURES)
private: System::Void radioButtonCure0_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure0->Checked)
				 currCure = 0;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure1->Checked)
				 currCure = 1;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure2->Checked)
				 currCure = 2;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure3->Checked)
				 currCure = 3;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure4->Checked)
				 currCure = 4;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure5->Checked)
				 currCure = 5;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure6->Checked)
				 currCure = 6;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure7_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure7->Checked)
				 currCure = 7;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure8_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure8->Checked)
				 currCure = 8;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure9_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure9->Checked)
				 currCure = 9;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure10->Checked)
				 currCure = 10;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure11_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure11->Checked)
				 currCure = 11;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure12_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure12->Checked)
				 currCure = 12;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure13_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure13->Checked)
				 currCure = 13;
			 UpdateCurrentCure();
		 }
private: System::Void radioButtonCure14_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure14->Checked)
				 currCure = 14;
			 UpdateCurrentCure();
		 }
private: System::Void buttonConnectPill_Click(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::OnPillConnect(
				 currCure, 
				 (int)numericUpDownPillsCharges->Value);
		 }
private: System::Void buttonNextMedTick_Click(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::NextMedTick();
		 }
private: System::Void radioButtonTorture20_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture20->Checked)
				currTorture = 20;
			UpdateCurrentTorture();
		 }
private: System::Void radioButtonTorture21_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture21->Checked)
				currTorture = 21;
			UpdateCurrentTorture();
		 }
private: System::Void radioButtonTorture22_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture22->Checked)
				currTorture = 22;
			UpdateCurrentTorture();
		 }
private: System::Void radioButtonTorture23_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture23->Checked)
				currTorture = 23;
			UpdateCurrentTorture();
		 }
private: System::Void radioButtonTorture24_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture24->Checked)
				currTorture = 24;
			UpdateCurrentTorture();
		 }
private: System::Void radioButtonTorture25_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture25->Checked)
				currTorture = 25;
			UpdateCurrentTorture();
		 }
private: System::Void radioButtonTorture26_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			if (radioButtonTorture26->Checked)
				currTorture = 26;
			UpdateCurrentTorture();
		 }
private: System::Void buttonConnectPill2_Click(System::Object^  sender, System::EventArgs^  e) {
			 Callouts::OnPillConnect(
				 currTorture, 
				 (int)numericUpDownPills2Charges->Value);
		 }
private: System::Void buttonNextMedTick2_Click(System::Object^  sender, System::EventArgs^  e) {
			Callouts::NextMedTick();
		 }

//MISC
private: System::Void numericUpDownPillsCharges_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 KernelApi::SetPillCharges(currCure, (int)numericUpDownPillsCharges->Value);
		 }
private: System::Void numericUpDownPills2Charges_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 KernelApi::SetPillCharges(currTorture, (int)numericUpDownPills2Charges->Value);
		 }
}; //class

} //namespace
