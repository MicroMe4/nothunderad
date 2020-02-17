#include "receiver.h"
#include <QDebug>
Receiver::Receiver(QObject *parent) : QObject(parent)
{
    this->int_cot = false;
    this->para_cot = false;
    this->app_cot = false;
    this->parent_int_cot = false;
}

void Receiver::receiveThing(int instanceId, QByteArray message){
    qDebug() << "Instance ID = " << instanceId;
    ParentProcess::PackageInfomation pkg = static_cast<ParentProcess::PackageInfomation>(message.at(0) & 0xFF);
    QByteArray array;
    if(pkg == ParentProcess::PACKAGE_MYPID){
        for(int i = 4; i < message.size(); i++){
            array.push_back(message.at(i));
        }
        this->contentUMyInt = array.toUInt();
        this->int_cot = true;
    }
    else if(pkg == ParentProcess::PACKAGE_ARGV){
        for(int i = 4; i < message.size(); i++){
            array.push_back(message.at(i));
        }
        this->contentStringPara= QString(array.toStdString().c_str());
        this->para_cot = true;
    }
    else if(pkg == ParentProcess::PACKAGE_MYAPPNAME){
        for(int i = 4; i < message.size(); i++){
            array.push_back(message.at(i));
        }
        this->contentStringAppName = QString(array.toStdString().c_str());
        this->app_cot = true;
    }
    else if(pkg == ParentProcess::PACKAGE_PARENTAPPNAME){
        for(int i = 4; i < message.size(); i++){
            array.push_back(message.at(i));
        }
        this->contentStringAppName = QString(array.toStdString().c_str());
        this->parent_app_cot = true;
    }
    else if(pkg == ParentProcess::PACKAGE_PARENTPID){
        for(int i = 4; i < message.size(); i++){
            array.push_back(message.at(i));
        }
        this->contentUMyInt = array.toUInt();
        this->parent_int_cot = true;
    }
    if (int_cot & para_cot & app_cot & parent_int_cot & parent_app_cot){
        emit this->Sendconfirmation(true);
        emit this->contentSig(this->contentStringAppName,this->contentStringParentAppName,this->contentUMyInt,this->contentUParentInt,this->contentStringPara);
        this->para_cot = false;
        this->int_cot = false;
        this->app_cot = false;
        this->parent_app_cot = false;
        this->parent_int_cot = false;
    }
}

uint Receiver::ContentUMyInt(){
    return this->contentUMyInt;
}

uint Receiver::ContentUParentInt(){
    return this->contentUParentInt;
}

QString Receiver::ContentStringPara(){
    return this->contentStringPara;
}

QString Receiver::ContentStringAppName(){
    return this->contentStringAppName;
}

QString Receiver::ContentStringParentAppName(){
    return this->contentStringParentAppName;
}
