#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "header.h"

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
    string read_MAC(int id);

private:
    Ui::MainWindow *ui;
    ifstream ifile;

};
#endif // MAINWINDOW_H
