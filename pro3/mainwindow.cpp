#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlRecord>
#include <QModelIndex>
#include <QDebug>
#include <QPalette>
#include <QWidget>

QString buyername;//保存进货员登录时的用户名
QString checkername; //保存审核员登录时的用户名
QString sellername; //保存出货员登录的用户名

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newWelcome() //用户登录选择
{
    int userType = ui->cbUserType->currentIndex(); //获取登录类型，进货员为0，出货员为1，审核员为2
    QString newComerNumber,newComerPasswd;
    newComerNumber = ui->leUser->text(); //获取输入的用户名和密码
    newComerPasswd = ui->lePwd->text();

    int ok = (newComerNumber.isEmpty())||(newComerPasswd.isEmpty());
    if(ok)
    {
          QMessageBox::critical(this,tr("错误!"),tr("用户名或密码为空!"));
    }
    else
    {
        QSqlQuery query;
        switch(userType)
        {
        case 0:
            query.exec("select * from employee where empid='"+newComerNumber+"' and emppsd='"+newComerPasswd+"' and emppost='0'");
            if(query.next())
            {
                buyername=newComerNumber;//记录登录进货员用户名
                //welcomebuyUi.Welcomebuy_load();//显示进货员界面
                welcomebuyUi.show();
                //this->hide();//隐藏主界面
            }
            else
            {
                QMessageBox::warning(this , tr("进货员登录失败"),tr("用户名或密码错误!"));
            }
            break;
        case 1:
            query.exec("select * from employee where empid='"+newComerNumber+"' and emppsd='"+newComerPasswd+"' and emppost='1'");  //有无此号
            if(query.next())
            {
                sellername = newComerNumber; //记录登录出货员用户名
                welcomesellUi.show();
                //this->hide();
            }
            else //登录失败
                QMessageBox::warning(this, tr("出货员登录失败"),tr("用户名或密码错误！"));
            break;
        case 2:
            query.exec("select * from employee where empid='"+newComerNumber+"' and emppsd='"+newComerPasswd+"' and emppost='2'");  //有无此号
            if(query.next())
            {
                checkername = newComerPasswd; //记录登录管理员用户名
                welcomecheckUi.show();//显示审核员界面
                //this->hide(); //隐藏主界面
            }
            else //登录失败
              QMessageBox::warning(this, tr("审核员登录失败"),tr("用户名或密码错误！"));
            break;
        default:
            QMessageBox::warning(this, tr("错误"),tr("无此用户！"));
        };
    }

    /*清空输入内容*/
    ui->leUser->clear();
    ui->lePwd->clear();
    ui->leUser->setFocus();
}


