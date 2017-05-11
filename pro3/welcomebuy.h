#ifndef WELCOMEBUY_H
#define WELCOMEBUY_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QString>
#include "ui_welcomebuy.h"
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class welcomebuy;
}

class welcomebuy : public QDialog
{
    Q_OBJECT

public:
    welcomebuy(QWidget *parent = 0);
    ~welcomebuy();

private slots: //槽函数
    void pbAdd_clicked();
    void pbDelete_clicked();
    void pbModify_clicked();
    void pbSearchGoods_clicked();
    void pbSearchInlist_clicked();


private:
    Ui::welcomebuy *ui;
    QSqlRelationalTableModel *GoodsModel;
    QSqlRelationalTableModel *InlistModel;
    void submit();
};

#endif // WELCOMEBUY_H
