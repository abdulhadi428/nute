#pragma once
#include "stdafx.h"
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
	//TAB PAGE 4
	private: System::Windows::Forms::GroupBox^  groupBoxPills;
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
	private: System::Windows::Forms::Label^  labelPillsCharges;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownPillsCharges;
	private: System::Windows::Forms::Button^  buttonNextMedTick;
	private: System::Windows::Forms::Button^  buttonConnectPill;
	//TAB PAGE 5
	private: System::Windows::Forms::TextBox^  LogWindow;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		bool bKeysToButtons;
		int currCure;
		bool bVibrating;
		bool bVibratingReverseDirection;
		int bVibrationTicks;
		//LOG
		void ScrollLogToEnd() {
			LogWindow->SelectionStart = LogWindow->Text->Length;
			LogWindow->ScrollToCaret();
			LogWindow->Refresh();
		}
	public:
		void Vibro() {
			bVibrating = true;
			bVibrationTicks = 10; //0.5 seconds
		}
		void DecreasePillCharges() {
			if (numericUpDownPillsCharges->Value !=0)
				numericUpDownPillsCharges->Value--;
		}
		void Log(String^ message) {
			LogWindow->Text += (message + "\r\n");
			ScrollLogToEnd();
		}
		void Log(int nothing, char* msg)
		{
			String^ message = gcnew String(msg);
			Log(message);
		}
		void SetCureName(int cure_id, char* _name)
		{
			String^ name = gcnew String(_name);
			SetCureName(cure_id, name);
		}
		void SetCureName(int cure_id, String^ name) {
			switch (cure_id) {
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
			}
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
			this->groupBoxRooms = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonLustra15 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra12 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra14 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra13 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra11 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonLustra10 = (gcnew System::Windows::Forms::RadioButton());
			this->buttonRoom14Explosion = (gcnew System::Windows::Forms::Button());
			this->buttonRoom13Explosion = (gcnew System::Windows::Forms::Button());
			this->checkBoxNoLustra = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxPills = (gcnew System::Windows::Forms::GroupBox());
			this->buttonNextMedTick = (gcnew System::Windows::Forms::Button());
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
			this->labelPillsCharges = (gcnew System::Windows::Forms::Label());
			this->numericUpDownPillsCharges = (gcnew System::Windows::Forms::NumericUpDown());
			this->buttonConnectPill = (gcnew System::Windows::Forms::Button());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
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
			this->groupBoxRooms->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->groupBoxPills->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownPillsCharges))->BeginInit();
			this->tabPage5->SuspendLayout();
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
			this->Screen->Size = System::Drawing::Size(160, 128);
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
			this->buttonA->Location = System::Drawing::Point(19, 51);
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
			this->buttonB->Location = System::Drawing::Point(19, 86);
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
			this->buttonC->Location = System::Drawing::Point(19, 118);
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
			this->buttonX->Location = System::Drawing::Point(308, 64);
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
			this->buttonY->Location = System::Drawing::Point(308, 101);
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
			this->buttonZ->Location = System::Drawing::Point(308, 135);
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
			this->ScreenZoomed->Size = System::Drawing::Size(640, 512);
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
			this->tabPage3->Controls->Add(this->groupBoxRooms);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(410, 243);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Отсеки";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// groupBoxRooms
			// 
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra15);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra12);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra14);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra13);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra11);
			this->groupBoxRooms->Controls->Add(this->radioButtonLustra10);
			this->groupBoxRooms->Controls->Add(this->buttonRoom14Explosion);
			this->groupBoxRooms->Controls->Add(this->buttonRoom13Explosion);
			this->groupBoxRooms->Controls->Add(this->checkBoxNoLustra);
			this->groupBoxRooms->Location = System::Drawing::Point(0, 0);
			this->groupBoxRooms->Name = L"groupBoxRooms";
			this->groupBoxRooms->Size = System::Drawing::Size(410, 238);
			this->groupBoxRooms->TabIndex = 0;
			this->groupBoxRooms->TabStop = false;
			this->groupBoxRooms->Text = L"Люстра / Отсек";
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
			// checkBoxNoLustra
			// 
			this->checkBoxNoLustra->AutoSize = true;
			this->checkBoxNoLustra->Location = System::Drawing::Point(15, 137);
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
			this->tabPage4->Text = L"Пилюли";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// groupBoxPills
			// 
			this->groupBoxPills->Controls->Add(this->buttonNextMedTick);
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
			this->buttonNextMedTick->Location = System::Drawing::Point(242, 191);
			this->buttonNextMedTick->Name = L"buttonNextMedTick";
			this->buttonNextMedTick->Size = System::Drawing::Size(146, 23);
			this->buttonNextMedTick->TabIndex = 26;
			this->buttonNextMedTick->Text = L"Next Med Tick";
			this->buttonNextMedTick->UseVisualStyleBackColor = true;
			this->buttonNextMedTick->Click += gcnew System::EventHandler(this, &MainForm::buttonNextMedTick_Click);
			// 
			// radioButtonCure12
			// 
			this->radioButtonCure12->AutoSize = true;
			this->radioButtonCure12->Location = System::Drawing::Point(242, 95);
			this->radioButtonCure12->Name = L"radioButtonCure12";
			this->radioButtonCure12->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure12->TabIndex = 25;
			this->radioButtonCure12->TabStop = true;
			this->radioButtonCure12->Text = L"Cure 12";
			this->radioButtonCure12->UseVisualStyleBackColor = true;
			this->radioButtonCure12->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure12_CheckedChanged);
			// 
			// radioButtonCure11
			// 
			this->radioButtonCure11->AutoSize = true;
			this->radioButtonCure11->Location = System::Drawing::Point(242, 70);
			this->radioButtonCure11->Name = L"radioButtonCure11";
			this->radioButtonCure11->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure11->TabIndex = 24;
			this->radioButtonCure11->TabStop = true;
			this->radioButtonCure11->Text = L"Cure 11";
			this->radioButtonCure11->UseVisualStyleBackColor = true;
			this->radioButtonCure11->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure11_CheckedChanged);
			// 
			// radioButtonCure10
			// 
			this->radioButtonCure10->AutoSize = true;
			this->radioButtonCure10->Location = System::Drawing::Point(242, 45);
			this->radioButtonCure10->Name = L"radioButtonCure10";
			this->radioButtonCure10->Size = System::Drawing::Size(68, 19);
			this->radioButtonCure10->TabIndex = 23;
			this->radioButtonCure10->TabStop = true;
			this->radioButtonCure10->Text = L"Cure 10";
			this->radioButtonCure10->UseVisualStyleBackColor = true;
			this->radioButtonCure10->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure10_CheckedChanged);
			// 
			// radioButtonCure9
			// 
			this->radioButtonCure9->AutoSize = true;
			this->radioButtonCure9->Location = System::Drawing::Point(242, 20);
			this->radioButtonCure9->Name = L"radioButtonCure9";
			this->radioButtonCure9->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure9->TabIndex = 22;
			this->radioButtonCure9->TabStop = true;
			this->radioButtonCure9->Text = L"Cure 9";
			this->radioButtonCure9->UseVisualStyleBackColor = true;
			this->radioButtonCure9->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure9_CheckedChanged);
			// 
			// radioButtonCure8
			// 
			this->radioButtonCure8->AutoSize = true;
			this->radioButtonCure8->Location = System::Drawing::Point(121, 95);
			this->radioButtonCure8->Name = L"radioButtonCure8";
			this->radioButtonCure8->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure8->TabIndex = 21;
			this->radioButtonCure8->TabStop = true;
			this->radioButtonCure8->Text = L"Cure 8";
			this->radioButtonCure8->UseVisualStyleBackColor = true;
			this->radioButtonCure8->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure8_CheckedChanged);
			// 
			// radioButtonCure7
			// 
			this->radioButtonCure7->AutoSize = true;
			this->radioButtonCure7->Location = System::Drawing::Point(121, 70);
			this->radioButtonCure7->Name = L"radioButtonCure7";
			this->radioButtonCure7->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure7->TabIndex = 20;
			this->radioButtonCure7->TabStop = true;
			this->radioButtonCure7->Text = L"Cure 7";
			this->radioButtonCure7->UseVisualStyleBackColor = true;
			this->radioButtonCure7->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure7_CheckedChanged);
			// 
			// radioButtonCure6
			// 
			this->radioButtonCure6->AutoSize = true;
			this->radioButtonCure6->Location = System::Drawing::Point(121, 45);
			this->radioButtonCure6->Name = L"radioButtonCure6";
			this->radioButtonCure6->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure6->TabIndex = 19;
			this->radioButtonCure6->TabStop = true;
			this->radioButtonCure6->Text = L"Cure 6";
			this->radioButtonCure6->UseVisualStyleBackColor = true;
			this->radioButtonCure6->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure6_CheckedChanged);
			// 
			// radioButtonCure5
			// 
			this->radioButtonCure5->AutoSize = true;
			this->radioButtonCure5->Location = System::Drawing::Point(121, 20);
			this->radioButtonCure5->Name = L"radioButtonCure5";
			this->radioButtonCure5->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure5->TabIndex = 18;
			this->radioButtonCure5->TabStop = true;
			this->radioButtonCure5->Text = L"Cure 5";
			this->radioButtonCure5->UseVisualStyleBackColor = true;
			this->radioButtonCure5->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure5_CheckedChanged);
			// 
			// radioButtonCure4
			// 
			this->radioButtonCure4->AutoSize = true;
			this->radioButtonCure4->Location = System::Drawing::Point(15, 95);
			this->radioButtonCure4->Name = L"radioButtonCure4";
			this->radioButtonCure4->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure4->TabIndex = 17;
			this->radioButtonCure4->TabStop = true;
			this->radioButtonCure4->Text = L"Cure 4";
			this->radioButtonCure4->UseVisualStyleBackColor = true;
			this->radioButtonCure4->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure4_CheckedChanged);
			// 
			// radioButtonCure3
			// 
			this->radioButtonCure3->AutoSize = true;
			this->radioButtonCure3->Location = System::Drawing::Point(15, 70);
			this->radioButtonCure3->Name = L"radioButtonCure3";
			this->radioButtonCure3->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure3->TabIndex = 16;
			this->radioButtonCure3->TabStop = true;
			this->radioButtonCure3->Text = L"Cure 3";
			this->radioButtonCure3->UseVisualStyleBackColor = true;
			this->radioButtonCure3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure3_CheckedChanged);
			// 
			// radioButtonCure2
			// 
			this->radioButtonCure2->AutoSize = true;
			this->radioButtonCure2->Location = System::Drawing::Point(15, 45);
			this->radioButtonCure2->Name = L"radioButtonCure2";
			this->radioButtonCure2->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure2->TabIndex = 15;
			this->radioButtonCure2->TabStop = true;
			this->radioButtonCure2->Text = L"Cure 2";
			this->radioButtonCure2->UseVisualStyleBackColor = true;
			this->radioButtonCure2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure2_CheckedChanged);
			// 
			// radioButtonCure1
			// 
			this->radioButtonCure1->AutoSize = true;
			this->radioButtonCure1->Location = System::Drawing::Point(15, 20);
			this->radioButtonCure1->Name = L"radioButtonCure1";
			this->radioButtonCure1->Size = System::Drawing::Size(61, 19);
			this->radioButtonCure1->TabIndex = 14;
			this->radioButtonCure1->TabStop = true;
			this->radioButtonCure1->Text = L"Cure 1";
			this->radioButtonCure1->UseVisualStyleBackColor = true;
			this->radioButtonCure1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButtonCure1_CheckedChanged);
			// 
			// labelPillsCharges
			// 
			this->labelPillsCharges->AutoSize = true;
			this->labelPillsCharges->Location = System::Drawing::Point(12, 139);
			this->labelPillsCharges->Name = L"labelPillsCharges";
			this->labelPillsCharges->Size = System::Drawing::Size(236, 15);
			this->labelPillsCharges->TabIndex = 13;
			this->labelPillsCharges->Text = L"Число зарядов (в любой пилюле) {0..99}";
			// 
			// numericUpDownPillsCharges
			// 
			this->numericUpDownPillsCharges->Location = System::Drawing::Point(256, 137);
			this->numericUpDownPillsCharges->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			this->numericUpDownPillsCharges->Name = L"numericUpDownPillsCharges";
			this->numericUpDownPillsCharges->Size = System::Drawing::Size(54, 21);
			this->numericUpDownPillsCharges->TabIndex = 12;
			this->numericUpDownPillsCharges->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			this->numericUpDownPillsCharges->Leave += gcnew System::EventHandler(this, &MainForm::numericUpDownPillsCharges_Leave);
			this->numericUpDownPillsCharges->Enter += gcnew System::EventHandler(this, &MainForm::numericUpDownPillsCharges_Enter);
			// 
			// buttonConnectPill
			// 
			this->buttonConnectPill->Location = System::Drawing::Point(25, 168);
			this->buttonConnectPill->Name = L"buttonConnectPill";
			this->buttonConnectPill->Size = System::Drawing::Size(146, 23);
			this->buttonConnectPill->TabIndex = 11;
			this->buttonConnectPill->Text = L"Connect Pill";
			this->buttonConnectPill->UseVisualStyleBackColor = true;
			this->buttonConnectPill->Click += gcnew System::EventHandler(this, &MainForm::buttonConnectPill_Click);
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->LogWindow);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(410, 243);
			this->tabPage5->TabIndex = 4;
			this->tabPage5->Text = L"Log";
			this->tabPage5->UseVisualStyleBackColor = true;
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
			this->groupBoxRooms->ResumeLayout(false);
			this->groupBoxRooms->PerformLayout();
			this->tabPage4->ResumeLayout(false);
			this->groupBoxPills->ResumeLayout(false);
			this->groupBoxPills->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownPillsCharges))->EndInit();
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

