//
// Created by melissaa on 2022-05-03.
//

#include "Defines.h"
std::map<Helpers::ViewportModes, const char*> g_ViewportModeNameMap = {
    {Helpers::ViewportModes::Top, "Top"},
    {Helpers::ViewportModes::Front, "Front"},
    {Helpers::ViewportModes::Side, "Side"},
    {Helpers::ViewportModes::Wireframe, "Wireframe"},
    {Helpers::ViewportModes::TextureUsage, "TextureUsage"},
    {Helpers::ViewportModes::BSPCuts, "BSP Cuts"},
    {Helpers::ViewportModes::Textured, "Textured"},
    {Helpers::ViewportModes::DynamicLighting, "Dynamic Lighting"},
    {Helpers::ViewportModes::ZonesNPortals, "Zones/Portals"},
};