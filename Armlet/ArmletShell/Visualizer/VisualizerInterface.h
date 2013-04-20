//log support(showing always last string)
//TODO class for color translation between 12bit and 24bit
//TODO class for alpa-channelling (unmanaged)
//TODO class for palettes
//TODO class for button callbacks
//TODO draw rectangles
//THEN 3 layers library (static background + controls + animation=framebuffed images)
//THEN fonts and textframebuffers
//THEN image lists
//THEN SD file system support for images
//THEN blinking icons and menu icons
//THEN static texts

#pragma once

namespace Visualizer {
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::ComponentModel;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace System::Windows::Forms;
	using namespace System::Drawing::Imaging;

	ref class VisualizerInterface
	{
	public:
		static TextBox^ LogWindow;
		static Bitmap^ Bmp;
		static array<Int64>^ ButtonTicks;
		static array<Boolean>^ ButtonPressed;
		static array<int>^ ButtonHolded;
		static array<Boolean>^ SkipButtonRelease;

		static void Init(TextBox^ _LogWindow);
		static void Log(String^ message);
		
		static int KeyToButton(System::Windows::Forms::Keys key);
		static void OnButtonPress(int button);
		static void OnButtonRelease(int button);
		static void HalfSecButtonTimer();
		static void Render();

		//OnButtonUp
		//OnButtonDown
		//OnButtonTimer
		//DrawButtons

		//OnPillConnect
		//OnPillDisconnect
		//Pill=

		//OnLustraInRange, id= and on/off
		//OnZamokInRange, id= and on/off
		//OnRadioPacket, packet
		//

		//

		//Render - draw VideoMemory (it is in 18bpp?), default LUT 6->8 bit
		//					curr from 4->8bit (r<<4)+(r>>2)

	};
}

//(r<<2)+(r>>4) for each channel
/*
={
0x00,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C //00-07 +0
0x20,0x24,0x28,0x2C,0x30,0x34,0x38,0x3C //08-15 +0
0x41,0x45,0x49,0x4D,0x51,0x55,0x59,0x5D //16-23 +1
0x61,0x65,0x69,0x6D,0x71,0x75,0x79,0x7D //24-31 +1
0x82,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C //32-39 +2
0xA0,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C //40-47 +2
0xC0,0x04,0x08,0x0C,0x10,0x14,0x18,0x1C //48-55 +3
0xE0,0xE4,0xE8,0xEC,0xF0,0xF4,0xF8,0xFF //56-63 +3
}
*/