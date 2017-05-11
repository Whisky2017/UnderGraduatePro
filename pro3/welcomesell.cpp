#include "welcomesell.h"
#include "ui_welcomesell.h"
#include <QSqlRecord>
welcomesell::welcomesell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::welcomesell)
{
    ui->setupUi(this);
    OutlistModel = new QSqlRelationalTableModel(this);
    OutlistModel->setTable("outlist"); //绑定出货单表
    OutlistModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
    OutlistModel->select(); //执行select()函数更新视图信息
    ui->tvOutlist->setModel(OutlistModel); //表与视图绑定

    GoodsModel = new QSqlRelationalTableModel(this);
    GoodsModel->setTable("goodsinfo"); //绑定货物表
    GoodsModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
    //GoodsModel->setRelation(5,QSqlRelation("card","scardNumber","money")); //通过scardNumber将card表中的money与Student表绑定
    GoodsModel->select(); //执行select()函数更新视图信息
    ui->tvGoods->setModel(GoodsModel); //表与视图绑定

    /*信号与槽函数的连接*/
    connect(ui->pbAdd, SIGNAL(clicked()),this,SLOT(pbAdd_clicked())); //添加出货单
    connect(ui->pbDelete, SIGNAL(clicked()),this,SLOT(pbDelete_clicked())); //删除选中出货单
    connect(ui->pbModify, SIGNAL(clicked()),this,SLOT(pbModify_clicked())); //提交更改信息
    connect(ui->pbSearchGoods, SIGNAL(clicked()),this,SLOT(pbSearchGoods_clicked())); //根据货物名查找
    connect(ui->pbSearchOutlist, SIGNAL(clicked()),this,SLOT(pbSearchOutlist_clicked())); //根据出货表编号查找
}

void welcomesell::submit() //提交事物处理
{
    OutlistModel->database().transaction(); //开始事务操作
    if (OutlistModel->submitAll())
    {
        OutlistModel->database().commit(); //提交
        QMessageBox::about(this, tr("操作outlist表"),tr("提交成功!"));
    }
    else
    {
        OutlistModel->database().rollback(); //回滚
        QMessageBox::warning(this, tr("OutlistModel"),tr("数据库错误: %1").arg(OutlistModel->lastError().text()));
    }
}

void welcomesell::pbAdd_clicked() //增加出货单
{
     int ok=0;
     QString outlistid,goodsid,outnum;


     /*获取输入新出货单的信息*/
     outlistid = ui->leOutlistNum->text();
     goodsid = ui->leGoodsNum->text();
     outnum = ui->leAmount->text();


     /*判断三项输入是否为空*/
     ok = (outlistid.isEmpty())||(goodsid.isEmpty())||(outnum.isEmpty());
     if(ok)
     {
         QMessageBox::critical(this,tr("错误!"),tr("有一项为空!"));
     }
     else //不为空增加新出货单
     {
          ok = QMessageBox::warning(this,tr("添加出货单"),tr("你确定添加出货单信息吗？"),QMessageBox::Yes,QMessageBox::No);
          if(ok == QMessageBox::No)
          {
              return;
          }
          else
          {
              QString insertStr="insert into outlist(outlistid,goodsid,outnum) values('"+outlistid+"','"+goodsid+"','"+outnum+"')";
              QSqlQuery query;
              if(query.exec(insertStr))
              {
                  QMessageBox::about(this, tr("操作outlist表"),tr("提交成功!"));
                  //insertStr="insert into outlist(goodsid,outnum) values('"+goodsid+"','"+outnum+"')";
                  //if(query.exec(insertStr))
                  //{
                  //    QMessageBox::about(this, tr("操作outlist表"),tr("提交成功!"));
                      OutlistModel->select();
                  //}
              }
          }

     }
}

void welcomesell::pbDelete_clicked() //删除出货单
{
    int curRow = ui->tvOutlist->currentIndex().row(); //获取选中的行
    QSqlQuery query;
    QString outlistid,queryStr;

    outlistid=OutlistModel->record(curRow).value("outlistid").toString();//获取对应的outlistid值

    OutlistModel->removeRow(curRow);  //删除该行
    int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
       OutlistModel->revertAll(); //如果不删除，则撤销
    }
    else
    {
        OutlistModel->submitAll(); //否则提交，在数据库中删除该行
        queryStr="delete from outlist where outlistid='"+outlistid+"'"; //删除outlist表中对应的出货表信息
        if(query.exec(queryStr))
            QMessageBox::about(this, tr("删除出货单"),tr("删除成功!"));
    }
}

void welcomesell::pbModify_clicked() //更改出货单信息
{
    submit();
}

void welcomesell::pbSearchGoods_clicked() //根据货物名模糊匹配搜索货物信息
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

void welcomesell::pbSearchOutlist_clicked() //根据进货单编号匹配搜索进货单信息
{
    QString outlistid;
    outlistid = ui->leSearchNum->text();
    if(outlistid.isEmpty()) //如果输入信息为空，则选择所有出货单信息
    {
        OutlistModel->setFilter(QObject::tr("outlistid like '%1'").arg("%"));
        OutlistModel->select();
    }
    else
    {
        OutlistModel->setFilter(QObject::tr("outlistid like '%1'").arg((QString)("%")+outlistid+(QString)("%"))); //根据货物名进行筛选
        OutlistModel->select(); //显示结果
    }
}

welcomesell::~welcomesell()
{
    delete ui;
}
