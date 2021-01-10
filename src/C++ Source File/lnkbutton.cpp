#include "lnkbutton.h"
#include<QMouseEvent>
#include <QtDebug>
#include <QRect>

//public:
LnkButton::LnkButton(QString objName,QString fileName, QRect rect, QWidget *parent): QLabel(parent)
{
    this->setObjectName(objName);
    this->setScaledContents(true);
    this->setGeometry(rect);
    this->setStyleSheet(QString("#") + objName + QString("{image:url(") + fileName + QString(");}"));
}

//protect:
void LnkButton::enterEvent(QEvent *ev){
   this->setPixmap(QPixmap(":/mask.png"));
}

void LnkButton::leaveEvent(QEvent *ev){
    this->clear();
}

void LnkButton::mousePressEvent(QMouseEvent *ev){
    pos = QPoint(ev->x(), ev->y());
}

void LnkButton::mouseReleaseEvent(QMouseEvent *ev){
    if(pos == QPoint(ev->x(), ev->y())){
        qDebug() << "clicked";
        emit buttonClicked();
    }
}