//INITIALIZATION
private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void MainForm_Shown(System::Object^  sender, System::EventArgs^  e) {
			 LowLevel::LowLevelLibMain();
		 }
//TIMER - main output event
private: System::Void Render_Tick(System::Object^  sender, System::EventArgs^  e) {
			 FormHelper::Render();
			 Screen->Image = FormHelper::bmp;
			 ScreenZoomed->Image = FormHelper::bmp;
			 if (bVibrating) {
				int step = 25;
				bVibratingReverseDirection = !bVibratingReverseDirection;
				if (bVibratingReverseDirection) {
					step = -step;
				}
				Left += step;
				bVibrationTicks--;
				if (!bVibrationTicks)
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
//BUTTONS and BUTTONS accelerator keys (Press/Release)
private: System::Void MainForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if (bKeysToButtons) 
			 {
				int button = FormHelper::KeyToButton(e->KeyCode);
				if (button != NO_BUTTON)
					 LowLevel::OnButtonPress(button);
			 }
		 }
private: System::Void MainForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if (bKeysToButtons) 
			 {
				int button = FormHelper::KeyToButton(e->KeyCode);
				if (button != NO_BUTTON)
					 LowLevel::OnButtonRelease(button);
			}
		 }
private: System::Void buttonA_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_A);
		 }
