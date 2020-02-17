#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include "parentprocess.h"

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = nullptr);
    uint ContentUMyInt();
    uint ContentUParentInt();
    QString ContentStringPara();
    QString ContentStringAppName();
    QString ContentStringParentAppName();
signals:
    void contentSig(QString,QString,uint,uint,QString);
    void Sendconfirmation(bool);
public slots:
    void receiveThing(int instanceId, QByteArray message);
private:
    uint contentUMyInt;
    uint contentUParentInt;
    QString contentStringPara;
    QString contentStringAppName;
    QString contentStringParentAppName;
    bool int_cot,para_cot,app_cot,parent_int_cot,parent_app_cot;
};

#endif // RECEIVER_H
