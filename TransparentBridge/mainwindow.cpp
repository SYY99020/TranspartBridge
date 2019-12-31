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
    m_timer->start(delete_time*1000); //every 1 minutes ，每分钟调用一次
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::outputMAC() {
    int Table_width = read_config("MAC_TABLE","Width");
    int Table_height = read_config("MAC_TABLE","Height");
    int x_pos = read_config("MAC_TABLE","X_Pos");
    int y_pos = read_config("MAC_TABLE","Y_Pos");
    int Row_height = read_config("MAC_TABLE","RowHeight");
    int font_size = read_config("MAC_TABLE","FontSize");
    QFont font;
    font.setPointSize(font_size);//字体大小
    int font_size2 = read_config("Label","FontSize");
    QFont font2;
    font2.setPointSize(font_size2);//字体大小
    ui->MACWord->setFont(font2);
    ui->MACWord->setGeometry(x_pos,y_pos,100,50);

    ui->MACTable->setFont(font);
    ui->MACTable->setGeometry(x_pos,y_pos+50,Table_width,Table_height);
    ui->MACTable->setColumnCount(2);    // 两列
    ui->MACTable->setColumnWidth(0, Table_width/3);  //0 设置列宽
    ui->MACTable->setColumnWidth(1, Table_width/3*2); //1 设置列宽
    ui->MACTable->verticalHeader()->hide();
    ui->MACTable->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->MACTable->setHorizontalHeaderLabels(QStringList()<<"PC_id"<<"MAC--address");
    ui->MACTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    for (int id = 1; id <= 6; id++){
        int RowCont = ui->MACTable->rowCount();
        ui->MACTable->insertRow(RowCont);//增加一行
        ui->MACTable->setRowHeight(RowCont,Row_height);
        ui->MACTable->setItem(RowCont,0,new QTableWidgetItem(QString::fromStdString("PC"+to_string(id))));
        ui->MACTable->item(RowCont,0)->setTextAlignment(Qt::AlignHCenter);
        ui->MACTable->setItem(RowCont,1,new QTableWidgetItem(QString::fromStdString(PC_MAC[id])));
        ui->MACTable->item(RowCont,1)->setTextAlignment(Qt::AlignHCenter);
    }
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

    //PC_button.button(PC_id-1)->setStyleSheet("background-color:blue");

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
    QString MAC = "";
    FT1->delife(MAC);
    FT2->delife(MAC);
    m_timer->start(delete_time*1000); //every 1 minutes ，每分钟调用一次
}
void MainWindow::learning(){
    if(send_pc && recv_pc)
    {
        int port = 1;
        int send_bridge = which_bridge(send_pc);
        int recv_bridge = which_bridge(recv_pc);
        //自发自收，recv_pc已在转发表中则不泛洪，否则泛洪
        if(send_bridge == recv_bridge)
        {
            //泛洪时另一个转发表也要添加
            if(send_bridge == 1)
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),port,life_time);
            else if(send_bridge == 2)
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),port,life_time);
            else {
                exit(-1);
            }
        }
        else {
            //网桥1给网桥2发
            if(send_bridge == 1)
            {
                //发包的时候学习
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),port,life_time);
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),port,life_time);
                //回包的时候学习
                FT2->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),port,life_time);
                FT1->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),port,life_time);
            }
            //网桥2给网桥1发
            else if (send_bridge == 2) {
                //发包的时候学习
                FT1->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),port,life_time);
                FT2->insert(send_pc,QString::fromStdString(PC_MAC[send_pc]),port,life_time);
                //回包的时候学习
                FT2->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),port,life_time);
                FT1->insert(recv_pc,QString::fromStdString(PC_MAC[recv_pc]),port,life_time);
            }
        }
    }
    else {
        return;
    }
}