private: System::Void buttonA_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_A);
		 }
private: System::Void buttonB_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_B);
		 }
private: System::Void buttonB_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_B);
		 }
private: System::Void buttonC_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_C);
		 }
private: System::Void buttonC_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_C);
		 }
private: System::Void buttonX_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_X);
		 }
private: System::Void buttonX_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_X);
		 }
private: System::Void buttonY_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_Y);
		 }
private: System::Void buttonY_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_Y);
		 }
private: System::Void buttonZ_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_Z);
		 }
private: System::Void buttonZ_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_Z);
		 }
private: System::Void buttonL_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_L);
		 }
private: System::Void buttonL_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_L);
		 }
private: System::Void buttonE_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_E);
		 }
private: System::Void buttonE_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_E);
		 }
private: System::Void buttonR_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonRelease(BUTTON_R);
		 }
private: System::Void buttonR_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 LowLevel::OnButtonPress(BUTTON_R);
		 }
//REGENERATION LEVEL
private: System::Void radioButtonRegenerationLevel1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonRegenerationLevel1->Checked;
			if (b)
				LowLevel::SetRegenerationLevel(1);
		 }
private: System::Void radioButtonRegenerationLevel2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonRegenerationLevel2->Checked;
			if (b)
				LowLevel::SetRegenerationLevel(2);
		 }
