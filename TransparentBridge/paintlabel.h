#ifndef PAINTLABEL_H
#define PAINTLABEL_H

#include <QLabel>
#include <QPainter>

class PaintLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PaintLabel(QWidget *parent);
    void Init(int x_pos_1, int x_pos_2, int y_pos, int height, int width);
    //连线
    int line = 0;   //线的数目
    QPointF beginpos[100],endpos[100];
    int line_num[100];    // 记录端口之间线的序号 n代指PCn所连线，0表示网桥之间的连线

signals:
    void cursorchanged(QPaintEvent *event);

protected:
    QPainter painter;
    //绘图事件
    void paintEvent(QPaintEvent *);
    int Bridge_x_pos[2];
    int Bridge_y_pos;
    int Bridge_width;
    int Bridge_height;
};

#endif // PAINTLABEL_H