void MainWindow::setPC(){
    int x_pos = read_config("PC_IMG","X_Pos");
    int y_pos = read_config("PC_IMG","Y_Pos");
    int Height = read_config("PC_IMG","Height");
    int Width = read_config("PC_IMG","Width");
    int pc_height = read_config("PC","Height");
    int pc_width = read_config("PC","Width");
    int font_size = read_config("Label","FontSize");
    QFont font;
    font.setPointSize(font_size);//字体大小
    ui->PC1Word->setFont(font);
    ui->PC2Word->setFont(font);
    ui->PC3Word->setFont(font);
    ui->PC4Word->setFont(font);
    ui->PC5Word->setFont(font);
    ui->PC6Word->setFont(font);
    ui->paintlabel->setGeometry(x_pos,y_pos+pc_height+20,Width,Height-2*pc_height-40);
//    ui->paintlabel->setStyleSheet("border:1px solid red;");

    ui->PC1->setGeometry(x_pos,y_pos,pc_width,pc_height);
    ui->PC1Word->setGeometry(x_pos+pc_width/2-15,y_pos+pc_width,30,20);

    ui->PC2->setGeometry(x_pos,y_pos+Height-pc_height-20,pc_width,pc_height);
    ui->PC2Word->setGeometry(x_pos+pc_width/2-15,y_pos+Height-20,30,20);

    ui->PC3->setGeometry(x_pos+Width/2-pc_width/2,y_pos,pc_width,pc_height);
    ui->PC3Word->setGeometry(x_pos+Width/2-15,y_pos+pc_width,30,20);

    ui->PC4->setGeometry(x_pos+Width/2-pc_width/2,y_pos+Height-pc_height-20,pc_width,pc_height);
    ui->PC4Word->setGeometry(x_pos+Width/2-15,y_pos+Height-20,30,20);

    ui->PC5->setGeometry(x_pos+Width-pc_width,y_pos,pc_width,pc_height);
    ui->PC5Word->setGeometry(x_pos+Width-pc_width/2-15,y_pos+pc_height,30,20);

    ui->PC6->setGeometry(x_pos+Width-pc_width,y_pos+Height-pc_height-20,pc_width,pc_height);
    ui->PC6Word->setGeometry(x_pos+Width-pc_width/2-15,y_pos+Height-20,30,20);

    int bridge_height = read_config("Bridge","Height");
    int bridge_width = read_config("Bridge","Width");
    ui->paintlabel->Init(Width/3-bridge_width,Width/2+bridge_width,(Height-2*pc_height-40)/2-bridge_height/2,bridge_height,bridge_width);
}

void MainWindow::setForwardingTable(){
    int Height = read_config("Forwarding_TABLE","Height");
    int Width = read_config("Forwarding_TABLE","Width");
    int x_pos_1 = read_config("Forwarding_TABLE","X_Pos_1");
    int x_pos_2 = read_config("Forwarding_TABLE","X_Pos_2");
    int y_pos = read_config("Forwarding_TABLE","Y_Pos");
    int Row_Height = read_config("Forwarding_TABLE","RowHeight");
    int font_size = read_config("Label","FontSize");
    int lifetime = read_config("Forwarding_TABLE","LifeTime");
    int deletetime = read_config("Forwarding_TABLE","DeleteTime");
    QFont font;
    font.setPointSize(font_size);//字体大小
    ui->Bridge1Word->setFont(font);
    ui->Bridge2Word->setFont(font);
    int table_font = read_config("Forwarding_TABLE","FontSize");
    QFont font2;
    font2.setPointSize(table_font);//字体大小

    ui->Bridge1Word->setGeometry(x_pos_1,y_pos,150,20);
    ui->Bridge2Word->setGeometry(x_pos_2,y_pos,150,20);
    FT1 = new ForwardingTable(this,Height-30,Width,Row_Height,lifetime,deletetime);
    FT1->setGeometry(x_pos_1,y_pos+30,Width,Height-30);
    FT1->setFont(font2);
    FT2 = new ForwardingTable(this,Height-30,Width,Row_Height,lifetime,deletetime);
    FT2->setGeometry(x_pos_2,y_pos+30,Width,Height-30);
    FT2->setFont(font2);
}

void MainWindow::setOutputWindow(){
    int font_size = read_config("Label","FontSize");
    int output_size = read_config("OutputLabel","FontSize");
    QFont font;
    font.setPointSize(font_size);//字体大小
    ui->OutputWord->setFont(font);
    QFont font2;
    font2.setPointSize(output_size);//字体大小
    ui->OutputLabel->setFont(font2);

    int Height = read_config("OutputLabel","Height");
    int Width = read_config("OutputLabel","Width");
    int x_pos = read_config("OutputLabel","X_Pos");
    int y_pos = read_config("OutputLabel","Y_Pos");
    ui->OutputWord->setGeometry(x_pos,y_pos,100,20);
    ui->OutputLabel->setGeometry(x_pos,y_pos+30,Width,Height-20);
//    ui->OutputLabel->setStyleSheet("border:1px solid red;");
}

void MainWindow::on_OK_clicked()
{
    learning();
}
