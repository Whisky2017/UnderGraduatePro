#ifndef WELCOMECHECK_H
#define WELCOMECHECK_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include "ui_welcomecheck.h"

namespace Ui {
class welcomecheck;
}

class welcomecheck : public QDialog
{
    Q_OBJECT

public:
    welcomecheck(QWidget *parent = 0);
    ~welcomecheck();

private:
    Ui::welcomecheck *ui;
    QSqlRelationalTableModel *GoodsModel;
    QSqlTableModel *ListModel;
    void submit();

private slots: //槽函数
    void pbModify_clicked();
    void pbSearchGoods_clicked();
    void pbSearchList_clicked();

};

#endif // WELCOMECHECK_H
