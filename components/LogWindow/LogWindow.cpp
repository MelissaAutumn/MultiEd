//
// Created by melissaa on 2022-05-07.
//

#include <Engine.h>

#ifdef verify
#undef verify
#endif
#ifdef check
#undef check
#endif

#include <QDialog>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QLineEdit>

#include "LogWindow.h"

#include "../../services/Services.h"
#include "../../helpers/UnrealHelper.h"


Components::qtLogWindow::qtLogWindow() {
    m_pWindow = new QDialog();

    auto pLayout = new QVBoxLayout();
    m_pLogView = new QPlainTextEdit();
    m_pLogView->setReadOnly(true);
    m_pConsoleEntry = new QLineEdit();

    // TODO: Localize!
    m_pWindow->setWindowTitle("Log Window");
    m_pWindow->resize(640, 480);

    QDialog::connect(m_pConsoleEntry, &QLineEdit::returnPressed, [=]() { this->Send(); });//&qtLogWindow::Send);

    pLayout->addWidget(m_pLogView);
    pLayout->addWidget(m_pConsoleEntry);

    m_pWindow->setLayout(pLayout);
}

void Components::qtLogWindow::Log(const QString &qString) {
    m_pLogView->appendPlainText(qString);
}

void Components::qtLogWindow::Log(const FString &fString) {
    Log(Helpers::GetQStringFromFString(fString));
}

void Components::qtLogWindow::Show() {
    m_pWindow->show();
}

void Components::qtLogWindow::Hide() {
    m_pWindow->hide();
}

void Components::qtLogWindow::Send() {
        auto text = m_pConsoleEntry->text();
        auto result = g_pEditorAPI->LogExec(text);
        m_pConsoleEntry->clear();

        // Show our command in the window
        Log("> " + text);

        if (!result) {
            Log(QString("Log: Unrecognized command."));
        }
}
