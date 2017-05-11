#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "welcomebuy.h"
#include "ui_welcomebuy.h"
#include "welcomecheck.h"
#include "welcomesell.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    welcomebuy welcomebuyUi; //进货员登录界面
    welcomecheck welcomecheckUi; //审核员登录界面
    welcomesell welcomesellUi; //出货员登录界面

private slots:
    void newWelcome(); //登录处理函数

};

#endif // MAINWINDOW_H
