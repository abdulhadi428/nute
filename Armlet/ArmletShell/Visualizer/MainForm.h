#pragma once
#include "stdafx.h"

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
	private: System::Windows::Forms::Timer^  Render;
	private: System::Windows::Forms::Timer^  HalfSecondTimer;
	//SCREEN ZOOMED	
	private: System::Windows::Forms::PictureBox^  ScreenZoomed;
	private: System::Windows::Forms::GroupBox^  groupBoxMain;
			 //ARMLET

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
	//TABCONTROL
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::GroupBox^  groupBox1;
			 //TAB PAGE 1
	//TAB PAGE 2
	//TAB PAGE 3
	//TAB PAGE 4
	//TAB PAGE 5

	private: System::Windows::Forms::TextBox^  MessageBox;



	private: System::Windows::Forms::Button^  ServerButtonSendMessage;

	private: System::Windows::Forms::TextBox^  LogWindow;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  ServerButtonSetPlayerName;
	private: System::Windows::Forms::TextBox^  textBox2;



	private: System::Windows::Forms::CheckBox^  checkBox3;

	private: System::Windows::Forms::CheckBox^  checkBox1;






	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox22;






	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::GroupBox^  groupBox4;



	private: System::Windows::Forms::Button^  button9;











private: System::Windows::Forms::RadioButton^  radioButton4;
private: System::Windows::Forms::RadioButton^  radioButton3;
private: System::Windows::Forms::RadioButton^  radioButton2;
private: System::Windows::Forms::RadioButton^  radioButton1;
private: System::Windows::Forms::RadioButton^  radioButton7;
private: System::Windows::Forms::RadioButton^  radioButton6;
private: System::Windows::Forms::RadioButton^  radioButton5;
private: System::Windows::Forms::RadioButton^  radioButton13;
private: System::Windows::Forms::RadioButton^  radioButton12;
private: System::Windows::Forms::RadioButton^  radioButton10;
private: System::Windows::Forms::RadioButton^  radioButton11;
private: System::Windows::Forms::RadioButton^  radioButton9;
private: System::Windows::Forms::RadioButton^  radioButton8;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
private: System::Windows::Forms::RadioButton^  radioButton22;
private: System::Windows::Forms::RadioButton^  radioButton23;
private: System::Windows::Forms::RadioButton^  radioButton24;
private: System::Windows::Forms::RadioButton^  radioButton25;
private: System::Windows::Forms::RadioButton^  radioButton18;
private: System::Windows::Forms::RadioButton^  radioButton19;
private: System::Windows::Forms::RadioButton^  radioButton20;
private: System::Windows::Forms::RadioButton^  radioButton21;
private: System::Windows::Forms::RadioButton^  radioButton17;
private: System::Windows::Forms::RadioButton^  radioButton16;
private: System::Windows::Forms::RadioButton^  radioButton15;
private: System::Windows::Forms::RadioButton^  radioButton14;




	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Screen = (gcnew System::Windows::Forms::PictureBox());
			this->buttonA = (gcnew System::Windows::Forms::Button());
			this->buttonB = (gcnew System::Windows::Forms::Button());
			this->buttonC = (gcnew System::Windows::Forms::Button());
			this->buttonX = (gcnew System::Windows::Forms::Button());
			this->buttonY = (gcnew System::Windows::Forms::Button());
			this->buttonZ = (gcnew System::Windows::Forms::Button());
			this->buttonL = (gcnew System::Windows::Forms::Button());
			this->buttonR = (gcnew System::Windows::Forms::Button());
			this->buttonE = (gcnew System::Windows::Forms::Button());
			this->ScreenZoomed = (gcnew System::Windows::Forms::PictureBox());
			this->Render = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBoxMain = (gcnew System::Windows::Forms::GroupBox());
			this->HalfSecondTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton13 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton12 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton10 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton11 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton9 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton8 = (gcnew System::Windows::Forms::RadioButton());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->checkBox22 = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton7 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton6 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->ServerButtonSetPlayerName = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->MessageBox = (gcnew System::Windows::Forms::TextBox());
			this->ServerButtonSendMessage = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->LogWindow = (gcnew System::Windows::Forms::TextBox());
			this->radioButton14 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton15 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton16 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton17 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton18 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton19 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton20 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton21 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton22 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton23 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton24 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton25 = (gcnew System::Windows::Forms::RadioButton());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Screen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenZoomed))->BeginInit();
			this->groupBoxMain->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->groupBox4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->tabPage5->SuspendLayout();
			this->SuspendLayout();
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
			this->ScreenZoomed->Click += gcnew System::EventHandler(this, &MainForm::ScreenZoomed_Click);
			// 
			// Render
			// 
			this->Render->Enabled = true;
			this->Render->Interval = 50;
			this->Render->Tick += gcnew System::EventHandler(this, &MainForm::Render_Tick);
			// 
			// groupBoxMain
			// 
			this->groupBoxMain->BackColor = System::Drawing::Color::Black;
			this->groupBoxMain->Controls->Add(this->buttonB);
			this->groupBoxMain->Controls->Add(this->Screen);
			this->groupBoxMain->Controls->Add(this->buttonA);
			this->groupBoxMain->Controls->Add(this->buttonC);
			this->groupBoxMain->Controls->Add(this->buttonX);
			this->groupBoxMain->Controls->Add(this->buttonY);
			this->groupBoxMain->Controls->Add(this->buttonZ);
			this->groupBoxMain->Controls->Add(this->buttonL);
			this->groupBoxMain->Controls->Add(this->buttonR);
			this->groupBoxMain->Controls->Add(this->buttonE);
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
			// HalfSecondTimer
			// 
			this->HalfSecondTimer->Enabled = true;
			this->HalfSecondTimer->Interval = 500;
			this->HalfSecondTimer->Tick += gcnew System::EventHandler(this, &MainForm::Buttons_Tick);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->groupBox3);
			this->tabPage3->Location = System::Drawing::Point(4, 24);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(410, 241);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Отсеки";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->radioButton13);
			this->groupBox3->Controls->Add(this->radioButton12);
			this->groupBox3->Controls->Add(this->radioButton10);
			this->groupBox3->Controls->Add(this->radioButton11);
			this->groupBox3->Controls->Add(this->radioButton9);
			this->groupBox3->Controls->Add(this->radioButton8);
			this->groupBox3->Controls->Add(this->button2);
			this->groupBox3->Controls->Add(this->button1);
			this->groupBox3->Controls->Add(this->checkBox22);
			this->groupBox3->Location = System::Drawing::Point(0, 0);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(410, 238);
			this->groupBox3->TabIndex = 0;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Люстра / Отсек";
			// 
			// radioButton13
			// 
			this->radioButton13->AutoSize = true;
			this->radioButton13->Location = System::Drawing::Point(139, 75);
			this->radioButton13->Name = L"radioButton13";
			this->radioButton13->Size = System::Drawing::Size(113, 19);
			this->radioButton13->TabIndex = 18;
			this->radioButton13->TabStop = true;
			this->radioButton13->Text = L"15 / Вне отсека";
			this->radioButton13->UseVisualStyleBackColor = true;
			// 
			// radioButton12
			// 
			this->radioButton12->AutoSize = true;
			this->radioButton12->Location = System::Drawing::Point(15, 74);
			this->radioButton12->Name = L"radioButton12";
			this->radioButton12->Size = System::Drawing::Size(113, 19);
			this->radioButton12->TabIndex = 17;
			this->radioButton12->TabStop = true;
			this->radioButton12->Text = L"12 / Вне отсека";
			this->radioButton12->UseVisualStyleBackColor = true;
			// 
			// radioButton10
			// 
			this->radioButton10->AutoSize = true;
			this->radioButton10->Location = System::Drawing::Point(139, 48);
			this->radioButton10->Name = L"radioButton10";
			this->radioButton10->Size = System::Drawing::Size(100, 19);
			this->radioButton10->TabIndex = 16;
			this->radioButton10->TabStop = true;
			this->radioButton10->Text = L"14 / Отсек 14";
			this->radioButton10->UseVisualStyleBackColor = true;
			// 
			// radioButton11
			// 
			this->radioButton11->AutoSize = true;
			this->radioButton11->Location = System::Drawing::Point(139, 22);
			this->radioButton11->Name = L"radioButton11";
			this->radioButton11->Size = System::Drawing::Size(100, 19);
			this->radioButton11->TabIndex = 15;
			this->radioButton11->TabStop = true;
			this->radioButton11->Text = L"13 / Отсек 13";
			this->radioButton11->UseVisualStyleBackColor = true;
			// 
			// radioButton9
			// 
			this->radioButton9->AutoSize = true;
			this->radioButton9->Location = System::Drawing::Point(15, 48);
			this->radioButton9->Name = L"radioButton9";
			this->radioButton9->Size = System::Drawing::Size(100, 19);
			this->radioButton9->TabIndex = 14;
			this->radioButton9->TabStop = true;
			this->radioButton9->Text = L"11 / Отсек 11";
			this->radioButton9->UseVisualStyleBackColor = true;
			// 
			// radioButton8
			// 
			this->radioButton8->AutoSize = true;
			this->radioButton8->Location = System::Drawing::Point(15, 22);
			this->radioButton8->Name = L"radioButton8";
			this->radioButton8->Size = System::Drawing::Size(100, 19);
			this->radioButton8->TabIndex = 13;
			this->radioButton8->TabStop = true;
			this->radioButton8->Text = L"10 / Отсек 10";
			this->radioButton8->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(259, 48);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(142, 23);
			this->button2->TabIndex = 12;
			this->button2->Text = L"Взрыв в отсеке 14";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(259, 18);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(142, 23);
			this->button1->TabIndex = 11;
			this->button1->Text = L" Взрыв в отсеке 13";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// checkBox22
			// 
			this->checkBox22->AutoSize = true;
			this->checkBox22->Location = System::Drawing::Point(15, 137);
			this->checkBox22->Name = L"checkBox22";
			this->checkBox22->Size = System::Drawing::Size(177, 19);
			this->checkBox22->TabIndex = 10;
			this->checkBox22->Text = L"Люстра не детектируется";
			this->checkBox22->UseVisualStyleBackColor = true;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBox1);
			this->tabPage1->Location = System::Drawing::Point(4, 24);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(410, 241);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Сервер";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButton7);
			this->groupBox1->Controls->Add(this->radioButton6);
			this->groupBox1->Controls->Add(this->radioButton5);
			this->groupBox1->Controls->Add(this->ServerButtonSetPlayerName);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->MessageBox);
			this->groupBox1->Controls->Add(this->ServerButtonSendMessage);
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Size = System::Drawing::Size(410, 245);
			this->groupBox1->TabIndex = 22;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Основное";
			// 
			// radioButton7
			// 
			this->radioButton7->AutoSize = true;
			this->radioButton7->Location = System::Drawing::Point(15, 114);
			this->radioButton7->Name = L"radioButton7";
			this->radioButton7->Size = System::Drawing::Size(91, 19);
			this->radioButton7->TabIndex = 24;
			this->radioButton7->TabStop = true;
			this->radioButton7->Text = L"RegenLev 3";
			this->radioButton7->UseVisualStyleBackColor = true;
			// 
			// radioButton6
			// 
			this->radioButton6->AutoSize = true;
			this->radioButton6->Location = System::Drawing::Point(15, 88);
			this->radioButton6->Name = L"radioButton6";
			this->radioButton6->Size = System::Drawing::Size(91, 19);
			this->radioButton6->TabIndex = 23;
			this->radioButton6->TabStop = true;
			this->radioButton6->Text = L"RegenLev 2";
			this->radioButton6->UseVisualStyleBackColor = true;
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->Location = System::Drawing::Point(15, 62);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(91, 19);
			this->radioButton5->TabIndex = 22;
			this->radioButton5->TabStop = true;
			this->radioButton5->Text = L"RegenLev 1";
			this->radioButton5->UseVisualStyleBackColor = true;
			// 
			// ServerButtonSetPlayerName
			// 
			this->ServerButtonSetPlayerName->Location = System::Drawing::Point(287, 20);
			this->ServerButtonSetPlayerName->Name = L"ServerButtonSetPlayerName";
			this->ServerButtonSetPlayerName->Size = System::Drawing::Size(107, 23);
			this->ServerButtonSetPlayerName->TabIndex = 21;
			this->ServerButtonSetPlayerName->Text = L"SetPlayerName";
			this->ServerButtonSetPlayerName->UseVisualStyleBackColor = true;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(15, 22);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(266, 21);
			this->textBox2->TabIndex = 20;
			// 
			// MessageBox
			// 
			this->MessageBox->Location = System::Drawing::Point(107, 62);
			this->MessageBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->MessageBox->Multiline = true;
			this->MessageBox->Name = L"MessageBox";
			this->MessageBox->Size = System::Drawing::Size(287, 132);
			this->MessageBox->TabIndex = 19;
			// 
			// ServerButtonSendMessage
			// 
			this->ServerButtonSendMessage->Location = System::Drawing::Point(229, 202);
			this->ServerButtonSendMessage->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->ServerButtonSendMessage->Name = L"ServerButtonSendMessage";
			this->ServerButtonSendMessage->Size = System::Drawing::Size(165, 26);
			this->ServerButtonSendMessage->TabIndex = 17;
			this->ServerButtonSendMessage->Text = L"SendMessage";
			this->ServerButtonSendMessage->UseVisualStyleBackColor = true;
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
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBox2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(410, 243);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Замки";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->radioButton4);
			this->groupBox2->Controls->Add(this->radioButton3);
			this->groupBox2->Controls->Add(this->radioButton2);
			this->groupBox2->Controls->Add(this->radioButton1);
			this->groupBox2->Controls->Add(this->checkBox5);
			this->groupBox2->Controls->Add(this->checkBox3);
			this->groupBox2->Controls->Add(this->checkBox1);
			this->groupBox2->Location = System::Drawing::Point(0, 0);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(410, 245);
			this->groupBox2->TabIndex = 0;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Могу открыть / Виден браслету";
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(110, 106);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(169, 19);
			this->radioButton4->TabIndex = 17;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"Замок не детектируется";
			this->radioButton4->UseVisualStyleBackColor = true;
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(110, 80);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(72, 19);
			this->radioButton3->TabIndex = 16;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"Замок 3";
			this->radioButton3->UseVisualStyleBackColor = true;
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(110, 54);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(72, 19);
			this->radioButton2->TabIndex = 15;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Замок 2";
			this->radioButton2->UseVisualStyleBackColor = true;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(110, 28);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(72, 19);
			this->radioButton1->TabIndex = 14;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Замок 1";
			this->radioButton1->UseVisualStyleBackColor = true;
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Location = System::Drawing::Point(15, 77);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(77, 19);
			this->checkBox5->TabIndex = 7;
			this->checkBox5->Text = L"Доступ 3";
			this->checkBox5->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(15, 53);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(77, 19);
			this->checkBox3->TabIndex = 5;
			this->checkBox3->Text = L"Доступ 2";
			this->checkBox3->UseVisualStyleBackColor = true;
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(15, 29);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(77, 19);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Доступ 1";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->groupBox4);
			this->tabPage4->Controls->Add(this->textBox1);
			this->tabPage4->Location = System::Drawing::Point(4, 24);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(410, 241);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"Пилюли";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->radioButton22);
			this->groupBox4->Controls->Add(this->radioButton23);
			this->groupBox4->Controls->Add(this->radioButton24);
			this->groupBox4->Controls->Add(this->radioButton25);
			this->groupBox4->Controls->Add(this->radioButton18);
			this->groupBox4->Controls->Add(this->radioButton19);
			this->groupBox4->Controls->Add(this->radioButton20);
			this->groupBox4->Controls->Add(this->radioButton21);
			this->groupBox4->Controls->Add(this->radioButton17);
			this->groupBox4->Controls->Add(this->radioButton16);
			this->groupBox4->Controls->Add(this->radioButton15);
			this->groupBox4->Controls->Add(this->radioButton14);
			this->groupBox4->Controls->Add(this->label1);
			this->groupBox4->Controls->Add(this->numericUpDown1);
			this->groupBox4->Controls->Add(this->button9);
			this->groupBox4->Location = System::Drawing::Point(0, 0);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(410, 235);
			this->groupBox4->TabIndex = 1;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Лекарство / Заряды";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 139);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(198, 15);
			this->label1->TabIndex = 13;
			this->label1->Text = L"Число зарядов (в любой пилюле)";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(216, 137);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(54, 21);
			this->numericUpDown1->TabIndex = 12;
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(25, 168);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(146, 23);
			this->button9->TabIndex = 11;
			this->button9->Text = L"Connect Pill";
			this->button9->UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(0, 0);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(407, 241);
			this->textBox1->TabIndex = 0;
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
			this->tabPage5->Click += gcnew System::EventHandler(this, &MainForm::tabPage5_Click);
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
			// 
			// radioButton14
			// 
			this->radioButton14->AutoSize = true;
			this->radioButton14->Location = System::Drawing::Point(15, 20);
			this->radioButton14->Name = L"radioButton14";
			this->radioButton14->Size = System::Drawing::Size(61, 19);
			this->radioButton14->TabIndex = 14;
			this->radioButton14->TabStop = true;
			this->radioButton14->Text = L"Cure 1";
			this->radioButton14->UseVisualStyleBackColor = true;
			// 
			// radioButton15
			// 
			this->radioButton15->AutoSize = true;
			this->radioButton15->Location = System::Drawing::Point(15, 45);
			this->radioButton15->Name = L"radioButton15";
			this->radioButton15->Size = System::Drawing::Size(61, 19);
			this->radioButton15->TabIndex = 15;
			this->radioButton15->TabStop = true;
			this->radioButton15->Text = L"Cure 2";
			this->radioButton15->UseVisualStyleBackColor = true;
			// 
			// radioButton16
			// 
			this->radioButton16->AutoSize = true;
			this->radioButton16->Location = System::Drawing::Point(15, 70);
			this->radioButton16->Name = L"radioButton16";
			this->radioButton16->Size = System::Drawing::Size(61, 19);
			this->radioButton16->TabIndex = 16;
			this->radioButton16->TabStop = true;
			this->radioButton16->Text = L"Cure 3";
			this->radioButton16->UseVisualStyleBackColor = true;
			// 
			// radioButton17
			// 
			this->radioButton17->AutoSize = true;
			this->radioButton17->Location = System::Drawing::Point(15, 95);
			this->radioButton17->Name = L"radioButton17";
			this->radioButton17->Size = System::Drawing::Size(61, 19);
			this->radioButton17->TabIndex = 17;
			this->radioButton17->TabStop = true;
			this->radioButton17->Text = L"Cure 4";
			this->radioButton17->UseVisualStyleBackColor = true;
			// 
			// radioButton18
			// 
			this->radioButton18->AutoSize = true;
			this->radioButton18->Location = System::Drawing::Point(121, 95);
			this->radioButton18->Name = L"radioButton18";
			this->radioButton18->Size = System::Drawing::Size(61, 19);
			this->radioButton18->TabIndex = 21;
			this->radioButton18->TabStop = true;
			this->radioButton18->Text = L"Cure 8";
			this->radioButton18->UseVisualStyleBackColor = true;
			// 
			// radioButton19
			// 
			this->radioButton19->AutoSize = true;
			this->radioButton19->Location = System::Drawing::Point(121, 70);
			this->radioButton19->Name = L"radioButton19";
			this->radioButton19->Size = System::Drawing::Size(61, 19);
			this->radioButton19->TabIndex = 20;
			this->radioButton19->TabStop = true;
			this->radioButton19->Text = L"Cure 7";
			this->radioButton19->UseVisualStyleBackColor = true;
			// 
			// radioButton20
			// 
			this->radioButton20->AutoSize = true;
			this->radioButton20->Location = System::Drawing::Point(121, 45);
			this->radioButton20->Name = L"radioButton20";
			this->radioButton20->Size = System::Drawing::Size(61, 19);
			this->radioButton20->TabIndex = 19;
			this->radioButton20->TabStop = true;
			this->radioButton20->Text = L"Cure 6";
			this->radioButton20->UseVisualStyleBackColor = true;
			// 
			// radioButton21
			// 
			this->radioButton21->AutoSize = true;
			this->radioButton21->Location = System::Drawing::Point(121, 20);
			this->radioButton21->Name = L"radioButton21";
			this->radioButton21->Size = System::Drawing::Size(61, 19);
			this->radioButton21->TabIndex = 18;
			this->radioButton21->TabStop = true;
			this->radioButton21->Text = L"Cure 5";
			this->radioButton21->UseVisualStyleBackColor = true;
			// 
			// radioButton22
			// 
			this->radioButton22->AutoSize = true;
			this->radioButton22->Location = System::Drawing::Point(242, 95);
			this->radioButton22->Name = L"radioButton22";
			this->radioButton22->Size = System::Drawing::Size(68, 19);
			this->radioButton22->TabIndex = 25;
			this->radioButton22->TabStop = true;
			this->radioButton22->Text = L"Cure 12";
			this->radioButton22->UseVisualStyleBackColor = true;
			// 
			// radioButton23
			// 
			this->radioButton23->AutoSize = true;
			this->radioButton23->Location = System::Drawing::Point(242, 70);
			this->radioButton23->Name = L"radioButton23";
			this->radioButton23->Size = System::Drawing::Size(68, 19);
			this->radioButton23->TabIndex = 24;
			this->radioButton23->TabStop = true;
			this->radioButton23->Text = L"Cure 11";
			this->radioButton23->UseVisualStyleBackColor = true;
			// 
			// radioButton24
			// 
			this->radioButton24->AutoSize = true;
			this->radioButton24->Location = System::Drawing::Point(242, 45);
			this->radioButton24->Name = L"radioButton24";
			this->radioButton24->Size = System::Drawing::Size(68, 19);
			this->radioButton24->TabIndex = 23;
			this->radioButton24->TabStop = true;
			this->radioButton24->Text = L"Cure 10";
			this->radioButton24->UseVisualStyleBackColor = true;
			// 
			// radioButton25
			// 
			this->radioButton25->AutoSize = true;
			this->radioButton25->Location = System::Drawing::Point(242, 20);
			this->radioButton25->Name = L"radioButton25";
			this->radioButton25->Size = System::Drawing::Size(61, 19);
			this->radioButton25->TabIndex = 22;
			this->radioButton25->TabStop = true;
			this->radioButton25->Text = L"Cure 9";
			this->radioButton25->UseVisualStyleBackColor = true;
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Screen))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenZoomed))->EndInit();
			this->groupBoxMain->ResumeLayout(false);
			this->tabPage3->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->tabPage1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 VisualizerInterface::Init(LogWindow);
		 }
