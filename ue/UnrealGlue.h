//
// Created by melissaa on 2022-04-30.
//

#ifndef MULTIED_UNREALGLUE_H
#define MULTIED_UNREALGLUE_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "../helpers/Defines.h"
// Because including ue stuff here would cause QT to freak out, create some forward declarations as needed.
// (I'm sorry!)

class UEngine;
class UEditorEngine;
class UViewport;

namespace Components {
    class qtLogWindow;
}

class UnrealGlue {

public:

    explicit UnrealGlue(intptr_t* pWinID) {}

    void Boot(int argc, char* argv[]);
    bool Loop();
    void InitBrushBuilders();

    void BeforeMapLoad();
    void AfterMapLoad();

    void MakeViewports();

    SDL_Window* m_pWindow;

    std::vector<intptr_t*> m_pWndIDs;
    std::vector<Helpers::ViewportModes> m_nViewportModes;

private:
    // Returns UEngine*, however QT and ue were interfering, so cast it yourself.
    static UEngine* InitEngine();
    UViewport* CreateViewport(const wchar_t*Name, int RenMap, int Flags);

    std::vector<UViewport*> m_pViewports;
    std::vector<int32_t> m_vViewportFlagsBeforeMapLoad;

};


#endif //MULTIED_UNREALGLUE_H