private: System::Void radioButtonRegenerationLevel3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonRegenerationLevel3->Checked;
 			if (b)
				LowLevel::SetRegenerationLevel(3);
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
			LowLevel::SetLockAccess(1,checkBoxLock1->Checked);
		 }
private: System::Void checkBoxLock2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			LowLevel::SetLockAccess(2,checkBoxLock2->Checked);
		 }
private: System::Void checkBoxLock3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			LowLevel::SetLockAccess(3,checkBoxLock3->Checked);
		 }
private: System::Void radioButtonLock1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLock1->Checked;
 			if (b)
				LowLevel::SetCurrentLock(1);
		 }
private: System::Void radioButtonLock2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLock2->Checked;
 			if (b)
				LowLevel::SetCurrentLock(2);
		 }
private: System::Void radioButtonLock3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLock3->Checked;
 			if (b)
				LowLevel::SetCurrentLock(3);
		 }
private: System::Void radioButtonLockNo_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			bool b = radioButtonLockNo->Checked;
 			if (b)
				LowLevel::SetCurrentLock(0);
		 }
//ROOMS
private: System::Void radioButtonLustra10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra10->Checked;
			 if (b)
				LowLevel::SetCurrentLustra(10);
		 }
private: System::Void radioButtonLustra11_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra11->Checked;
			 if (b)
				LowLevel::SetCurrentLustra(11);
		 }
