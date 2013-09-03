#include "Honor2.h"

char* Honor2Logic::OnNewWound( TARGET trg )
{
	return _medNewWound(trg);
}

fresult Honor2Logic::Init( Honor2App* app )
{
	_medInit();
	return SUCCESS;
}

fresult Honor2Logic::OnKnockout()
{
	return SUCCESS;
}

fresult Honor2Logic::OnPillConnected( CURE_ID pillId )
{
	return SUCCESS;
}

fresult Honor2Logic::OnExposion()
{
	return SUCCESS;
}

fresult Honor2Logic::SetRegenRate( ubyte_t regenRate )
{
	return SUCCESS;
}

fresult Honor2Logic::SetPlayerName( char* playerName )
{
	return SUCCESS;
}

fresult Honor2Logic::ShowMessage( char* title, ImageHandle iconHandle, char* text )
{
	return SUCCESS;
}

fresult Honor2Logic::AppendLog( LogKinds log, char* message )
{
	return SUCCESS;
}

fresult Honor2Logic::AppendMainStatus( char* mainStatus )
{
	return SUCCESS;
}

fresult Honor2Logic::OnRoomPresenceDetected( sbyte_t roomId )
{
	return SUCCESS;
}

fresult Honor2Logic::OnMedTick()
{
	return SUCCESS;
}

fresult Honor2Logic::ReportError( char* errorText )
{
	return SUCCESS;
}

fresult Honor2Logic::OnNewMessage( char* messageText )
{
	return SUCCESS;
}
