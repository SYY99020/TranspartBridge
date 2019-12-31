#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    open_config();
}

MainWindow::~MainWindow()
{
    delete ui;
    close_config();
}

void MainWindow::open_config(){
    ifile.open(CONFIG_PATH, ios::in);
    if (!ifile){
        qDebug() << "打开文件失败！" << endl;
        exit(-1);
    }
}

void MainWindow::close_config(){
    ifile.close();
}

string MainWindow::read_MAC(int id){

}

int MainWindow::read_config(string group, string item){

}
