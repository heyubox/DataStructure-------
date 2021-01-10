#include "easyplt.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDebug>


EasyPlt::EasyPlt(QWidget *parent)
    : QWidget(parent)
{
    hid = false;
    initUI();
    connect(hidbt,&QPushButton::clicked, this, &EasyPlt::on_hidbt_clicked);
}

EasyPlt::~EasyPlt()
{
}

void EasyPlt::initUI(){

    this->setWindowFlag(Qt::FramelessWindowHint);
    QPalette palette1(QPalette::Background, Qt::white);

    this->setAutoFillBackground(true);
    this->setPalette(palette1);

    hidbt = new QPushButton(this);
    hidbt->setObjectName("hidbt");
    hidbt->setGeometry(0,130,18,20);

    wg = new QWidget(this);
    wg->setGeometry(22,0,260,300);

    QGroupBox *gb1 = new QGroupBox(wg);
    gb1->setTitle("areaA");

    QVBoxLayout *ly1 = new QVBoxLayout;
    llng1 = new QLineEdit;
    llat1 = new QLineEdit;
    hlng1 = new QLineEdit;
    hlat1 = new QLineEdit;

    llng1->setEnabled(false);
    llat1->setEnabled(false);
    hlng1->setEnabled(false);
    hlat1->setEnabled(false);

    QLabel *lb1 = new QLabel("lowedge lng: ");
    QLabel *lb2 = new QLabel("lowedge lat: ");
    QLabel *lb3 = new QLabel("highedge lng: ");
    QLabel *lb4 = new QLabel("highedge lat: ");

    llng1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    llat1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hlng1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hlat1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout *ly11 = new QHBoxLayout;
    QHBoxLayout *ly12 = new QHBoxLayout;
    QHBoxLayout *ly13 = new QHBoxLayout;
    QHBoxLayout *ly14 = new QHBoxLayout;

    ly11->addWidget(lb1);
    ly11->addWidget(llng1);
    ly12->addWidget(lb2);
    ly12->addWidget(llat1);
    ly13->addWidget(lb3);
    ly13->addWidget(hlng1);
    ly14->addWidget(lb4);
    ly14->addWidget(hlat1);

    ly1->addLayout(ly11);
    ly1->addLayout(ly12);
    ly1->addLayout(ly13);
    ly1->addLayout(ly14);

    gb1->setLayout(ly1);


    QGroupBox *gb2 = new QGroupBox("areaB",wg);
    QVBoxLayout *ly2 = new QVBoxLayout;
    llng2 = new QLineEdit;
    llat2 = new QLineEdit;
    hlng2 = new QLineEdit;
    hlat2 = new QLineEdit;

    llng2->setEnabled(false);
    llat2->setEnabled(false);
    hlng2->setEnabled(false);
    hlat2->setEnabled(false);

    QLabel *lb5 = new QLabel("lowedge lng: ");
    QLabel *lb6 = new QLabel("lowedge lat: ");
    QLabel *lb7 = new QLabel("highedge lng: ");
    QLabel *lb8 = new QLabel("highedge lat: ");

    llng2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    llat2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hlng2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    hlat2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout *ly21 = new QHBoxLayout;
    QHBoxLayout *ly22 = new QHBoxLayout;
    QHBoxLayout *ly23 = new QHBoxLayout;
    QHBoxLayout *ly24 = new QHBoxLayout;

    ly21->addWidget(lb5);
    ly21->addWidget(llng2);
    ly22->addWidget(lb6);
    ly22->addWidget(llat2);
    ly23->addWidget(lb7);
    ly23->addWidget(hlng2);
    ly24->addWidget(lb8);
    ly24->addWidget(hlat2);

    ly2->addLayout(ly21);
    ly2->addLayout(ly22);
    ly2->addLayout(ly23);
    ly2->addLayout(ly24);

    gb2->setLayout(ly2);

    hints = new QLabel(wg);
    hints->setGeometry(10,125,85,30);
    palette1.setBrush(QPalette::WindowText, Qt::red);
    hints->setPalette(palette1);
    hints->setText("     ");

    ar = new QRadioButton("A",wg);
    br = new QRadioButton("B",wg);
    bg = new QButtonGroup(wg);
    bg->addButton(ar);
    bg->addButton(br);
    ar->setChecked(true);

    aply = new QPushButton("show",wg);
    aply->setGeometry(188, 130, 40, 30);

    gb1->setGeometry(3,0,230,120);
    gb2->setGeometry(3,160,230, 120);
    ar->move(93, 135);
    br->move(143, 135);

    this->setStyleSheet("QLineEdit{background-color:white;}\
                        QPushButton#hidbt{border-image:url(:/hide.png);}\
                        QPushButton#hidbt:hover{border-image:url(:/hidemask.png);}\
                        ");


}

void EasyPlt::on_hidbt_clicked(){
    if(!hid){
        wg->hide();
        this->setStyleSheet("QLineEdit{background-color:white;}\
                             QPushButton#hidbt{border-image:url(:/show.png);}\
                             QPushButton#hidbt:hover{border-image:url(:/showmask.png);}\
                            ");
        this->resize(20, this->height());
    }else{
        this->resize(282, this->height());
        wg->show();
        this->setStyleSheet("QLineEdit{background-color:white;}\
                            QPushButton#hidbt{border-image:url(:/hide.png);}\
                            QPushButton#hidbt:hover{border-image:url(:/hidemask.png);}\
                            ");
    }
    hid = !hid;
}

