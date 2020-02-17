#include "mainwindow.h"
#include <QApplication>
#include "parentprocess.h"
#include "singleapplication.h"
#include "receiver.h"

int main(int argc, char *argv[])
{
    SingleApplication a(argc,argv,true);
    ParentProcess parp(&a,argc,argv);
    Receiver rec;
    MainWindow w;
    if(a.isSecondary()){
        a.sendMessage(parp.toArray(ParentProcess::PACKAGE_MYPID));
        a.sendMessage(parp.toArray(ParentProcess::PACKAGE_ARGV));
        a.sendMessage(parp.toArray(ParentProcess::PACKAGE_MYAPPNAME));
        a.sendMessage(parp.toArray(ParentProcess::PACKAGE_PARENTPID));
        a.sendMessage(parp.toArray(ParentProcess::PACKAGE_PARENTAPPNAME));
        a.exit(0);
    }
    else if(a.isPrimary())
    {
        w.addItem(parp.getThisAppName(),parp.getParentName(),parp.getMyPID(),parp.getParentPid(),parp.getMyFullParameter());
        QObject::connect(&a,&SingleApplication::receivedMessage,&rec,&Receiver::receiveThing);
        QObject::connect(&rec,&Receiver::contentSig,&w,&MainWindow::receiveContent);
    }
    w.show();
    return a.exec();
}
