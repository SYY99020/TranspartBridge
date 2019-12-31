#include "forwardingtable.h"
#include "ui_forwardingtable.h"
#include <QDebug>
ForwardingTable::ForwardingTable(QWidget *parent,int height,int width,int rowheight) :
    QWidget(parent),
    ui(new Ui::ForwardingTable)
{
    ui->setupUi(this);
    int colwidth1 = width / 5;
    int colwidth2
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0, colwidth1);  //1 设置列宽
    ui->tableWidget->setColumnWidth(1, colwidth2);  //2 设置列宽
    ui->tableWidget->setColumnWidth(2, colwidth3);  //3 设置列宽
    ui->tableWidget->setColumnWidth(3, colwidth4);  //4 设置列宽
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"PC id"<<"MAC"<<"port"<<"life-span");
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

//    qDebug() << s->ID << s->MAC << s->LIFE << s->PORT;
//    qDebug() << "insert";
    int i = 0;
//    qDebug() << "insert_1";
    for(p = table; p->next; p = p->next){
        i++;
//        qDebug() << "insert1";
        if(!QString::compare(p->MAC,mac))
            return -1;
//        qDebug() << "insert2";
    }

//    qDebug() << "bianli";
//    qDebug() << "bianli1";
//    qDebug() << "bianli2";
    p->next = s;

//    qDebug() << s->ID;
    int RowCont;
    RowCont=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(RowCont);//增加一行
    ui->tableWidget->setRowHeight(RowCont,15);
    ui->tableWidget->setItem(RowCont,0,new QTableWidgetItem(QString::number(id)));
    ui->tableWidget->setItem(RowCont,1,new QTableWidgetItem(mac));
    ui->tableWidget->setItem(RowCont,2,new QTableWidgetItem(QString::number(port)));
    ui->tableWidget->setItem(RowCont,3,new QTableWidgetItem(QString::number(life)));

//    qDebug() << "i+1" <<i+1;
    return i+1;
}

void ForwardingTable::delife(QString exclude){
    ForwardingTableItem* p;
    for(p = table; p->next; p = p->next){
        if(!QString::compare(p->MAC,exclude))
            continue;
        else {
            p->LIFE--;
        }
    }
    int RowCont = ui->tableWidget->rowCount();
//    qDebug() << "row" <<RowCont;
    for(int i=0; i < RowCont; i++)
    {
//        qDebug() << "i" << i;
        if(!QString::compare(ui->tableWidget->item(i, 1)->text(),exclude)){
            continue;
        }
        else{
//            qDebug() << "life" ;
            int life = ui->tableWidget->item(i, 3)->text().toInt();
//            qDebug() << life;
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(--life)));
        }
    }
}

