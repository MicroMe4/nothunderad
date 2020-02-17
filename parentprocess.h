#ifndef PARENTPROCESS_H
#define PARENTPROCESS_H
#include <QObject>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

class ParentProcess:public QObject
{
    Q_OBJECT
public:
    explicit ParentProcess(QObject *parent = nullptr,int argc = 0,char *argv[] = nullptr);
    DWORD getParentPid();
    QString getParentName();
    QStringList getMyParameters();
    QString getMyFullParameter();
    QString getThisAppName();
    DWORD getMyPID();
    enum PACKAGEPROCESS{PACAKGE_V1,PACKAGE_V2};
    enum PackageInfomation{PACKAGE_MYPID,PACKAGE_ARGV,PACKAGE_PARENTAPPNAME,PACKAGE_MYAPPNAME,PACKAGE_PARENTPID};
    QByteArray toArray(PackageInfomation info);
    void refresh();
private:
    void init();
    int argc;
    char **argv;
    PROCESSENTRY32 midvalfather;
    PROCESSENTRY32 midvalme;
    bool isCached;
};

#endif // PARENTPROCESS_H