private: System::Void radioButtonLustra12_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra12->Checked;
			 if (b)
				LowLevel::SetCurrentLustra(12); //NoRoom
		 }
private: System::Void radioButtonLustra13_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra13->Checked;
			 if (b)
				LowLevel::SetCurrentLustra(13);
		 }
private: System::Void radioButtonLustra14_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra14->Checked;
			 if (b)
				LowLevel::SetCurrentLustra(14);
		 }
private: System::Void radioButtonLustra15_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 bool b = radioButtonLustra15->Checked;
			 if (b)
				LowLevel::SetCurrentLustra(15); //NoRoom
		 }
private: System::Void checkBoxNoLustra_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				LowLevel::LustraInRange(checkBoxNoLustra->Checked);
		 }
private: System::Void buttonRoom13Explosion_Click(System::Object^  sender, System::EventArgs^  e) {
			 LowLevel::Explosion(13);
		 }
private: System::Void buttonRoom14Explosion_Click(System::Object^  sender, System::EventArgs^  e) {
			 LowLevel::Explosion(14);
		 }
//PILLS(CURES)
private: System::Void radioButtonCure1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure1->Checked)
				 currCure = 1;
		 }
private: System::Void radioButtonCure2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure2->Checked)
				 currCure = 2;
		 }
private: System::Void radioButtonCure3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure3->Checked)
				 currCure = 3;
		 }
private: System::Void radioButtonCure4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure4->Checked)
				 currCure = 4;
		 }
private: System::Void radioButtonCure5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure5->Checked)
				 currCure = 5;
		 }
private: System::Void radioButtonCure6_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure6->Checked)
				 currCure = 6;
		 }
private: System::Void radioButtonCure7_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure7->Checked)
				 currCure = 7;
		 }
private: System::Void radioButtonCure8_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure8->Checked)
				 currCure = 8;
		 }
private: System::Void radioButtonCure9_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure9->Checked)
				 currCure = 9;
		 }
private: System::Void radioButtonCure10_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure10->Checked)
				 currCure = 10;
		 }
private: System::Void radioButtonCure11_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure11->Checked)
				 currCure = 11;
		 }
private: System::Void radioButtonCure12_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (radioButtonCure12->Checked)
				 currCure = 12;
		 }
private: System::Void buttonConnectPill_Click(System::Object^  sender, System::EventArgs^  e) {
			 LowLevel::OnPillConnect(
				 currCure, 
				 (int)numericUpDownPillsCharges->Value);
		 }
private: System::Void buttonNextMedTick_Click(System::Object^  sender, System::EventArgs^  e) {
			 LowLevel::NextMedTick();
		 }
}; //class

} //namespace
