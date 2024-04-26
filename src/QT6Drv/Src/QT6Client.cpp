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

void UQT6Client::Destroy() {
	UClient::Destroy();
}

void UQT6Client::NotifyDestroy(void *Src) {
	FNotifyHook::NotifyDestroy(Src);
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

void UQT6Client::ShowViewportWindows(DWORD ShowFlags, int DoShow) {

}

void UQT6Client::EnableViewportWindows(DWORD ShowFlags, int DoEnable) {
}

void UQT6Client::Tick() {
}

UBOOL UQT6Client::Exec(const TCHAR *Cmd, FOutputDevice &Ar) {
}

UViewport * UQT6Client::NewViewport(const FName Name) {
}

void UQT6Client::MakeCurrent(UViewport *NewViewport) {
}

UBOOL UQT6Client::SetClipboardText(FString &Text) {
}

INT UQT6Client::GetDPIScaledX(INT X) {
}

INT UQT6Client::GetDPIScaledY(INT Y) {
}

void * UQT6Client::CreateTTFFont(const TCHAR *FontName, int Height, int Italic, int Bold, int Underlined,
	int AntiAliased) {
}

void UQT6Client::MakeGlyphsList(FBitmask &RequestedGlyphs, void *InFont, TArray<FGlyphInfo> &OutGlyphs, INT AntiAlias) {
}

void UQT6Client::RenderGlyph(FGlyphInfo &Info, UTexture *PageTexture, INT X, INT Y, INT YAdjust) {
}

void UQT6Client::DestroyGlyphsList(TArray<FGlyphInfo> &Glyphs) {
}

void UQT6Client::DestroyTTFFont(void *Font) {
}
