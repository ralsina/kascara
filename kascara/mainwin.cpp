#include "mainwin.h"
#include "mainwin.moc"

#include <qlayout.h>


#include <kapp.h>
#include <kpanner.h>

bool last_had_eol=true;

KShellWin::~KShellWin()
{
    delete proc;
}

KShellWin::KShellWin(int argc, char **argv):KTopLevelWidget()
{
    KPanner *view=new KPanner(this,"",3);//KPanner::O_HORIZONTAL || KPanner::U_ABSOLUTE);
    
    QVBoxLayout *l0=new QVBoxLayout(view->child0());
    QVBoxLayout *l1=new QVBoxLayout(view->child1());
    
    output=new QListBox (view->child0());
    output->setFont ("courier");

    history=new QListBox (view->child1());
    history->setFont ("courier");

    input=new QLineEdit (view->child1());
    input->setFont ("courier");
    input->setMaximumHeight (20);
    input->setMinimumHeight (20);
    
    output->show();
    history->show();
    input->show();
    
    l0->addWidget(output);
    l1->addWidget(history);
    l1->addWidget(input);
    
    l0->activate();
    l1->activate();
    
    setView (view);
    resize(400,400);
    view->resize(400,400);
    view->show();
    view->setLimits(1,-24);
    view->setAbsSeparator(376);
    show();


    proc=new KProcess();
    proc->setExecutable("ttyize");

    for (int i=0;i<argc;i++)
    {
        debug ("passing arg %s",argv[i]);
        (*proc) <<argv[i];
    }
    

    proc->start (KProcess::NotifyOnExit,KProcess::All);

    QObject::connect(proc,
                     SIGNAL(receivedStdout(KProcess *, char *,int)),
                     this,
                     SLOT(receivedData(KProcess *, char *,int )));
    QObject::connect(proc,
                     SIGNAL(receivedStderr(KProcess *, char *,int)),
                     this,
                     SLOT(receivedData(KProcess *, char *,int )));
    QObject::connect(input,SIGNAL(returnPressed()),
                     this,SLOT(commandEntered()));
    QObject::connect(history,SIGNAL(selected(int)),
                     this,SLOT(jumpToCommand(int)));

    input->setFocus();
}

void KShellWin::commandEntered()
{

    QString t(output->text(output->count()-1));
    t+=input->text();
    output->changeItem(t.data(),output->count()-1);
    output->setCurrentItem(output->count()-1);
    output->setTopItem(output->count()-1);
    int *i=(int *)malloc (sizeof(int));
    i[0]=output->count()-1;
    commandPos.append(i);
    
    history->insertItem(input->text());
    history->setCurrentItem(history->count()-1);
    history->setTopItem(history->count()-1);
    proc->writeStdin(strdup(input->text()),strlen(input->text()));
    proc->writeStdin(strdup("\r"),1);
    input->setText("");
    last_had_eol=true;
}

void KShellWin::receivedData(KProcess *, char *buffer,int buflen)
{
    output->setAutoUpdate(false);
    char *_buffer=new char[buflen+1];
    strncpy(_buffer,buffer,buflen);
    _buffer[buflen]=0;

    bool ends_on_eol=false;
    if (_buffer[buflen-1]=='\n')
        ends_on_eol=true;
    
    char *line;

    line=strtok (_buffer,"\n");

    if (!last_had_eol)
    {
        QString t(output->text(output->count()-1));
        t+=line;
        output->changeItem(t.data(),output->count()-1);
    }
    else
    {
        output->insertItem(line);
    }
    while (1)
    {
        line=strtok (NULL,"\n");
        if (!line)
        {
            break;
        }
        output->insertItem(line);
    }
    delete [] _buffer;
    
    output->setAutoUpdate(true);
    output->repaint();
    output->setCurrentItem(output->count()-1);
    output->setTopItem(output->count()-1);

    last_had_eol=ends_on_eol;
    
}


void KShellWin::jumpToCommand(int line)
{
    history->setCurrentItem(line);
    history->setTopItem(line);
    output->setCurrentItem(*(commandPos.at(line)));
    output->setTopItem(*(commandPos.at(line)));
}

