#include "paintlabel.h"

PaintLabel ::PaintLabel(QWidget *parent):QLabel(parent)
{
    setMouseTracking(true);     //设置鼠标可跟踪
}
//绘图事件
void PaintLabel::paintEvent(QPaintEvent *)
{
    painter.begin(this);
    painter.setPen(QPen(Qt::black,2));   //设置画笔形式
    //作圆点
    for (int i = 1; i < num; i++) {
        painter.drawEllipse(rectbutton[i]);
        painter.drawText(wpos[i].x(),wpos[i].y(),Text[i]);
    }
    //画线
    for (int i = 0; i <= line; i++){
        painter.drawLine(beginpos[i],endpos[i]);
        QPointF temp = (endpos[i]+beginpos[i])/2.0;   //输出位置
        painter.drawText(temp.x(),temp.y(),weigh[i]);
    }
    painter.end();
    update();
}
