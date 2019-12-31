#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "header.h"
#include "forwardingtable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void open_config();
    void close_config();
    int read_config(string group, string item);
    void read_MAC();
    void outputMAC();
    void setPC();
    void setForwardingTable();
    void setOutputWindow();

private:
    Ui::MainWindow *ui;
    ifstream ifile;
    string PC_MAC[7];
    ForwardingTable* FT1;
    ForwardingTable* FT2;
};
#endif // MAINWINDOW_H
