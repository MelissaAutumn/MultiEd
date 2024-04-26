//
// Created by melissaa on 2022-05-07.
//
#include <Core.h>
#ifdef verify
#undef verify
#endif
#include <QString>
#include "UnrealHelper.h"



QString Helpers::GetQStringFromFString(FString str) {
    return QString::fromWCharArray((wchar_t *) str.GetCharArray().GetData());
}