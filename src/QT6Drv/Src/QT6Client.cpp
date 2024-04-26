//
// Created by melissaa on 25/04/24.
//
#include "../Inc/QT6Drv.h"

IMPLEMENT_CLASS(UQT6Client);


/**
 * Constructor
 * @return
 */
UQT6Client::UQT6Client()
{
	guard(USDLClient::USDLClient);
	StartupFullscreen = FALSE;
	unguard;
}

/**
 * Static Constructor
 */
void UQT6Client::StaticConstructor()
{
	guard(USDLClient::StaticConstructor);

	new(GetClass(),TEXT("StartupFullscreen"),    RF_Public)UBoolProperty(CPP_PROPERTY(StartupFullscreen),    TEXT("Display"),  CPF_Config );

	// Sensible default values
	WindowedViewportX=1024;
	WindowedViewportY=768;
	WindowedColorBits=32;
	FullscreenViewportX=1024;
	FullscreenViewportY=768;
	FullscreenColorBits=32;
	Brightness=0.5;
	MipFactor=1.0;
	CaptureMouse=true;
	ScreenFlashes=true;
	Decals=true;
	MinDesiredFrameRate=30.0;

	unguard;
}

void UQT6Client::Init(UEngine *InEngine) {
	guard(USDLClient::USDLClient);

	GClient = this;

	// Init base.
	UClient::Init( InEngine );

	// Note configuration.
	PostEditChange();

	// Success.
	debugf( NAME_Init, TEXT("QT6Client initialized.") );

	unguard;
}