private: System::Void MainForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 int button = VisualizerInterface::KeyToButton(e->KeyCode);
			 if (button != NO_BUTTON)
				 VisualizerInterface::OnButtonPress(button);
		 }
private: System::Void MainForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 int button = VisualizerInterface::KeyToButton(e->KeyCode);
			 if (button != NO_BUTTON)
				 VisualizerInterface::OnButtonRelease(button);
		 }
private: System::Void buttonA_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_A);
		 }
private: System::Void buttonA_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_A);
		 }
private: System::Void buttonB_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_B);
		 }
private: System::Void buttonB_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_B);
		 }
private: System::Void buttonC_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_C);
		 }
private: System::Void buttonC_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_C);
		 }
private: System::Void buttonX_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_X);
		 }
private: System::Void buttonX_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_X);
		 }
private: System::Void buttonY_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_Y);
		 }
private: System::Void buttonY_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_Y);
		 }
private: System::Void buttonZ_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_Z);
		 }
private: System::Void buttonZ_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_Z);
		 }
private: System::Void buttonL_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_L);
		 }
private: System::Void buttonL_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_L);
		 }
private: System::Void buttonE_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_E);
		 }
private: System::Void buttonE_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_E);
		 }
private: System::Void buttonR_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonRelease(BUTTON_R);
		 }
private: System::Void buttonR_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 VisualizerInterface::OnButtonPress(BUTTON_R);
		 }
private: System::Void Buttons_Tick(System::Object^  sender, System::EventArgs^  e) {
			 VisualizerInterface::HalfSecButtonTimer();
		 }
private: System::Void Render_Tick(System::Object^  sender, System::EventArgs^  e) {
			 VisualizerInterface::Render();
			 Screen->Image = VisualizerInterface::Bmp;
			 ScreenZoomed->Image = VisualizerInterface::Bmp;
		 }
private: System::Void MainForm_Shown(System::Object^  sender, System::EventArgs^  e) {
			 LowLevelLibMain();
		 }
private: System::Void ScreenZoomed_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void tabPage5_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}
