#include "mif.h"
#include "lnkbutton.h"
#include "fstfinf.h"
#include "trdfinf.h"
#include "forinf.h"
#include "fixinf.h"
#include "eiginf.h"
#include "ourteam.h"
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>


//public:
Mif::Mif(QWidget *parent)
    : QDialog(parent)
{
    hasTree = false;
    initUI();
    connect(btf1, &LnkButton::buttonClicked, this, &Mif::onf1);
    connect(btf3, &LnkButton::buttonClicked, this, &Mif::onf3);
    connect(btf56, &LnkButton::buttonClicked, this, &Mif::onf56);
    connect(btf4, &LnkButton::buttonClicked, this, &Mif::onf4);
    connect(btf8, &LnkButton::buttonClicked, this, &Mif::onf8);
    connect(btfx, &LnkButton::buttonClicked, this, &Mif::onfx);
}

Mif::~Mif()
{
}


//protected:
void Mif::closeEvent(QCloseEvent *e){
    QMessageBox::StandardButton result = QMessageBox::question(this,"Message","quit");
    if(result == QMessageBox::Yes){
        if(!hasTree){
            QMessageBox::warning(this, "Message", "Please Wait Until Tree Done");
        }else{
            emit mifonQuit();
        }
    }
    e->ignore();
}


//private:
void Mif::initUI(){
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#03024e"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->resize(670,450);

    palette.setColor(QPalette::WindowText, Qt::white);

    btf1 = new LnkButton("locus", ":/locus.png", QRect(100,70,111,101), this);
    QLabel *f1_lb = new QLabel(this);
    f1_lb->setPalette(palette);
    f1_lb->move(119,171);
    f1_lb->setText("Show Locus");

    btf3 = new LnkButton("ser", ":/ser.png", QRect(280, 70, 111, 101), this);
    QLabel *f3_lb = new QLabel(this);
    f3_lb->setPalette(palette);
    f3_lb->move(290, 171);
    f3_lb->setText("Area Amount");

    btf56 = new LnkButton("sepflow", ":/sepflow.png", QRect(460, 70, 111, 101), this);
    QLabel *f56_lb = new QLabel(this);
    f56_lb->setPalette(palette);
    f56_lb->move(470, 171);
    f56_lb->setText("Traffic Flow");

    btf4 = new LnkButton("grid", ":/grid.png", QRect(100,240,111,101),this);
    QLabel *f4_lb = new QLabel(this);
    f4_lb->setPalette(palette);
    f4_lb->move(110,341);
    f4_lb->setText("Grid Density");

    btf8 = new LnkButton("kPath", ":/kPath.png", QRect(280, 240, 111, 101), this);
    QLabel *f8_lb = new QLabel(this);
    f8_lb->setPalette(palette);
    f8_lb->move(310, 341);
    f8_lb->setText("Kth Path");

    btfx = new LnkButton("team",":/team.png", QRect(460, 240, 111, 101), this);
    QLabel *fx_lb = new QLabel(this);
    fx_lb->setPalette(palette);
    fx_lb->move(450, 341);
    fx_lb->setText("Professional Team");
}

//public slots:
void Mif::treeDone(){
    hasTree = true;
    qDebug("tree done");
}

void Mif::amount(double &lowlng, double &lowlat, double &highlng, double &highlat, int &tbegin, int &tend){
    emit amountUp(lowlng, lowlat, highlng, highlat, tbegin, tend);
}

void Mif::amountDone(int result){
    emit amountDoneDown(result);
}

void Mif::volume5(double &llng1, double &llat1, double &hlng1, double &hlat1,
                  double &llng2, double &llat2, double &hlng2, double &hlat2){
    emit volume5Up(llng1, llat1, hlng1, hlat1, llng2, llat2, hlng2, hlat2);
}

void Mif::volume6(double &llng, double &llat, double &hlng, double &hlat){
    emit volume6Up(llng, llat, hlng, hlat);
}

void Mif::volumeDone(int *ptr){
    emit volumeDoneDown(ptr);
}


void Mif::kPath(double &llng1, double &llat1, double &hlng1, double &hlat1,
                double &llng2, double &llat2, double &hlng2, double &hlat2)
{
    emit kPathUp(llng1, llat1, hlng1, hlat1, llng2, llat2, hlng2, hlat2);
}

void Mif::kPathDone(int tol){
    emit kPathDoneDown(tol);
}


//private slots:
void Mif::onf1(){
    f1 = new FstFInf;
    f1->show();
    f1->setBackInf(this);
    this->hide();
}

void Mif::onf3(){

    if(!hasTree){
        QMessageBox::warning(this, "Warning", "Tree unDone");
        return ;
    }

    f3 = new TrdFInf;
    connect(f3, &TrdFInf::amountUp, this, &Mif::amount);
    connect(this, &Mif::amountDoneDown, f3, &TrdFInf::amountDone);

    f3->setBackInf(this);
    f3->show();
    this->hide();
}

void Mif::onf56(){

    if(!hasTree){
        QMessageBox::warning(this, "Warning", "Tree unDone");
        return ;
    }

    f56 = new FixInf;
    f56->setBackInf(this);

    connect(f56, &FixInf::Volume5Up, this, &Mif::volume5);
    connect(f56, &FixInf::Volume6Up, this, &Mif::volume6);
    connect(this, &Mif::volumeDoneDown, f56, &FixInf::volumeDone);

    f56->show();
    this->hide();
}

void Mif::onf4(){
    f4 = new ForInf;
    f4->setBackInf(this);

    f4->show();
    this->hide();
}

void Mif::onf8(){
    if(!hasTree){
        QMessageBox::warning(this, "Warning", "Tree unDone");
        return ;
    }

    f8 = new EigInf;
    f8->setBackInf(this);

    connect(f8, &EigInf::kPathUp, this, &Mif::kPath);
    connect(this, &Mif::kPathDoneDown, f8, &EigInf::kPathDone);

    f8->show();
    this->hide();
}

void Mif::onfx(){
    fx = new OurTeam;
    fx->setBackInf(this);

    fx->show();
    this->hide();
}
