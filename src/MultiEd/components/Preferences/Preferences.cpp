//
// Created by melissaa on 23/05/22.
//
#include <QDebug>
#include <QString>
//#include <Core.h>
#include "Preferences.h"
#include "../../helpers/UnrealHelper.h"

namespace Components {
    Preferences::Preferences() {
        LogTree(LocalizeGeneral("AdvancedOptionsTitle",TEXT("Window")), 0);
    }

    void Preferences::LogTree(const TCHAR* szCaption, int depth) {
        TArray<struct FPreferencesInfo> aPrefs;
        UObject::GetPreferences( aPrefs, szCaption, 0 );

        for(int i = 0; i < aPrefs.Num(); i++) {
            auto caption = Helpers::GetQStringFromFString(aPrefs(i).Caption);
            qDebug() << depth << caption;
            LogTree(*aPrefs(i).Caption, depth + 1);
        }
    }

} // Components