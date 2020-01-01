#include "forwardingtable.h"
#include "ui_forwardingtable.h"
#include <QDebug>
ForwardingTable::ForwardingTable(QWidget *parent,int height,int width,int rowheight,int lifetime, int deletetime) :
    QWidget(parent),
    ui(new Ui::ForwardingTable)
{
    ui->setupUi(this);
    life_time = lifetime;
    delete_time = deletetime;
    int colwidth1 = width / 6;
    int colwidth3 = width / 6;
    int colwidth4 = width / 5;
    int colwidth2 = width - colwidth1 - colwidth3 - colwidth4 - 10;
    ui->tableWidget->setGeometry(0,0,width,height);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0, colwidth1);  //1 设置列宽
    ui->tableWidget->setColumnWidth(1, colwidth2);  //2 设置列宽
    ui->tableWidget->setColumnWidth(2, colwidth3);  //3 设置列宽
    ui->tableWidget->setColumnWidth(3, colwidth4);  //4 设置列宽
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget->setRowHeight(0,rowheight);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"PC id"<<"MAC"<<"port"<<"life");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
}

ForwardingTable::~ForwardingTable()
{    
    delete ui;
}

int ForwardingTable::insert(int id, QString mac, int port, int life){
    ForwardingTableItem* p;
    ForwardingTableItem* s = new ForwardingTableItem(id,mac,port,life);// = nullptr;
    int i = 0;
    for(p = table; p; p = p->next){

        //转发表中已经存在，则更新为最新的life
        if(!QString::compare(p->MAC,mac))
        {
            p->LIFE = life;/*
            qDebug() <<"update";
            qDebug() << "i-1" << i-1;*/
            ui->tableWidget->setItem(i-1,3,new QTableWidgetItem(QString::number(life)));
            return i;
        }
        if(!p->next)
            break;
        i++;
    }
    p->next = s;

    qDebug() << s->ID;
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行
    ui->tableWidget->setRowHeight(RowCont,15);
    ui->tableWidget->setItem(RowCont,0,new QTableWidgetItem(QString::number(id)));
    ui->tableWidget->setItem(RowCont,1,new QTableWidgetItem(mac));
    ui->tableWidget->setItem(RowCont,2,new QTableWidgetItem(QString::number(port)));
    ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem(QString::number(life)));

    return i+1;
}

void ForwardingTable::delife(){
    ForwardingTableItem* p;
    ForwardingTableItem* father;
    for(p = table; p;){
        if(p->LIFE == 1)
        {
//            qDebug() << "table life";
            father->next=p->next;
            p->next=nullptr;
            delete p;
//            qDebug() << "table life1";
            p=father->next;
            continue;
        }
        else {
            p->LIFE-= delete_time;
            father = p;
            p = p->next;
        }
    }
    int RowCont = ui->tableWidget->rowCount();
    for(int i=RowCont-1; i >= 0; i--)
    {
        int life = ui->tableWidget->item(i, 3)->text().toInt();
        if(life == 1){
            ui->tableWidget->removeRow(i);
        }
        else {
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(life-delete_time)));
        }
    }
}

bool ForwardingTable::ifexist(int pc_id){
    ForwardingTableItem* p;
    for(p = table; p; p = p->next){
        if(p->ID == pc_id)
            return true;
    }
    return false;
}
