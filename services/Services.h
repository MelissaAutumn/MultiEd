//
// Created by melissaa on 2022-05-01.
//

#ifndef MULTIED_SERVICES_H
#define MULTIED_SERVICES_H

#include "HelperAPI.h"
#include "EditorAPI.h"

static Services::HelperAPI* g_pHelperAPI = new Services::HelperAPI();
static Services::EditorAPI* g_pEditorAPI = new Services::EditorAPI();

#endif //MULTIED_SERVICES_H
