#include "header.h"
#include <QDebug>
int read_config(){
    ifstream ifile(CONFIG_PATH, ios::in);
    if (!ifile){
        qDebug() << "打开文件失败！";
        exit(-1);
    }

}
