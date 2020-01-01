#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"

void MainWindow::open_config(){
    ifile.open(CONFIG_PATH, ios::in);
    if (!ifile){
        qDebug() << "打开文件" << CONFIG_PATH << "失败!";
        exit(-1);
    }
}

void MainWindow::close_config(){
    ifile.close();
}

void MainWindow::read_MAC(){
    open_config();
    // find group
    while (!ifile.eof()) {
        string st = "";
        ifile >> st;
        string::size_type start, end;
        start = st.find("[");
        if (start == string::npos)
            continue;
        end = st.find("]");
        string key = st.substr(start + 1,end - start - 1);
//        qDebug() << QString::fromStdString(key);
        if (key == "MAC_INFO")
            break;
    }
    if (ifile.eof()){
        close_config();
        qDebug() << "config have no MAC_INFO";
        exit(-1);
    }
    for (int id = 1; id <= 6; id++){
        string item = "PC";
        string num;
        item += to_string(id);
        string st = "";
        ifile >> st;
        string::size_type equal;
        equal = st.find("=");
        if (equal == string::npos){
            close_config();
            qDebug() << "config error";
            exit(-1);
        }
        string key = st.substr(0,equal);
        string value = st.substr(equal+1, st.length() - equal - 2);
//        qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
        if (key == item){
            PC_MAC[id] = value;
//            qDebug() << id << QString::fromStdString(value);
        }
    }
    close_config();
    return;
}

int MainWindow::read_config(string group, string item){
    open_config();
    // find group
    while (!ifile.eof()) {
        string st = "";
        ifile >> st;
        string::size_type start, end;
        start = st.find("[");
        if (start == string::npos)
            continue;
        end = st.find("]");
        string key = st.substr(start + 1,end - start - 1);
//        qDebug() << QString::fromStdString(key);
        if (key == group)
            break;
    }
    if (ifile.eof())
        return -1;
    while (!ifile.eof()) {
        string st = "";
        ifile >> st;
        string::size_type equal;
        equal = st.find("=");
        if (equal == string::npos)
            break;
        string key = st.substr(0,equal);
        string value = st.substr(equal+1, st.length() - equal - 2);
//        qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
        if (key == item){
            close_config();
//            qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
            return atoi(value.c_str());
        }
    }
    close_config();
    return -1;
}

void MainWindow::read_PORT(){
    open_config();
    // find group
    while (!ifile.eof()) {
        string st = "";
        ifile >> st;
        string::size_type start, end;
        start = st.find("[");
        if (start == string::npos)
            continue;
        end = st.find("]");
        string key = st.substr(start + 1,end - start - 1);
//        qDebug() << QString::fromStdString(key);
        if (key == "PORT_INFO")
            break;
    }
    if (ifile.eof()){
        close_config();
        qDebug() << "config have no PORT_INFO";
        exit(-1);
    }
    for (int id = 1; id <= 6; id++){
        string item = "PC";
        string num;
        item += to_string(id);
        string st = "";
        ifile >> st;
        string::size_type equal;
        equal = st.find("=");
        if (equal == string::npos){
            close_config();
            qDebug() << "config error";
            exit(-1);
        }
        string key = st.substr(0,equal);
        string value = st.substr(equal+1, st.length() - equal - 2);
//        qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
        if (key == item){
            PC_port[id] = atoi(value.c_str());
//            qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
        }
    }
    for (int id = 0; id < 2; id++){
        string item = "Bridge";
        string num;
        item += to_string(id+1);
        string st = "";
        ifile >> st;
        string::size_type equal;
        equal = st.find("=");
        if (equal == string::npos){
            close_config();
            qDebug() << "config error";
            exit(-1);
        }
        string key = st.substr(0,equal);
        string value = st.substr(equal+1, st.length() - equal - 2);
//        qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
        if (key == item){
            Bridge_port[id] = atoi(value.c_str());
//            qDebug() << QString::fromStdString(key) << QString::fromStdString(value);
        }
    }
    close_config();
    return;
}
