//
// Created by melissaa on 2022-05-03.
//

#include "SDLHelper.h"
#include <SDL2/SDL_syswm.h>

intptr_t* Helpers::GetWindowHandle(SDL_Window *pWindow) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(pWindow, &wmInfo);

    intptr_t* pWndID = nullptr;

    // Set the WndID so we can embed the window
    switch(wmInfo.subsystem) {
#ifdef __LINUX_X86__
        case SDL_SYSWM_X11:
            pWndID = (intptr_t*)wmInfo.info.x11.window;
            break;
#endif
#ifdef WIN32
            case SDL_SYSWM_WINDOWS:
                        pWndID = (intptr_t*)wmInfo.info.win.window;
                        break;
#endif
        default:
            //appErrorf(TEXT("Platform not supported"));
            return nullptr;
    }

    return pWndID;
}
