#ifndef FORWARDINGTABLE_H
#define FORWARDINGTABLE_H

#include <QWidget>

class ForwardingTableItem {
public:
    ForwardingTableItem(){
        next = nullptr;
    }
    ForwardingTableItem(int id, QString mac, int port, int life){
        ID = id;
        MAC = mac;
        PORT = port;
        LIFE = life;
        next = nullptr;
    }

    /* 一条转发表的item包含PC id,MAC地址，端口号，生命周期 */
    int ID;
    QString MAC;
    int PORT;
    int LIFE;
    ForwardingTableItem* next;
};

namespace Ui {
class ForwardingTable;
}

class ForwardingTable : public QWidget
{
    Q_OBJECT

public:
    explicit ForwardingTable(QWidget *parent = nullptr);
    ~ForwardingTable();
    int insert(int id, QString mac, int port, int life);
    void delife(QString exclude);
//  void Update(const Frame *FRAME,const bool src_if_device,const bool dst_if_device);
//  void Inspect();
//  void PrintTable();
//  int rowcount(QString mac);
    //在表格中查找特定item行数
    static const int Table_width = 310;
    static const int Table_height = 200;
//    static const int conn_port = 3;
    static const int frame_time= 1000;
    static const int colwidth1 = 50;
    static const int colwidth2 = 50;
    static const int colwidth3 = 120;
    static const int colwidth4 = 60;
private:
    ForwardingTableItem* table = new ForwardingTableItem();
    Ui::ForwardingTable *ui;
};
#endif // FORWARDINGTABLE_H
