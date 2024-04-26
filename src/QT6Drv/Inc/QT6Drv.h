//
// Created by melissaa on 25/04/24.
// Based on SDLDrv, for MultiEd
//

#ifndef QT6DRV_H
#define QT6DRV_H

#ifndef QT6DRV_API
    #define QT6DRV_API DLL_IMPORT
#endif


// Unreal includes.
#include "Engine.h"
#include "UnRender.h"

// Classes.
class UQT6Viewport;
class UQT6Client;


class UQT6Client : public UClient, public FNotifyHook {
    DECLARE_CLASS(UQT6Client,UClient,CLASS_Transient|CLASS_Config,QT6Drv)

    UBOOL StartupFullscreen;

    // Constructors.
    UQT6Client();
    void StaticConstructor();

    // FNotifyHook interface.
    void NotifyDestroy( void* Src );

    void Init(UEngine *InEngine) override;

    void ShowViewportWindows(DWORD ShowFlags, int DoShow) override;

    void EnableViewportWindows(DWORD ShowFlags, int DoEnable) override;

    void Tick() override;

    UBOOL Exec(const TCHAR *Cmd, FOutputDevice &Ar) override;

    UViewport * NewViewport(const FName Name) override;

    void MakeCurrent(UViewport *NewViewport) override;

    FString GetClipboardText() override;

    UBOOL SetClipboardText(FString &Text) override;

    INT GetDPIScaledX(INT X) override;

    INT GetDPIScaledY(INT Y) override;

    void * CreateTTFFont(const TCHAR *FontName, int Height, int Italic, int Bold, int Underlined,
        int AntiAliased) override;

    void MakeGlyphsList(FBitmask &RequestedGlyphs, void *InFont, TArray<FGlyphInfo> &OutGlyphs, INT AntiAlias) override;

    void RenderGlyph(FGlyphInfo &Info, UTexture *PageTexture, INT X, INT Y, INT YAdjust) override;

    void DestroyGlyphsList(TArray<FGlyphInfo> &Glyphs) override;

    void DestroyTTFFont(void *Font) override;
};

// QT6Viewport
class UQT6Viewport : public UViewport {
    DECLARE_CLASS(UQT6Viewport,UViewport,CLASS_Transient,QT6Drv)
    DECLARE_WITHIN(UQT6Client)

    UBOOL IsFullscreen() override;

    UBOOL ResizeViewport(DWORD BlitType, INT X, INT Y, INT ColorBytes) override;

    void SetModeCursor() override;

    void UpdateWindowFrame() override;

    void OpenWindow(void *ParentWindow, UBOOL Temporary, INT NewX, INT NewY, INT OpenX, INT OpenY,
        const TCHAR *ForcedRenDevClass) override;

    void CloseWindow() override;

    void UpdateInput(UBOOL Reset) override;

    void * GetWindow() override;

    void SetMouseCapture(UBOOL Capture, UBOOL Clip, UBOOL FocusOnly) override;

    void Repaint(UBOOL Blit) override;
};

#endif //QT6DRV_H
