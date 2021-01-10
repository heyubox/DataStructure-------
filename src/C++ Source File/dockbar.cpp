#include "dockbar.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QValidator>

//public:
DockBar::DockBar(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    connect(btShow, &QPushButton::clicked, this, &DockBar::on_btShow_clicked);
}

DockBar::~DockBar()
{

}


//private:
void DockBar::initUI(){
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(Qt::black));
    QFont font;
    font.setFamily("Microsoft Yahei");  font.setPixelSize(20);
    font.setBold(true);

    this->setAutoFillBackground(true);
    this->setPalette(palette);
    palette.setColor(QPalette::Background, QColor(Qt::transparent));

    QLabel *title = new QLabel(this);
    title->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    title->setAlignment(Qt::AlignCenter);
    title->setFont(font);
    palette.setColor(QPalette::WindowText, Qt::white);
    title->setPalette(palette);
    title->setText(tr("Show Point Locus"));

    QHBoxLayout *top = new QHBoxLayout();
    top->addStretch(1);
    top->addWidget(title);
    top->addStretch(1);

    QLabel *idName = new QLabel(this);
    idName->setText(tr("Taxi ID: "));
    idName->setPalette(palette);
    idName->setAlignment(Qt::AlignCenter);
    idName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QRegExp rx("[1-9]\\d?\\d?\\d?\\d?");
    QValidator *validator = new QRegExpValidator(rx, this);
    edit = new QLineEdit(this);
    edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    edit->setValidator(validator);

    QHBoxLayout *mid = new QHBoxLayout();
    mid->addStretch(1);
    mid->addWidget(idName);
    mid->addWidget(edit);
    mid->addStretch(1);


    btShow = new QPushButton(this);
    btShow->setMinimumSize(180,0);
    btShow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btShow->setText(tr("Show"));
    btShow->setEnabled(false);

    QHBoxLayout *bottom = new QHBoxLayout();
    bottom->addStretch(1);
    bottom->addWidget(btShow);
    bottom->addStretch(1);

    QHBoxLayout *hl = new QHBoxLayout;
    hints = new QLabel;
    palette.setBrush(QPalette::WindowText, Qt::red);
    hints->setPalette(palette);
    hints->setText("           ");
    hl->addStretch(1);
    hl->addWidget(hints);

    QVBoxLayout *whole = new QVBoxLayout();
    whole->addLayout(top);
    whole->addSpacing(10);
    whole->addLayout(mid);
    whole->addSpacing(15);
    whole->addLayout(bottom);
    whole->addLayout(hl);
    whole->addStretch(1);

    this->setLayout(whole);
}

//public slots:
void DockBar::enablebtShow(){
    btShow->setEnabled(true);
}

//private slots
void DockBar::on_btShow_clicked(){
    QString tmp = edit->text();
    hints->setText("           ");
    bool ok = false;
    int num = tmp.toInt(&ok);
    if(ok && num > 0 && num < 10358){
        tmp = QString("../jsonSet/") + tmp + QString(".json?callback=callback");
        emit(showLocusUp(tmp));
        btShow->setEnabled(false);
    }else{
        hints->setText("input error");
    }
}



