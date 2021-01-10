#include "timeopt.h"
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDebug>
#include <QSlider>

#define BEGW 25
#define BEGH 5

TimeOpt::TimeOpt(QWidget *parent)
    : QWidget(parent)
{
    ind = 0;
    hid = false;
    timeList = new int[337];
    timeList[0] = 2000000;
    for(int i = 1; i < 337; i++){
        if(!(i % 48))
            timeList[i] = timeList[i-48] + 1000000;
        else if(i % 2)
            timeList[i] = timeList[i-1] + 3000;
        else
            timeList[i] = timeList[i-2] + 10000;
    }

    initUI();
    connect(up, &QPushButton::clicked, this, &TimeOpt::on_up_clicked);
    connect(down, &QPushButton::clicked, this, &TimeOpt::on_down_clicked);
    connect(shid, &QPushButton::clicked, this, &TimeOpt::on_shid_clicked);
    connect(aply, &QPushButton::clicked, this, &TimeOpt::on_aply_clicked);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_moved(int)));

}

TimeOpt::~TimeOpt()
{
}


void TimeOpt::initUI(){


    this->resize(220, 193);
    this->setAutoFillBackground(true);
    this->setPalette(QPalette(QPalette::Background, QColor("#f0f0f0")));

    QPalette palette(QPalette::Background, Qt::white);
    wg = new QWidget(this);
    wg->setGeometry(25, 5, 195, 183);

    sd = new QLabel(wg);
    sd->setGeometry(0,0,30,40);
    sd->setAutoFillBackground(true);
    sd->setPalette(palette);
    sd->setAlignment(Qt::AlignCenter);
    sd->setFont(QFont("宋体",10));

    QLabel *cb = new QLabel(wg);
    cb->setGeometry(35,0,30,40);
    cb->setAutoFillBackground(true);
    cb->setPalette(palette);
    cb->setAlignment(Qt::AlignCenter);
    cb->setFont(QFont("宋体",10));
    cb->setText("-");

    sh = new QLabel(wg);
    sh->setGeometry(70,0,30,40);
    sh->setAutoFillBackground(true);
    sh->setPalette(palette);
    sh->setAlignment(Qt::AlignCenter);
    sh->setFont(QFont("宋体",10));

    QLabel *col = new QLabel(wg);
    col->setGeometry(105,0,30,40);
    col->setAutoFillBackground(true);
    col->setPalette(palette);
    col->setAlignment(Qt::AlignCenter);
    col->setFont(QFont("宋体",10));
    col->setText(":");

    sm = new QLabel(wg);
    sm->setGeometry(140,0,30,40);
    sm->setAutoFillBackground(true);
    sm->setPalette(palette);
    sm->setAlignment(Qt::AlignCenter);
    sm->setFont(QFont("宋体",10));

    QLabel *To = new QLabel(wg);
    To->setGeometry(0,40,170,10);
    To->setAutoFillBackground(true);
    To->setPalette(QPalette(QPalette::Background, QColor("#006699")));

    ed = new QLabel(wg);
    ed->setGeometry(0,50,30,40);
    ed->setAutoFillBackground(true);
    ed->setPalette(palette);
    ed->setAlignment(Qt::AlignCenter);
    ed->setFont(QFont("宋体",10));

    QLabel *ecb = new QLabel(wg);
    ecb->setGeometry(35,50,30,40);
    ecb->setAutoFillBackground(true);
    ecb->setPalette(palette);
    ecb->setAlignment(Qt::AlignCenter);
    ecb->setFont(QFont("宋体",10));
    ecb->setText("-");

    eh = new QLabel(wg);
    eh->setGeometry(70,50,30,40);
    eh->setAutoFillBackground(true);
    eh->setPalette(palette);
    eh->setAlignment(Qt::AlignCenter);
    eh->setFont(QFont("宋体",10));

    QLabel *ecol = new QLabel(wg);
    ecol->setGeometry(105,50,30,40);
    ecol->setAutoFillBackground(true);
    ecol->setPalette(palette);
    ecol->setAlignment(Qt::AlignCenter);
    ecol->setFont(QFont("宋体",10));
    ecol->setText(":");

    em = new QLabel(wg);
    em->setGeometry(140,50,30,40);
    em->setAutoFillBackground(true);
    em->setPalette(palette);
    em->setAlignment(Qt::AlignCenter);
    em->setFont(QFont("宋体",10));

    up = new QPushButton(wg);
    up->setObjectName("up");
    up->setGeometry(175,25,17,20);


    down = new QPushButton(wg);
    down->setObjectName("down");
    down->setGeometry(175,45,17,20);

    slider = new QSlider(Qt::Horizontal, wg);
    slider->move(0, 100);
    slider->setMinimum(0);
    slider->setMaximum(335);

    QLabel *lb = new QLabel(wg);
    lb->setGeometry(0,132, 90 ,20);
    lb->setAlignment(Qt::AlignCenter);
    lb->setText("Grid Size: ");

    input = new QLineEdit(wg);
    input->setGeometry(90,132,50,20);

    QLabel *lb2 = new QLabel(wg);
    lb2->setGeometry(140, 132, 30,20);
    lb2->setAlignment(Qt::AlignCenter);
    lb2->setText("m");

    aply = new QPushButton(wg);
    aply->setGeometry(0,158,170, 25);
    aply->setText("Show");

    shid = new QPushButton(this);
    shid->setObjectName("shid");
    shid->setGeometry(0,0,18,20);

    wg->setStyleSheet("\
        QPushButton#up{background-image:url(:/up.png);}\
        QPushButton#down{background-image:url(:/down.png);}\
        QPushButton#up:pressed{background-image:url(:/pup.png);}\
        QPushButton#down:pressed{background-image:url(:/pdown.png);}\
        QSlider:horizontal{background-color:#4B4B4B;padding: 0;margin: 0;width:180px;height:26px;border: 1px solid #000000;}\
        QSlider::groove:horizontal{height:6px;width:150px;padding:0;background-color:transparent;}\
        QSlider::add-page:horizontal{background:#dddddd;}\
        QSlider::sub-page:horizontal{background:#00ccff;}\
        QSlider::handle:horizontal{border-image:url(:/handle.png);margin:-5px -8px;}\
    ");

    this->setStyleSheet("QPushButton#shid{border-image:url(:/hide.png)} \
                         QPushButton#shid:hover{border-image:url(:/hidemask.png)}");


    setTime(0);

}


