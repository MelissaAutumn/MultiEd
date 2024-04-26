//
// Created by melissaa on 2022-05-03.
//

#ifndef MULTIED_DEFINES_H
#define MULTIED_DEFINES_H

#include <map>

namespace Helpers {


// Glue defines
// Don't want to rely on UE enums, even though that would be easy
enum ViewportModes {
    Ignore          = -1,
    Top             = 0,
    Front           = 1,
    Side            = 2,
    Wireframe       = 3,
    TextureUsage    = 4,
    BSPCuts         = 5,
    Textured        = 6,
    DynamicLighting = 7,
    ZonesNPortals   = 8,


    MaxViewportModes = 9
};
// ShowFlags for viewport.
// Copied from UCamera
enum ViewportShowFlags
{
    SHOW_Frame     		= 0x00000001, 	// Show world bounding cube.
    SHOW_ActorRadii		= 0x00000002, 	// Show actor collision radii.
    SHOW_Backdrop  		= 0x00000004, 	// Show background scene.
    SHOW_Actors    		= 0x00000008,	// Show actors.
    SHOW_Coords    		= 0x00000010,	// Show brush/actor coords.
    SHOW_ActorIcons		= 0x00000020,	// Show actors as icons.
    SHOW_Brush			= 0x00000040,	// Show the active brush.
    SHOW_StandardView	= 0x00000080,	// Viewport is a standard view.
    SHOW_Menu			= 0x00000100,	// Show menu on viewport.
    SHOW_ChildWindow	= 0x00000200,	// Show as true child window.
    SHOW_MovingBrushes	= 0x00000400,	// Show moving brushes.
    SHOW_PlayerCtrl		= 0x00000800,	// Player controls are on.
    SHOW_Paths          = 0x00001000,   // Show paths.
    SHOW_NoButtons		= 0x00002000,	// No menu/view buttons.
    SHOW_RealTime		= 0x00004000,	// Update window in realtime.
};
// Copied from Editor.h
enum EditorModes
{
    EM_None 			= 0,	// Gameplay, editor disabled.
    EM_ViewportMove		= 1,	// Move viewport normally.
    EM_ViewportZoom		= 2,	// Move viewport with acceleration.
    EM_BrushRotate		= 5,	// Rotate brush.
    EM_BrushSheer		= 6,	// Sheer brush.
    EM_BrushScale		= 7,	// Scale brush.
    EM_BrushStretch		= 8,	// Stretch brush.
    EM_TexturePan		= 11,	// Pan textures.
    EM_TextureRotate	= 13,	// Rotate textures.
    EM_TextureScale		= 14,	// Scale textures.
    EM_BrushSnap		= 18,	// Brush snap-scale.
    EM_TexView			= 19,	// Viewing textures.
    EM_TexBrowser		= 20,	// Browsing textures.
    EM_MeshView			= 21,	// Viewing mesh.
    EM_MeshBrowser		= 22,	// Browsing mesh.
    EM_BrushClip		= 23,	// brush Clipping.
    EM_VertexEdit		= 24,	// Multiple Vertex Editing.
    EM_FaceDrag			= 25,	// Face Dragging.
};

}

extern std::map<Helpers::ViewportModes, const char*> g_ViewportModeNameMap;



#endif //MULTIED_DEFINES_H
