//
// Created by melissaa on 20/04/24.
//
#include "HelpMenu.h"
using namespace Components;

void HelpMenu::Init() {
    auto pMenu = new QMenu("&Help");

    CreateAction(pMenu, "&MultiEd Help", [=](){ this->LaunchGitWiki(); }, "F1");
    CreateAction(pMenu, "&MultiEd Github Page", [=](){ this->LaunchGitRepo(); });
    pMenu->addSeparator();
    CreateAction(pMenu, "&Technical Support", [=](){ this->LaunchOldUnrealForum(); });
    CreateAction(pMenu, "Old&unreal UEd2.2 Forum", [=](){ this->LaunchOldUnrealUEDForum(); }, "F2");
    CreateAction(pMenu, "&Epic Games Web Site", [=](){ this->LaunchEpicGames(); });
    CreateAction(pMenu, "&Oldunreal Website", [=](){ this->LaunchOldUnreal(); });

    pMenu->addSeparator();
    CreateAction(pMenu, "&About MultiEd", [=](){ this->About(); });

    // Maybe don't have this here?
    auto pMenuBar = m_pWindow->menuBar();
    pMenuBar->addMenu(pMenu);
}

void HelpMenu::LaunchOldUnrealForum() {
    const QString sURL = "https://www.oldunreal.com/cgi-bin/yabb2/YaBB.pl";
    QDesktopServices::openUrl(sURL);
}

void HelpMenu::LaunchOldUnrealUEDForum() {
    const QString sURL = "https://www.oldunreal.com/cgi-bin/yabb2/YaBB.pl?board=UnrealEDTwoPointOne";
    QDesktopServices::openUrl(sURL);
}

void HelpMenu::LaunchEpicGames() {
    const QString sURL = "www.epicgames.com";
    QDesktopServices::openUrl(sURL);
}

void HelpMenu::LaunchOldUnreal() {
    const QString sURL = "https://www.oldunreal.com/";
    QDesktopServices::openUrl(sURL);
}

void HelpMenu::LaunchGitRepo() {
    const QString sURL = "https://github.com/MelissaAutumn/MultiEd";
    QDesktopServices::openUrl(sURL);
}

void HelpMenu::LaunchGitWiki() {
    const QString sURL = "https://github.com/MelissaAutumn/MultiEd/wiki";
    QDesktopServices::openUrl(sURL);

}

void HelpMenu::About() {
    QMessageBox::about(m_pWindow, "About MultiEd", "MultiEd is based off of UnrealEd 2.2.\nBuilt by Melissa A.\n\nOriginal UnrealEd version copyright 2000, Epic Games Inc.\nUpdated to UnrealEd 2.2 by www.oldunreal.com.");
}
