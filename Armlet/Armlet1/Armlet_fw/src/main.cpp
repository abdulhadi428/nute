
#define ARMLET__
//#define PILL_EDITOR__
//#define PLAYGROUND__

#ifdef PLAYGROUND__
void PlaygroundMain(void);
#elif defined PILL_EDITOR__
void PillEditorMain(void);
#elif defined ARMLET__
void ArmletMain(void);
#endif


int main(void) {
#ifdef PLAYGROUND__
	PlaygroundMain();
#elif defined PILL_EDITOR__
	PillEditorMain();
#elif defined ARMLET__
	ArmletMain();
#endif
}