void TimeOpt::setTime(int i){
    ind = i;
    sd->setText(QString::number(timeList[i] / 1000000));
    sh->setText(QString::number((timeList[i] % 1000000) / 10000));
    sm->setText(QString::number((timeList[i] % 10000) / 100));
    ed->setText(QString::number(timeList[i+1] / 1000000));
    eh->setText(QString::number((timeList[i+1] % 1000000) / 10000));
    em->setText(QString::number((timeList[i+1] % 10000) / 100));
}

void TimeOpt::on_up_clicked(){
    ind = (++ind) % 336;
    setTime(ind);
    slider->setValue(ind);
}

void TimeOpt::on_down_clicked(){
    if(ind){
        --ind;
    }else{
        ind = 335;
    }
    setTime(ind);
    slider->setValue(ind);
}

void  TimeOpt::on_aply_clicked(){
    if(input->text() == "")
        return;
    QString msg = QString("../grid/") + QString::number(timeList[ind]) + QString("-") + QString::number(timeList[ind+1])
            + QString(".json?callback=callback&") + input->text();
    aply->setEnabled(false);
    emit msgUp(msg);
}

void TimeOpt::on_shid_clicked(){
    if(!hid){
    wg->setVisible(false);
    this->resize(20, this->height());
    this->setStyleSheet("QPushButton#shid{border-image:url(:/show.png)} \
                         QPushButton#shid:hover{border-image:url(:/showmask.png)}");
    }else{
        this->resize(220, this->height());
        wg->setVisible(true);
        this->setStyleSheet("QPushButton#shid{border-image:url(:/hide.png)} \
                             QPushButton#shid:hover{border-image:url(:/hidemask.png)}");
    }
    hid = !hid;
}

void TimeOpt::on_slider_moved(int v){
    ind = v;
    setTime(v);
}
