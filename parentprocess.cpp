#include "parentprocess.h"
#include <QMessageBox>

ParentProcess::ParentProcess(QObject *parent, int _argc, char *_argv[]) : QObject (parent)
{
    this->argc = _argc;
    this->argv = _argv;
    ZeroMemory(&this->midvalfather, sizeof(this->midvalfather));
    ZeroMemory(&this->midvalme,sizeof (this->midvalme));
    this->isCached = false;
}

DWORD ParentProcess::getParentPid(){
    if(!this->isCached){
        this->init();
    }

    return this->midvalfather.th32ProcessID;
}

QString ParentProcess::getParentName(){
    if(this->isCached){
        this->init();
    }
    return QString::fromWCharArray(this->midvalfather.szExeFile);
}

QString ParentProcess::getThisAppName(){
    if(this->isCached){
        this->init();
    }
    return QString::fromWCharArray(this->midvalme.szExeFile);
}

void ParentProcess::refresh(){
    this->argc = 0;
    this->argv = nullptr;
    ZeroMemory(&this->midvalfather, sizeof(this->midvalfather));
    ZeroMemory(&this->midvalme,sizeof (this->midvalme));
    this->isCached = false;
}

QStringList ParentProcess::getMyParameters(){
    QStringList _list;
    for(int i = 0; i < this->argc ; i++){
        _list << argv[i];
    }
    return _list;
}

QString ParentProcess::getMyFullParameter(){
    QString _para;
    for(int i = 0; i < this->argc ; i++){
        _para.append(argv[1]).append(" ");
    }
    return _para;
}

DWORD ParentProcess::getMyPID(){
    return this->midvalme.th32ProcessID;
}


void ParentProcess::init(){
    if(this->isCached){
        return;
    }
    DWORD pid = GetCurrentProcessId(), ppid = 0;
    HANDLE _handler = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    PROCESSENTRY32 _proentry;
    if (_handler == INVALID_HANDLE_VALUE ){
        int but = QMessageBox::warning(reinterpret_cast<QWidget *>(this->parent()),"Critical Error","We Get An Invalid Handler. Process Cannot Be Continued", QMessageBox::Ok, QMessageBox::NoButton);
        if (but == QMessageBox::Ok || but == QMessageBox::Escape){
            exit(-1);
        }
    }
    ZeroMemory(&_proentry,sizeof(_proentry));
    _proentry.dwSize = sizeof (_proentry);
    if (!Process32First(_handler,&_proentry)){
        QMessageBox::warning(reinterpret_cast<QWidget *>(this->parent()),"Critical Error","We Cannot Start Such An Process Check", QMessageBox::Ok, QMessageBox::NoButton);
        exit(-2);
    }
    do{
        if(_proentry.th32ProcessID == pid){
            this->midvalme = _proentry;
            ppid = _proentry.th32ParentProcessID;
            break;
        }
    } while (Process32Next(_handler,&_proentry));
    ZeroMemory(&_proentry,sizeof(_proentry));
    _proentry.dwSize = sizeof (_proentry);
    if (!Process32First(_handler,&_proentry)){
        QMessageBox::warning(reinterpret_cast<QWidget *>(this->parent()),"Critical Error","We Cannot Start Such An Process Check", QMessageBox::Ok, QMessageBox::NoButton);
        exit(-2);
    }
    do{
        if(_proentry.th32ProcessID == ppid){
            this->midvalfather = _proentry;
            //ppid = _proentry.th32ParentProcessID;
            this->isCached = true;
            break;
        }
    } while (Process32Next(_handler,&_proentry));
    if (_handler != INVALID_HANDLE_VALUE){
        CloseHandle(_handler);
    }
}


QByteArray ParentProcess::toArray(PackageInfomation pkg){
    QByteArray array;
    if(this->isCached == false ){
        this->init();
    }
    if (pkg == PACKAGE_MYPID){
        array.push_back(static_cast<char>(PACKAGE_MYPID));
    }
    else if (pkg == PACKAGE_ARGV){
        array.push_back(static_cast<char>(PACKAGE_ARGV));
    }
    else if (pkg == PACKAGE_MYAPPNAME){
        array.push_back(static_cast<char>(PACKAGE_MYAPPNAME));
    }
    else if (pkg == PACKAGE_PARENTAPPNAME){
        array.push_back(static_cast<char>(PACKAGE_PARENTAPPNAME));
    }
    else if (pkg == PACKAGE_PARENTPID){
        array.push_back(static_cast<char>(PACKAGE_PARENTPID));
    }
    array.push_back(static_cast<char>(0x00));
    array.push_back(static_cast<char>(0x00));
    array.push_back(static_cast<char>(0x00));
    if (pkg == PACKAGE_MYPID){
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0xFF000000));
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0x00FF0000));
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0x0000FF00));
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0x000000FF));
    }
    else if (pkg == PACKAGE_PARENTPID){
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0xFF000000));
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0x00FF0000));
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0x0000FF00));
        array.push_back(static_cast<char>(static_cast<unsigned int>(this->argc)&0x000000FF));
    }
    else if (pkg == PACKAGE_ARGV){
        array.push_back(this->getMyFullParameter().toUtf8());
    }
    else if (pkg == PACKAGE_MYAPPNAME){
        array.push_back(this->getThisAppName().toUtf8());
    }
    else if (pkg == PACKAGE_PARENTAPPNAME){
        array.push_back(this->getParentName().toUtf8());
    }
    return array;
}
