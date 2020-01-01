#include "paintlabel.h"
#include "header.h"

PaintLabel::PaintLabel(QWidget *parent)
    :QLabel(parent)
{
//    QPalette palette(this->palette());
//    palette.setColor(QPalette::Background, Qt::white);
//    setAutoFillBackground(true);
//    setPalette(palette);

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
    painter.setPen(QPen(Qt::gray,1));   //设置画笔形式
    //作网桥
    QPixmap pix;
    pix.load(BRIDGE_PATH);
    painter.drawPixmap(Bridge_x_pos[0],Bridge_y_pos,Bridge_width,Bridge_height,pix);
    painter.drawRect(QRect(Bridge_x_pos[0],Bridge_y_pos,Bridge_width,Bridge_height));
    painter.drawPixmap(Bridge_x_pos[1],Bridge_y_pos,Bridge_width,Bridge_height,pix);
    painter.drawRect(QRect(Bridge_x_pos[1],Bridge_y_pos,Bridge_width,Bridge_height));
    //画线
    for (int i = 0; i <= 6; i++){
        if (chosen[i]){
            painter.setPen(QPen(Qt::red,2));
        }
        else{
            painter.setPen(QPen(Qt::gray,1));
        }
        int next = (i == 0) ? 3 : 2;
        for(int j = line_num[i]; j < line_num[i] + next; j++){
            painter.drawLine(beginpos[j],endpos[j]);
        }
    }
    painter.end();
    update();
}
