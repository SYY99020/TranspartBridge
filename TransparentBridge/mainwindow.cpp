#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"
#include <QTimer>
#include <QTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    int Height = read_config("Global","Height");
    int Width = read_config("Global","Width");
    this->setFixedSize(Width,Height);
    this->delayTime = read_config("TimeDelay","Time");
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
    //btn->setStyleSheet("QPushButton{background-color:rgb(255,255,0)}");
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
        btn->setStyleSheet("QPushButton{background-color:rgb(221, 1, 5)}");
        cur_click++;
    }
    else if(cur_click == 1){
        recv_pc = PC_id;
        btn->setStyleSheet("QPushButton{background-color:rgb(87, 95, 255)}");
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
}

void MainWindow::learning(){
    int lifetime = FT1->life_time;
    qDebug() << lifetime;

    if(send_pc && recv_pc)
    {
        qDebug() << "send_pc " << send_pc << "recv_pc " << recv_pc;
        //发包
        qDebug() << "send frame" ;
        DrawSend(send_pc,recv_pc,0);
        Send();
        string s = "PC" + to_string(send_pc) + " MAC:";
        s += PC_MAC[send_pc];
        s += " send: Hello\n";
        output += s;
        ui->OutputLabel->setText(QString::fromStdString(output));

        int tmp = recv_pc;
        recv_pc = send_pc;
        send_pc = tmp;

        //回包延时
        QTime _timer = QTime::currentTime().addMSecs(delayTime);
        while (QTime::currentTime() < _timer) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        for (int id = 0; id <= 6; id++){
            ui->paintlabel->chosen[id] = 0;
        }
        //回包
        Send();
        DrawSend(send_pc,recv_pc,1);
        string r = "PC" + to_string(send_pc) + " MAC:";
        r += PC_MAC[send_pc];
        r += " reply: Recved\n";
        output += r;
        ui->OutputLabel->setText(QString::fromStdString(output));


        //延时清空
        QTime _timer2 = QTime::currentTime().addMSecs(delayTime);
        while (QTime::currentTime() < _timer2) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        for (int id = 0; id <= 6; id++){
            ui->paintlabel->chosen[id] = 0;
        }
        output = "";
        ui->OutputLabel->setText(QString::fromStdString(output));
        ui->PC1->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
        ui->PC2->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
        ui->PC3->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
        ui->PC4->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
        ui->PC5->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
        ui->PC6->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
    }
    else {
        return;
    }
}

void MainWindow::Send(){
    int send_bridge = which_bridge(send_pc);
    int recv_bridge = which_bridge(recv_pc);
    int lifetime = FT1->life_time;
    qDebug() << "send_bridge " << send_bridge << " recv_bridge " << recv_bridge;
    if(send_bridge == recv_bridge)
    {
        //泛洪时另一个转发表也要添加
        if(send_bridge == 1)
        {
            FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),PC_port[send_pc],lifetime);

            //泛洪时另一个转发表也要添加
            if(!FT1->ifexist(recv_pc)){
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),Bridge_port[1],lifetime);
            }
        }
        else if(send_bridge == 2)
        {
            FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),PC_port[send_pc],lifetime);

            //泛洪时另一个转发表也要添加
            if(!FT2->ifexist(recv_pc)){
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),Bridge_port[0],lifetime);
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
            FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),PC_port[send_pc],lifetime);
            FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),Bridge_port[1],lifetime);
        }
            //网桥2给网桥1发
        else if (send_bridge == 2) {
            //发包的时候学习
            FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),PC_port[send_pc],lifetime);
            FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),Bridge_port[0],lifetime);
        }
    }
}
void MainWindow::DrawSend(int send_pc, int recv_pc, int isreply){
    //发送方
    ui->paintlabel->chosen[send_pc] = isreply + 1;
    //接收方
    ui->paintlabel->chosen[recv_pc] = isreply + 1;
    //网桥中间通信
    if (which_bridge(send_pc) != which_bridge(recv_pc)){
        ui->paintlabel->chosen[0] = isreply + 1;
        if (!FT1->ifexist(recv_pc)){
            //泛洪
            for (int id = 1; id <= 3; id++){
                ui->paintlabel->chosen[id] = isreply + 1;
            }
        }
        if (!FT2->ifexist(recv_pc)){
            //泛洪
            ui->paintlabel->chosen[0] = isreply + 1;
            for (int id = 4; id <= 6; id++){
                ui->paintlabel->chosen[id] = isreply + 1;
            }
        }
    }
    else{
        if (which_bridge(send_pc) == 1){
            if (!FT1->ifexist(recv_pc)){
                //泛洪
                ui->paintlabel->chosen[0] = isreply + 1;
                for (int id = 1; id <= 3; id++){
                    ui->paintlabel->chosen[id] = isreply + 1;
                }
                if (!FT2->ifexist(recv_pc)){
                    for (int id = 4; id <= 6; id++){
                        ui->paintlabel->chosen[id] = isreply + 1;
                    }
                }
            }
        }
        else{
            if (!FT2->ifexist(recv_pc)){
                //泛洪
                ui->paintlabel->chosen[0] = isreply + 1;
                for (int id = 1; id <= 3; id++){
                    ui->paintlabel->chosen[id] = isreply + 1;
                }
                if (!FT1->ifexist(recv_pc)){
                    for (int id = 4; id <= 6; id++){
                        ui->paintlabel->chosen[id] = isreply + 1;
                    }
                }
            }
        }
    }
}
void MainWindow::on_OK_clicked()
{
//    ui->PC1->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
//    ui->PC2->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
//    ui->PC3->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
//    ui->PC4->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
//    ui->PC5->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
//    ui->PC6->setStyleSheet("QPushButton{background-color:rgb(255,255,255)}");
    learning();
}
