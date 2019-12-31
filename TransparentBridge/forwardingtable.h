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
    explicit ForwardingTable(QWidget *parent,int height,int width,int rowheight,int lifetime,int deletetime);
    ~ForwardingTable();
    int insert(int id, QString mac, int port, int life);
    void delife(QString exclude);
    bool ifexist(int pc_id);
//  void Update(const Frame *FRAME,const bool src_if_device,const bool dst_if_device);
//  void Inspect();
//  void PrintTable();
//  int rowcount(QString mac);
private:
    ForwardingTableItem* table = new ForwardingTableItem();
    Ui::ForwardingTable *ui;
    int life_time;
    int delete_time;
};
#endif // FORWARDINGTABLE_H
