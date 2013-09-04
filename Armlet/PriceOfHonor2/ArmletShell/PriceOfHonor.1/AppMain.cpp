void AppMainThread(void* param)
{
	_medInit();
#ifdef _MSC_VER
	for (int i=0; i<MaxCureId; i++) {
		ArmletApi::WritePill(i,i+10);
		ArmletApi::SetCureName(i, (char*)CureName[i]);
	}
	for (int i=0; i<MaxTortureId; i++) {
		ArmletApi::WritePill(i+20,i+30);
		ArmletApi::SetCureName(i+20, (char*)TortureName[i]);
	}
#endif

	//extern const unsigned short ArrowDownOrangeBitmap[];
	//DrawBitmap_kel(0,0,24,24,(short*)ArrowDownOrangeBitmap);

	fresult fres;
	InitMenuHandlerDelegates(&UI);
	InitImages();
	InitColorSchema();
	fres = UI.Init();
	if (fres!=SUCCESS)
	{
		char* err = "Failed to init UI";
		DrawTextString(10,10,err,Length(err),0,0);
	}
	else
	{
		fres = UI.Draw();
		if (fres!=SUCCESS)
		{
			char* err = "Failed to draw UI";
			DrawTextString(10,10,err,Length(err),0,0);
		}
	}

}

uword_t gx=0;
uword_t gy=0;
Color gc=0x0000;
bool MoveBoxByTimer()
{
	DrawRect(gx,gy,30,30,0);
	gc+=0x111;
	if (gc>=0x1000) gc=0x0000;
	gx+=15; gy+=5;
	if (gx>SCREENX) gx-=SCREENX;
	if (gy>SCREENY) gy-=SCREENY;
	DrawRect(gx,gy,30,30,gc);
	return false;
}


}