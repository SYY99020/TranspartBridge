#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"

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
    setForwardingTable();
    setOutputWindow();
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

}

void MainWindow::setForwardingTable(){
    int Height = read_config("Forwarding_TABLE","Height");
    int Width = read_config("Forwarding_TABLE","Width");
    int x_pos_1 = read_config("Forwarding_TABLE","X_Pos_1");
    int x_pos_2 = read_config("Forwarding_TABLE","X_Pos_2");
    int y_pos = read_config("Forwarding_TABLE","Y_Pos");
    int Row_Height = read_config("Forwarding_TABLE","RowHeight");
    int font_size = read_config("Label","FontSize");
    int lifespan = read_config("Forwarding_TABLE","LifeSpan");
    QFont font;
    font.setPointSize(font_size);//字体大小
    ui->Bridge1Word->setFont(font);
    ui->Bridge2Word->setFont(font);
    int table_font = read_config("Forwarding_TABLE","FontSize");
    QFont font2;
    font2.setPointSize(table_font);//字体大小

    ui->Bridge1Word->setGeometry(x_pos_1,y_pos,150,20);
    ui->Bridge2Word->setGeometry(x_pos_2,y_pos,150,20);
    FT1 = new ForwardingTable(this,Height-30,Width,Row_Height,lifespan);
    FT1->setGeometry(x_pos_1,y_pos+30,Width,Height-30);
    FT1->setFont(font2);
    FT2 = new ForwardingTable(this,Height-30,Width,Row_Height,lifespan);
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
