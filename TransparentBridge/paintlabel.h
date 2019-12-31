#ifndef PAINTLABEL_H
#define PAINTLABEL_H

#include <QLabel>
#include <QPainter>

class PaintLabel : public QLabel
{
    Q_OBJECT
public:
    int num = 1,line = 0;   //点的序号&线的数目
    int line_a[100],line_b[100];    //连线序号
    int line_w[100];        //线的权值
    explicit PaintLabel(QWidget *parent);
    void Init(int x_pos_1, int x_pos_2, int y_pos, int height, int width);
    //生成树
    QRectF rectbutton[100];
    QPoint cpos,wpos[100];
    QString Text[100];
    QString weigh[100];
    QPointF beginpos[100],endpos[100];

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
