#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cctype>
#include <QStandardItemModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Window);
    //this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowSystemMenuHint);
    this->setWindowFlag(Qt::WindowMinimizeButtonHint);
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0,new QStandardItem(tr("App Name")));
    model->setHorizontalHeaderItem(1,new QStandardItem(tr("Parent App Name")));
    model->setHorizontalHeaderItem(2,new QStandardItem(tr("This App's PID")));
    model->setHorizontalHeaderItem(3,new QStandardItem(tr("Parent App's PID")));
    model->setHorizontalHeaderItem(4,new QStandardItem(tr("This App's Parameters")));
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lcdNumber->setDecMode();
    ui->lcdNumber->show();
    this->trayicon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/detailInfo/title/title.png");
    trayicon->setIcon(icon);
    trayicon->setToolTip(tr("Thunder Update Hider GUI"));
    connect(this->trayicon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedTrayicon(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::addItem(QString _myappname,QString _fatherappname,unsigned long _mypid,unsigned long _fatherpid,QString _myfullpara){
    QStandardItemModel *model = reinterpret_cast<QStandardItemModel *>(ui->tableView->model());
    int target = model->rowCount();
    model->setItem(target,0,new QStandardItem(_myappname));
    model->setItem(target,1, new QStandardItem(_fatherappname));
    model->setItem(target,2,new QStandardItem(QString::number(_mypid)));
    model->setItem(target,3,new QStandardItem(QString::number(_fatherpid)));
    model->setItem(target,4,new QStandardItem(_myfullpara));
    ui->lcdNumber->display(ui->lcdNumber->intValue()+1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_hideButton_clicked()
{
    this->hide();
    trayicon->show();
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(this,"关于作者","作者邮箱：info@zeronet",QMessageBox::Ok,QMessageBox::NoButton);
}

void MainWindow::receiveContent(QString _myappname, QString _fatherappname, unsigned long _mypid, unsigned long _fatherpid,QString _myfullpara){
    this->addItem(_myappname,_fatherappname,_mypid,_fatherpid,_myfullpara);
}

void MainWindow::on_activatedTrayicon(QSystemTrayIcon::ActivationReason reason){
    switch(reason){
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        trayicon->hide();
        break;
    default:
        break;
    }
}
