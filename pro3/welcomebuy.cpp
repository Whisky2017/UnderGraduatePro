#include "welcomebuy.h"
#include <QSqlRecord>

welcomebuy::welcomebuy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::welcomebuy)
{
    ui->setupUi(this);
    InlistModel = new QSqlRelationalTableModel(this);
    InlistModel->setTable("inlist"); //绑定进货单表
    InlistModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
    InlistModel->select(); //执行select()函数更新视图信息
    ui->tvInlist->setModel(InlistModel); //表与视图绑定

    GoodsModel = new QSqlRelationalTableModel(this);
    GoodsModel->setTable("goodsinfo"); //绑定货物表
    GoodsModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit); //OnManualSubmit表明我们要提交修改才能使其生效
    //GoodsModel->setRelation(5,QSqlRelation("card","scardNumber","money")); //通过scardNumber将card表中的money与Student表绑定
    GoodsModel->select(); //执行select()函数更新视图信息
    ui->tvGoods->setModel(GoodsModel); //表与视图绑定

    /*信号与槽函数的连接*/
    connect(ui->pbAdd, SIGNAL(clicked()),this,SLOT(pbAdd_clicked())); //添加进货单
    connect(ui->pbDelete, SIGNAL(clicked()),this,SLOT(pbDelete_clicked())); //删除选中进货单
    connect(ui->pbModify, SIGNAL(clicked()),this,SLOT(pbModify_clicked())); //提交更改信息
    connect(ui->pbSearchGoods, SIGNAL(clicked()),this,SLOT(pbSearchGoods_clicked())); //根据货物名查找
    connect(ui->pbSearchInlist, SIGNAL(clicked()),this,SLOT(pbSearchInlist_clicked())); //根据进货表编号查找
}

void welcomebuy::submit() //提交事物处理
{
    InlistModel->database().transaction(); //开始事务操作
    if (InlistModel->submitAll())
    {
        InlistModel->database().commit(); //提交
        QMessageBox::about(this, tr("操作inlist表"),tr("提交成功!"));
    }
    else
    {
        InlistModel->database().rollback(); //回滚
        QMessageBox::warning(this, tr("InlistModel"),tr("数据库错误: %1").arg(InlistModel->lastError().text()));
    }
}

void welcomebuy::pbAdd_clicked() //增加进货单
{
     int ok=0;
     QString inlistid,goodsid,innum;


     /*获取输入新进货单的信息*/
     inlistid = ui->leInlistNum->text();
     goodsid = ui->leGoodsNum->text();
     innum = ui->leAmount->text();


     /*判断三项输入是否为空*/
     ok = (inlistid.isEmpty())||(goodsid.isEmpty())||(innum.isEmpty());
     if(ok)
     {
         QMessageBox::critical(this,tr("错误!"),tr("有一项为空!"));
     }
     else //不为空增加新进货单
     {
          ok = QMessageBox::warning(this,tr("添加进货单"),tr("你确定添加进货单信息吗？"),QMessageBox::Yes,QMessageBox::No);
          if(ok == QMessageBox::No)
          {
              return;
          }
          else
          {
              QString insertStr="insert into inlist(inlistid,goodsid,innum) values('"+inlistid+"','"+goodsid+"','"+innum+"')";
              QSqlQuery query;
              if(query.exec(insertStr))
              {
                  QMessageBox::about(this, tr("操作inlist表"),tr("提交成功!"));
                  //insertStr="insert into inlist(goodsid,innum) values('"+goodsid+"','"+innum+"')";
                  //if(query.exec(insertStr))
                  //{
                  //    QMessageBox::about(this, tr("操作inlist表"),tr("提交成功!"));
                        InlistModel->select();
                  //}
              }

          }

     }
}

void welcomebuy::pbDelete_clicked() //删除进货单
{
    int curRow = ui->tvInlist->currentIndex().row(); //获取选中的行
    QSqlQuery query;
    QString inlistid,queryStr;

    inlistid=InlistModel->record(curRow).value("inlistid").toString();//获取对应的inlistid值

    InlistModel->removeRow(curRow);  //删除该行
    int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
       InlistModel->revertAll(); //如果不删除，则撤销
    }
    else
    {
        InlistModel->submitAll(); //否则提交，在数据库中删除该行
        queryStr="delete from inlist where inlistid='"+inlistid+"'"; //删除inlistid表中对应的进货单信息
        if(query.exec(queryStr))
        {
            QMessageBox::about(this, tr("删除进货单"),tr("删除成功!"));
            return;
        }
    }
}

void welcomebuy::pbModify_clicked() //更改进货单信息
{
    submit();
}

void welcomebuy::pbSearchGoods_clicked() //根据货物名模糊匹配搜索货物信息
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

void welcomebuy::pbSearchInlist_clicked() //根据进货单编号匹配搜索进货单信息
{
    QString id;
    id = ui->leSearchNum->text();
    if(id.isEmpty()) //如果输入信息为空，则选择所有进货单信息
    {
        InlistModel->setFilter(QObject::tr("inlistid like '%1'").arg("%"));
        InlistModel->select();
    }
    else
    {
        InlistModel->setFilter(QObject::tr("inlistid like '%1'").arg((QString)("%")+id+(QString)("%"))); //根据进货单号进行筛选
        //QMessageBox::about(this, tr("aa"),tr("成功!"));
        InlistModel->select(); //显示结果
    }
}

welcomebuy::~welcomebuy()
{
    delete ui;
}
