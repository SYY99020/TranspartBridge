#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int Height = read_config("Global","Height");
    int Width = read_config("Global","Width");
    this->setFixedSize(Width,Height);
    ui->setupUi(this);
    ui->MACTable->setGeometry(0,0,Width,Height);
    read_MAC();    
    read_PORT();
    outputMAC();
    setPC();
    connect(ui->PC1, SIGNAL(clicked()), this, SLOT(ButtonClick()));
    connect(ui->PC2, SIGNAL(clicked()), this, SLOT(ButtonClick()));
    connect(ui->PC3, SIGNAL(clicked()), this, SLOT(ButtonClick()));
    connect(ui->PC4, SIGNAL(clicked()), this, SLOT(ButtonClick()));
    connect(ui->PC5, SIGNAL(clicked()), this, SLOT(ButtonClick()));
    connect(ui->PC6, SIGNAL(clicked()), this, SLOT(ButtonClick()));
    setForwardingTable();
    setOutputWindow();
    m_timer = new QTimer(this);
//    m2_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(delife()));
    m_timer->start(FT1->delete_time*1000); //every 1 minutes ，每分钟调用一次
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClick()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int PC_id;
    if("PC1" == btn->objectName()){
        PC_id = 1;
    }
    else if("PC2" == btn->objectName()){
        PC_id = 2;
    }
    else if("PC3" == btn->objectName()){
        PC_id = 3;
    }
    else if("PC4" == btn->objectName()){
        PC_id = 4;
    }
    else if("PC5" == btn->objectName()){
        PC_id = 5;
    }
    else if("PC6" == btn->objectName()){
        PC_id = 6;
    }
    else {
        exit(-1);
    }

    if(cur_click == 0)
    {
        send_pc = PC_id;
        cur_click++;
    }
    else if(cur_click == 1){
        recv_pc = PC_id;
        cur_click--;
    }
    else {
        exit(-1);
    }

    //恢复前一个send/recv PC的格式，再给当前选中的PC上格式
    qDebug() << "send" << send_pc;
    qDebug() << "recv" << recv_pc;
}

int MainWindow::which_bridge(int pc_id){
    if(pc_id == 1 || pc_id == 2 || pc_id == 3)
        return 1;
    else if(pc_id == 4 || pc_id == 5 || pc_id == 6)
        return 2;
    else {
        exit(-1);
    }
}

void MainWindow::delife(){
//    qDebug()  << "FT1--------------";
    FT1->delife();
//    qDebug()  << "FT2--------------";
    FT2->delife();
    m_timer->start(FT1->delete_time*1000); //every 1 minutes ，每分钟调用一次
}

void MainWindow::learning(){
    int lifetime = FT1->life_time;
    qDebug() << lifetime;
    if(send_pc && recv_pc)
    {
        int port = 1;
        int send_port_B1, send_port_B2, recv_port_B1, recv_port_B2;
        int send_bridge = which_bridge(send_pc);
        int recv_bridge = which_bridge(recv_pc);
        //自发自收，recv_pc已在转发表中则不泛洪，否则泛洪
        if(send_bridge == recv_bridge)
        {
            //泛洪时另一个转发表也要添加
            if(send_bridge == 1)
            {
                send_port_B1 = PC_port[send_pc];
                recv_port_B1 = PC_port[recv_pc];
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B1,lifetime);
                FT1->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),recv_port_B1,lifetime);

                //泛洪时另一个转发表也要添加
                if(!FT1->ifexist(recv_pc)){
                    send_port_B2 = Bridge_port[1];
                    FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B2,lifetime);
                }
            }
            else if(send_bridge == 2)
            {
                send_port_B2 = PC_port[send_pc];
                recv_port_B2 = PC_port[recv_pc];
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B2,lifetime);
                FT2->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),recv_port_B2,lifetime);

                //泛洪时另一个转发表也要添加
                if(!FT2->ifexist(recv_pc)){
                    send_port_B1 = Bridge_port[0];
                    FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B1,lifetime);
                }
            }
            else {
                exit(-1);
            }
        }
        else {
            //网桥1给网桥2发
            if(send_bridge == 1)
            {
                //发包的时候学习
                send_port_B1 = PC_port[send_pc];
                send_port_B2 = Bridge_port[1];
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B1,lifetime);
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B2,lifetime);

                //回包的时候学习
                recv_port_B2 = PC_port[recv_pc];
                recv_port_B1 = Bridge_port[0];
                FT2->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),recv_port_B2,lifetime);
                FT1->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),recv_port_B1,lifetime);
            }
                //网桥2给网桥1发
            else if (send_bridge == 2) {
                //发包的时候学习
                send_port_B2 = PC_port[send_pc];
                send_port_B1 = Bridge_port[0];
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B2,lifetime);
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),send_port_B1,lifetime);

                //回包的时候学习
                recv_port_B1 = PC_port[recv_pc];
                recv_port_B2 = Bridge_port[1];
                FT1->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),recv_port_B1,lifetime);
                FT2->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),recv_port_B2,lifetime);
            }
        }
    }
    else {
        return;
    }
}

void MainWindow::on_OK_clicked()
{
    learning();
}
