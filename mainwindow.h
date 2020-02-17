#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addItem(QString appname,QString fatherappname,unsigned long mypid,unsigned long fatherpid,QString myfullpara);
public slots:
    void receiveContent(QString appname,QString fatherappname,unsigned long mypid,unsigned long fatherpid,QString myfullpara);
    void on_activatedTrayicon(QSystemTrayIcon::ActivationReason reason);
private slots:
    void on_hideButton_clicked();
    void on_actionAuthor_triggered();
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayicon;
};

#endif // MAINWINDOW_H
