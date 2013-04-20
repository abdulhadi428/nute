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
	private: System::Windows::Forms::Timer^  Buttons;
	//SCREEN ZOOMED	
	private: System::Windows::Forms::PictureBox^  ScreenZoomed;
	//ARMLET
	private: System::Windows::Forms::GroupBox^  groupBox1;
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
	//TAB CONTROL
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
		//TAB PAGE 1
		private: System::Windows::Forms::Button^  Pill1;
		private: System::Windows::Forms::Button^  Pill2;
		private: System::Windows::Forms::Button^  ReceiveButton;
		private: System::Windows::Forms::TextBox^  MessageBox;
		private: System::Windows::Forms::Label^  MessageBoxLabel;
		//TAB PAGE 2
		private: System::Windows::Forms::GroupBox^  groupBox2;
		private: System::Windows::Forms::TextBox^  LogWindow;

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
			this->Pill1 = (gcnew System::Windows::Forms::Button());
			this->Pill2 = (gcnew System::Windows::Forms::Button());
			this->LogWindow = (gcnew System::Windows::Forms::TextBox());
			this->ReceiveButton = (gcnew System::Windows::Forms::Button());
			this->MessageBox = (gcnew System::Windows::Forms::TextBox());
			this->MessageBoxLabel = (gcnew System::Windows::Forms::Label());
			this->Render = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->Buttons = (gcnew System::Windows::Forms::Timer(this->components));
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Screen))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenZoomed))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
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
			// 
			// Pill1
			// 
			this->Pill1->Location = System::Drawing::Point(19, 49);
			this->Pill1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->Pill1->Name = L"Pill1";
			this->Pill1->Size = System::Drawing::Size(87, 26);
			this->Pill1->TabIndex = 14;
			this->Pill1->Text = L"Pill1 (TODO)";
			this->Pill1->UseVisualStyleBackColor = true;
			// 
			// Pill2
			// 
			this->Pill2->Location = System::Drawing::Point(19, 82);
			this->Pill2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->Pill2->Name = L"Pill2";
			this->Pill2->Size = System::Drawing::Size(87, 26);
			this->Pill2->TabIndex = 15;
			this->Pill2->Text = L"Pill2 (TODO)";
			// 
			// LogWindow
			// 
			this->LogWindow->Location = System::Drawing::Point(3, 7);
			this->LogWindow->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->LogWindow->Multiline = true;
			this->LogWindow->Name = L"LogWindow";
			this->LogWindow->ReadOnly = true;
			this->LogWindow->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->LogWindow->Size = System::Drawing::Size(401, 238);
			this->LogWindow->TabIndex = 16;
			// 
			// ReceiveButton
			// 
			this->ReceiveButton->Location = System::Drawing::Point(124, 201);
			this->ReceiveButton->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->ReceiveButton->Name = L"ReceiveButton";
			this->ReceiveButton->Size = System::Drawing::Size(165, 26);
			this->ReceiveButton->TabIndex = 17;
			this->ReceiveButton->Text = L"ReceiveViaRadio (TODO)";
			this->ReceiveButton->UseVisualStyleBackColor = true;
			// 
			// MessageBox
			// 
			this->MessageBox->Location = System::Drawing::Point(124, 49);
			this->MessageBox->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->MessageBox->Multiline = true;
			this->MessageBox->Name = L"MessageBox";
			this->MessageBox->Size = System::Drawing::Size(270, 145);
			this->MessageBox->TabIndex = 19;
			// 
			// MessageBoxLabel
			// 
			this->MessageBoxLabel->AutoSize = true;
			this->MessageBoxLabel->Location = System::Drawing::Point(121, 24);
			this->MessageBoxLabel->Name = L"MessageBoxLabel";
			this->MessageBoxLabel->Size = System::Drawing::Size(82, 15);
			this->MessageBoxLabel->TabIndex = 20;
			this->MessageBoxLabel->Text = L"Message Box";
			// 
			// Render
			// 
			this->Render->Enabled = true;
			this->Render->Interval = 50;
			this->Render->Tick += gcnew System::EventHandler(this, &MainForm::Render_Tick);
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::Black;
			this->groupBox1->Controls->Add(this->buttonB);
			this->groupBox1->Controls->Add(this->Screen);
			this->groupBox1->Controls->Add(this->buttonA);
			this->groupBox1->Controls->Add(this->buttonC);
			this->groupBox1->Controls->Add(this->buttonX);
			this->groupBox1->Controls->Add(this->buttonY);
			this->groupBox1->Controls->Add(this->buttonZ);
			this->groupBox1->Controls->Add(this->buttonL);
			this->groupBox1->Controls->Add(this->buttonR);
			this->groupBox1->Controls->Add(this->buttonE);
			this->groupBox1->ForeColor = System::Drawing::Color::White;
			this->groupBox1->Location = System::Drawing::Point(12, 13);
			this->groupBox1->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox1->Size = System::Drawing::Size(418, 236);
			this->groupBox1->TabIndex = 21;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Браслет";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->MessageBox);
			this->groupBox2->Controls->Add(this->Pill1);
			this->groupBox2->Controls->Add(this->MessageBoxLabel);
			this->groupBox2->Controls->Add(this->Pill2);
			this->groupBox2->Controls->Add(this->ReceiveButton);
			this->groupBox2->Location = System::Drawing::Point(0, 0);
			this->groupBox2->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(3, 4, 3, 4);
			this->groupBox2->Size = System::Drawing::Size(418, 245);
			this->groupBox2->TabIndex = 22;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"groupBox2";
			// 
			// Buttons
			// 
			this->Buttons->Enabled = true;
			this->Buttons->Interval = 500;
			this->Buttons->Tick += gcnew System::EventHandler(this, &MainForm::Buttons_Tick);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 256);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 1;
			this->tabControl1->Size = System::Drawing::Size(418, 269);
			this->tabControl1->TabIndex = 23;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBox2);
			this->tabPage1->Location = System::Drawing::Point(4, 24);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(410, 241);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->LogWindow);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(410, 243);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Log Output";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// MainForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->AutoScroll = true;
			this->BackColor = System::Drawing::SystemColors::AppWorkspace;
			this->ClientSize = System::Drawing::Size(1094, 574);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->groupBox1);
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
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
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
};
}
