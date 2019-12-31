#include "paintlabel.h"
#include "header.h"

PaintLabel::PaintLabel(QWidget *parent)
    :QLabel(parent)
{
    setMouseTracking(true);     //设置鼠标可跟踪
}

void PaintLabel::Init(int x_pos_1, int x_pos_2, int y_pos, int height, int width){
    Bridge_x_pos[0] = x_pos_1;
    Bridge_x_pos[1] = x_pos_2;
    Bridge_y_pos = y_pos;
    Bridge_width = width;
    Bridge_height = height;
}
//绘图事件
void PaintLabel::paintEvent(QPaintEvent *)
{
    painter.begin(this);
    painter.setPen(QPen(Qt::black,2));   //设置画笔形式
    //作网桥
    QPixmap pix;
    pix.load(BRIDGE_PATH);
    painter.drawPixmap(Bridge_x_pos[0],Bridge_y_pos,Bridge_width,Bridge_height,pix);
    painter.drawPixmap(Bridge_x_pos[1],Bridge_y_pos,Bridge_width,Bridge_height,pix);
    //画线
    for (int i = 0; i <= line; i++){
        painter.drawLine(beginpos[i],endpos[i]);
        QPointF temp = (endpos[i]+beginpos[i])/2.0;   //输出位置
        painter.drawText(temp.x(),temp.y(),weigh[i]);
    }

    painter.end();
    update();
}
