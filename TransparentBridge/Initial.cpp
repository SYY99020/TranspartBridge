#include "header.h"

int read_config(){
    ifstream ifile(CONFIG_PATH, ios::in);
    if (!ifile){
        qDebug() << "打开文件失败！" << endl;
        exit(-1);
    }

}
