#ifndef MAINWIN_H
#define MAINWIN_H

#include <qmlined.h>
#include <qlistbox.h>
#include <qlined.h>
#include <qlist.h>

#include <ktopwidget.h>
#include <kprocess.h>

class KShellWin: public KTopLevelWidget
{
    Q_OBJECT
public:
    KShellWin(int argc, char **argv);
    ~KShellWin();
private:
    QListBox *output;
    QListBox *history;
    QLineEdit *input;
    KProcess *proc;
    QList <int> commandPos;
private slots:
    void commandEntered();
    void receivedData(KProcess *proc, char *buffer,int buflen);
    void jumpToCommand(int line);
};

#endif