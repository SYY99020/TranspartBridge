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

