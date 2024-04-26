//
// Created by melissaa on 2022-05-07.
//

#ifndef MULTIED_LOGWINDOW_H
#define MULTIED_LOGWINDOW_H

class QString;
class FString;
class QDialog;
class QPlainTextEdit;
class QLineEdit;
namespace Components {

    class qtLogWindow /*: public QDialog*/ {
    public:
        qtLogWindow();

        void Log(const QString& qString);
        void Log(const FString& fString);

        void Show();
        void Hide();

    public:
        void Send();

    private:
        QDialog* m_pWindow;
        QPlainTextEdit* m_pLogView;
        QLineEdit* m_pConsoleEntry;
    };

}

#endif //MULTIED_LOGWINDOW_H
