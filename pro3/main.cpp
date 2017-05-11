#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

//连接数据库
bool createConnection()
{
    QSqlDatabase db =QSqlDatabase::addDatabase("QMYSQL"); //连接数据库驱动
    db.setUserName("root");   //用户名
    db.setPassword("");  //密码
    db.setHostName("localhost"); //数据库所在主机的IP，此处数据库存在本地
    db.setDatabaseName("cangku");   //数据库名
    db.setPort(3306);   //端口
    if(!db.open()) //打开数据库
    {
        qDebug()<<"open faile\n"<<db.lastError().driverText()<<"\n";
        return false; //打开失败
    }
    return true; //打开成功
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    if(!createConnection())//连接数据库，连接失败，则退出程序
        return 1;
    MainWindow w;
    w.show();

    return a.exec();
}
