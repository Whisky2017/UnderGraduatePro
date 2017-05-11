#include "welcomecheck.h"
#include "ui_welcomecheck.h"
#include <QSqlRecord>
#include <QMessageBox>

welcomecheck::welcomecheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::welcomecheck)
{
    ui->setupUi(this);
    GoodsModel = new QSqlRelationalTableModel(this);
    GoodsModel->setTable("goodsinfo"); //绑定货物表
    GoodsModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
    //GoodsModel->setRelation(5,QSqlRelation("card","scardNumber","money")); //通过scardNumber将card表中的money与Student表绑定
    GoodsModel->select(); //执行select()函数更新视图信息
    ui->tvGoods->setModel(GoodsModel); //表与视图绑定

    ListModel = new QSqlTableModel(this);


    connect(ui->pbModify, SIGNAL(clicked()),this,SLOT(pbModify_clicked())); //提交更改信息
    connect(ui->pbSearchGoods, SIGNAL(clicked()),this,SLOT(pbSearchGoods_clicked()));
    connect(ui->pbSearchList, SIGNAL(clicked()),this,SLOT(pbSearchList_clicked()));
}

void welcomecheck::submit() //提交事物处理
{
    ListModel->database().transaction(); //开始事务操作
    if (ListModel->submitAll())
    {
        ListModel->database().commit(); //提交
        QMessageBox::about(this, tr("操作outlist表"),tr("提交成功!"));
    }
    else
    {
        ListModel->database().rollback(); //回滚
        QMessageBox::warning(this, tr("GoodsModel"),tr("数据库错误: %1").arg(ListModel->lastError().text()));
    }
}

void welcomecheck::pbModify_clicked() //更改出货单信息
{
    submit();
}

void welcomecheck::pbSearchGoods_clicked() //根据货物名模糊匹配搜索货物信息
{
    QString gdname;
    gdname = ui->leSearchname->text();
    if(gdname.isEmpty()) //如果输入信息为空，则选择所有货物信息
    {
        GoodsModel->setFilter(QObject::tr("gdname like '%1'").arg("%"));
        GoodsModel->select();
    }
    else
    {
        GoodsModel->setFilter(QObject::tr("gdname like '%1'").arg((QString)("%")+gdname+(QString)("%"))); //根据货物名进行筛选
        GoodsModel->select(); //显示结果
    }
}

void welcomecheck::pbSearchList_clicked()
{
    QString op = ui->cbDeal->currentText();
    int type = ui->cbListType->currentIndex();

    if(type == 0)
    {
        ListModel->setTable("outlist"); //绑定表
        ListModel->setEditStrategy(QSqlTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
        ListModel->select(); //执行select()函数更新视图信息
        ui->tvlist->setModel(ListModel); //表与视图绑定
    }
    else
    {
        ListModel->setTable("inlist"); //绑定表
        ListModel->setEditStrategy(QSqlTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
        ListModel->select(); //执行select()函数更新视图信息
        ui->tvlist->setModel(ListModel); //表与视图绑定
    }

    ListModel->setFilter(QObject::tr("deal like '%1'").arg(("%")+op+("%")));
    ListModel->select();


}

welcomecheck::~welcomecheck()
{
    delete ui;
}
