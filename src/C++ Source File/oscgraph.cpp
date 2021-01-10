#include "oscgraph.h"
#include <QPainter>
#include <QToolTip>
#include <QDebug>

OSCgraph::OSCgraph(volatile bool *dat, int **ptr, QWidget *parent)
    : QWidget(parent)
{
    data = dat;

    pen1 = new QPen(Qt::black, 1, Qt::SolidLine);
    pen2 = new QPen(Qt::black, 1, Qt::DotLine);
    pen3 = new QPen(Qt::green, 1, Qt::SolidLine);
    brush = new QBrush(QColor(0,255,0, 89));
    font = new QFont("宋体",10);

    cycle = 48;
    iten_len = 7;
    totalNum = 0;
    aind = 0;
    tind = 0;
    ymaxs = 20;

    outer = ptr;
    dptr = outer[0];

    timeList << "0:00" << "0:30" << "1:00" << "1:30" << "2:00" << "2:30" << "3:00" << "3:30"
             << "4:00" << "4:30" << "5:00" << "5:30" << "6:00" << "6:30" << "7:00" << "7:30"
             << "8:00" << "8:30" << "9:00" << "9:30" << "10:00" << "10:30" << "11:00" << "11:30"
             << "12:00" << "12:30" << "13:00" << "13:30" << "14:00" << "14:30" << "15:00" << "15:30"
             << "16:00" << "16:30" << "17:00" << "17:30" << "18:00" << "18:30" << "19:00" << "19:30"
             << "20:00" << "20:30" << "21:00" << "21:30" << "22:00" << "22:30" << "23:00" << "23:30";
    dayList << "08/2/2" << "08/2/3" << "08/2/4" << "08/2/5" << "08/2/6" << "08/2/7" << "08/2/8";
    cycleList[0] = 48;   cycleList[1] = 24;  cycleList[2] = 12;   cycleList[3] = 6;   cycleList[4] = 2;
    cycleList[5] = 1;
}

OSCgraph::~OSCgraph()
{
    delete pen1;
    delete pen2;
    delete pen3;
    delete brush;
    delete font;
}

void OSCgraph::setTotal(int num){
    *data = false;
    totalNum = num;
}

void OSCgraph::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    y_span = _Y_SPAN_;
    x_span = _X_SPAN_;
    T = 6 * x_span, hT = 3 * x_span;
    y_top = 20 * y_span,  x_top = 21 * x_span;
    y_grid = 5 * y_span,  x_grid = 6 * x_span;
    x_hgrid = 3 * x_span;

    xofs = _XOFS_;
    yofs = _YOFS_;

    QPainter painter(this);
    painter.drawRect(0, 0, width()-1, height()-1);

    if(*data){
        int xbx = xofs - 30;
        int ybx = xofs - 80;
        int yby = yofs - 20;

        QRectF xtb(xbx, yofs , 60 ,40);
        QRectF ytb(ybx, yby, 80, 40);
        QRectF dtb(xbx, 5, 60, 40);
        QRectF tolb(xofs + x_top, yofs - y_top / 2 - 30, width() - xofs - x_top, 60);

        painter.setFont(*font);
        painter.setPen(*pen1);

        for(int i = 0; i <= 4; i++){
            ytb.moveTo(ybx, yby - i * y_grid);
            painter.drawText(ytb, Qt::AlignCenter, QString::number(ymaxs * i / 4));
        }
        for(int i = 0; i < 4; i++){
            xtb.moveTo(xbx + i * x_grid, yofs);
            dtb.moveTo(xbx + i * x_grid, 5);
            painter.drawText(xtb, Qt::AlignCenter, timeSpan.at(i));
            painter.drawText(dtb, Qt::AlignCenter, daySpan.at(i));
        }
        QString tmp = QString("Total\nNum:\n") + QString::number(totalNum);
        painter.drawText(tolb, Qt::AlignCenter, tmp);
    }

    painter.setWindow(-_YPADDING_ - _MARGIN_, height()- _XPADDING_ - _MARGIN_ , width(), -height());

    //painter.fillRect(QRectF(-_YPADDING_, -_XPADDING_, width()- 2 * _MARGIN_, height()-2*_MARGIN_), Qt::white);

    if(*data){
        double fx;
        double A;
        QPainterPath sinF;
        sinF.moveTo(0,0);
        for(int x = 0; x < x_top; x++){
            if(!(x % hT))
                A = 1.0 * iten[x / hT] * y_top / ymaxs;
            fx = abs(A * sin( 2 * PI * x / T));
            sinF.lineTo(x, fx);
        }
        sinF.lineTo(x_top, 0);
        painter.setPen(*pen3);
        painter.setBrush(*brush);
        painter.drawPath(sinF);
    }

    painter.setPen(*pen1);
    painter.drawLine(0, 0, x_top, 0);
    painter.drawLine(0, 0, 0, y_top);

    for(int y = 0, i = 0; y <= y_top; y+=y_span, i++){
        if(i % 5)
            painter.drawLine(0, y, 7, y);
    }
    for(int y = 0; y <= y_top; y += y_grid){
            painter.drawLine(0, y, 14, y);
    }
    for(int x = 0,i = 0; x <= x_top; x+=x_span, i++){
            if(i % 3)
                painter.drawLine(x, 0, x, 7);
    }
    for(int x = 0; x <= x_top; x+=x_hgrid){
            painter.drawLine(x, 0, x, 14);
    }

    painter.setPen(*pen2);
    for(int y = 0; y <= y_top; y+=y_grid){
        painter.drawLine(15, y, x_top, y);
    }
    for(int x = 0; x <= x_top; x += x_hgrid){
        painter.drawLine(x, 15, x, y_top);
    }

}

void OSCgraph::mousePressEvent(QMouseEvent *event){
    if(*data){
        int x = event->x();
        int y = event->y();
        if(x > xofs && x < xofs + x_top  && y < yofs && y > yofs - y_top)
            QToolTip::showText(event->globalPos(),QString::number(dptr[aind + (x-xofs) / hT]), this);
    }
}

void OSCgraph::setCycle(int ind){

    if(!(*data)){
        return ;
    }

    *data = false;

    ymaxs = 20;
    aind = ind;
    tind = ind * cycle;

    for(int i = 0, tmp; i < 7; i++){
        tmp = aind + i;
        iten[i] = dptr[tmp];
        ymaxs = qMax(ymaxs, dptr[tmp]);
    }
    timeSpan.clear();
    daySpan.clear();
    for(int i = 0, tmp; i <= 6; i+=2){
        tmp = tind + i * cycle;
        timeSpan.append(timeList.at(tmp % 48));
        daySpan.append(dayList.at(tmp / 48));
    }
    while(ymaxs % 20){
        ymaxs++;
    }
    *data = true;
    repaint();
}

void OSCgraph::setCycLen(int c){

    if(*data){
        cycle = cycleList[c];
        iten_len = 336 / cycleList[c];
        dptr = outer[c];
        setCycle(0);
    }
    else{
        cycle = cycleList[c];
        iten_len = 336 / cycle;
        dptr = outer[c];
    }

}
