#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QButtonGroup>
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
    void read_PORT();
    void outputMAC();
    void setPC();
    void setForwardingTable();
    void setOutputWindow();

    //确定PC是直连哪个网桥的
    int which_bridge(int pc_id);
    //网桥的地址学习及泛洪
    void learning();
    void DrawSend(int send_pc, int recv_pc, int isreply);
private:
    Ui::MainWindow *ui;
    ifstream ifile;
    string PC_MAC[7];
    int PC_port[7];
    int Bridge_port[2];
    ForwardingTable* FT1;
    ForwardingTable* FT2;

    QButtonGroup PC_button;

    //cur_click指当前的点击为sender or receiver
    int cur_click = 0;
    int send_pc = 0;
    int recv_pc = 0;
    QTimer *m_timer;
    int delayTime;
private slots:
    void ButtonClick();
    void delife();
    void on_OK_clicked();
    void Send();
};
#endif // MAINWINDOW_H
