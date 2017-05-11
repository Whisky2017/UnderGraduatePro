#ifndef WELCOMESELL_H
#define WELCOMESELL_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QString>
#include "ui_welcomesell.h"
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class welcomesell;
}

class welcomesell : public QDialog
{
    Q_OBJECT

public:
    welcomesell(QWidget *parent = 0);
    ~welcomesell();

private:
    Ui::welcomesell *ui;
    QSqlRelationalTableModel *GoodsModel;
    QSqlRelationalTableModel *OutlistModel;
    void submit();
private slots:         //槽函数
    void pbAdd_clicked();
    void pbDelete_clicked();
    void pbModify_clicked();
    void pbSearchGoods_clicked();
    void pbSearchOutlist_clicked();

};


#endif // WELCOMESELL_H
